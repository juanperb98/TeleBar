#ifndef _TELEBAR_UTILS_ORM_
#define _TELEBAR_UTILS_ORM_

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <sqlite3.h>

#include <telebar/interface/ORMInterface.hpp>

class ORM{
private:
    std::string stream_;
    sqlite3* db_;

private:
    /*static int genericCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        for(int i = 0; i<argc; i++)
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "nullptr");
        printf("\n"); return 0;
    }*/

    static int selectCallback(void *stream, int argc, char **argv, char **azColName) {
        auto buffer = (std::string*) stream;
        if (!buffer->empty())
            (*buffer) += ",";
        (*buffer) += "{";
        for(int i = 0; i<argc; i++) {
            if (i != 0)
                (*buffer) += "|";
            (*buffer) += std::string(azColName[i]) + ":" + std::string(argv[i]);
        }
        (*buffer) += "}";
        return 0;
    }


public:
    explicit ORM(const std::string& dbName) {
        int rc = sqlite3_open(dbName.c_str(), &this->db_);

        if( rc ) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_));
            exit(0);
        }
    };

    ~ORM() {
        sqlite3_close(db_);
    };

    template <class ObjectType>
    bool createTable() {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        ObjectType object;
        std::string querry = "CREATE TABLE "+ object.getTableName() +"(id INT PRIMARY KEY ";
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            querry += ", " + std::get<0>(tuples[i]) + " CHAR(255)";
        }
        querry += ");";

        char *zErrMsg = nullptr;
        int rc;
        char *sql;

        rc = sqlite3_exec(db_, querry.c_str(), nullptr, nullptr, &zErrMsg);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            return false;
        }
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
        return true;
    };



    template <class ObjectType>
    int save(ObjectType object) {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        std::string querry = "INSERT INTO "+ object.getTableName() + " VALUES (";
        int id = object.getId();
        if ( id == -1 ) {
            auto allObjects = this->all<ObjectType>();
            for (auto objectAux : allObjects) {
                if (objectAux.getId() > id)
                    id = objectAux.getId();
            }
            id++;
        }
        querry += std::to_string(id);
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            querry += ", '" + std::get<1>(tuples[i]) + "'";
        }
        querry += ");";
        char *zErrMsg = nullptr;
        int rc;
        char *sql;
        rc = sqlite3_exec(db_, querry.c_str(), selectCallback, &this->stream_, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            return -1;
        }
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);

        return id;
    };



    template <class ObjectType>
    std::vector<ObjectType> all() {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        this->stream_.clear();
        ObjectType object;
        std::vector<ObjectType> objects;
        std::string querry = "select * from "+ object.getTableName() + ";";
        char *zErrMsg = nullptr;
        int rc;
        char *sql;
        rc = sqlite3_exec(db_, querry.c_str(), selectCallback, &this->stream_, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            return objects;
        }
        auto streams = object.getStreamsFromSerializedInput(this->stream_);
        for (auto& auxStream : streams) {
            object.deserialize(auxStream);
            objects.emplace_back(object);
        }
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
        return objects;
    };

    template <class ObjectType>
    ObjectType getById(int id) {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        ObjectType object;
        this->stream_.clear();
        std::string querry = "select * from "+ object.getTableName();
        querry += " where id is "+ std::to_string(id) +";";
        char *zErrMsg = nullptr;
        int rc;
        char *sql;
        rc = sqlite3_exec(db_, querry.c_str(), selectCallback, &this->stream_, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            return object;
        }
        object.deserialize(this->stream_);
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
        return object;
    };



    template <class ObjectType>
    std::vector<ObjectType> getByField(const std::string& field, const std::string& value) {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        this->stream_.clear();
        ObjectType object;
        std::vector<ObjectType> objects;
        std::string querry = "select * from "+ object.getTableName();
        querry += " where " + field + " is '"+ value +"';";

        char *zErrMsg = nullptr;
        int rc;
        char *sql;
        rc = sqlite3_exec(db_, querry.c_str(), selectCallback, &this->stream_, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            return objects;
        }
        auto streams = object.getStreamsFromSerializedInput(this->stream_);
        for (auto& auxStream : streams) {
            object.deserialize(auxStream);
            objects.emplace_back(object);
        }
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
        return objects;
    };

    template <class ObjectType>
    bool update(ObjectType object) {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        this->stream_.clear();
        std::string querry = "update "+ object.getTableName() + " set ";
        bool firstField = true;
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            if ( i != 1 )
                querry += ", ";
            querry += std::get<0>(tuples[i]) + " = '" + std::get<1>(tuples[i]) + "'";
        }
        querry += " where id is " + std::to_string(object.getId()) + ";";

        char *zErrMsg = nullptr;
        int rc;
        char *sql;
        rc = sqlite3_exec(db_, querry.c_str(), selectCallback, &this->stream_, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            return false;
        }
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
        return true;
    };


    template <class ObjectType>
    bool remove(ObjectType object) {
        sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        this->stream_.clear();
        std::string querry = "delete from "+ object.getTableName() + " where id is " + std::to_string(object.getId()) + ";";
        bool firstField = true;
        auto tuples = object.getTuplesFromStream(object.serialize());

        char *zErrMsg = nullptr;
        int rc;
        char *sql;
        rc = sqlite3_exec(db_, querry.c_str(), selectCallback, &this->stream_, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
            sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
            return false;
        }
        sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, nullptr);
        sqlite3_exec(this->db_, "COMMIT;", nullptr, nullptr, nullptr);
        return true;
    };

};

#endif