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
    std::string stream;
    sqlite3* db;

private:
    /*static int genericCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        for(int i = 0; i<argc; i++)
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
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
        int rc = sqlite3_open(dbName.c_str(), &this->db);

        if( rc ) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            exit(0);
        }
    };

    ~ORM() {
        sqlite3_close(db);
    };

    template <class ObjectType>
    bool createTable() {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        ObjectType object;
        std::string querry = "CREATE TABLE "+ object.getTableName() +"(id INT PRIMARY KEY NOT NULL";
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            querry += ", " + std::get<0>(tuples[i]) + " CHAR(255)";
        }
        querry += ");";

        char *zErrMsg = 0;
        int rc;
        char *sql;

        rc = sqlite3_exec(db, querry.c_str(), nullptr, 0, &zErrMsg);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            return false;
        }
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return true;
    };



    template <class ObjectType>
    bool save(ObjectType object) {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        std::string querry = "INSERT INTO "+ object.getTableName() + " VALUES (";
        int id = object.getId();
        if ( id == -1 ) {
            auto allObjects = this->all<ObjectType>();
            for (auto object : allObjects) {
                if (object.getId() > id)
                    id = object.getId();
            }
            id++;
        }
        querry += std::to_string(id);
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            querry += ", '" + std::get<1>(tuples[i]) + "'";
        }
        querry += ");";

        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            return false;
        }
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return true;
    };



    template <class ObjectType>
    std::vector<ObjectType> all() {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        this->stream.clear();
        ObjectType object;
        std::vector<ObjectType> objects;
        std::string querry = "select * from "+ object.getTableName() + ";";
        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            return objects;
        }
        auto streams = object.getStreamsFromSerializedInput(this->stream);
        for (auto& auxStream : streams) {
            object.deserialize(auxStream);
            objects.emplace_back(object);
        }
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return objects;
    };

    template <class ObjectType>
    ObjectType getById(int id) {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        ObjectType object;
        this->stream.clear();
        std::string querry = "select * from "+ object.getTableName();
        querry += " where id is "+ std::to_string(id) +";";
        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            return object;
        }
        object.deserialize(this->stream);
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return object;
    };



    template <class ObjectType>
    std::vector<ObjectType> getByField(const std::string& field, const std::string& value) {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        this->stream.clear();
        ObjectType object;
        std::vector<ObjectType> objects;
        std::string querry = "select * from "+ object.getTableName();
        querry += " where " + field + " is '"+ value +"';";

        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            return objects;
        }
        auto streams = object.getStreamsFromSerializedInput(this->stream);
        for (auto& auxStream : streams) {
            object.deserialize(auxStream);
            objects.emplace_back(object);
        }
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return objects;
    };

    template <class ObjectType>
    bool update(ObjectType object) {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        this->stream.clear();
        std::string querry = "update "+ object.getTableName() + " set ";
        bool firstField = true;
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            if ( i != 1 )
                querry += ", ";
            querry += std::get<0>(tuples[i]) + " = '" + std::get<1>(tuples[i]) + "'";
        }
        querry += " where id is " + std::to_string(object.getId()) + ";";

        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            return false;
        }
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return true;
    };


    template <class ObjectType>
    bool remove(ObjectType object) {
        sqlite3_exec(this->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        this->stream.clear();
        std::string querry = "delete from "+ object.getTableName() + " where id is " + std::to_string(object.getId()) + ";";
        bool firstField = true;
        auto tuples = object.getTuplesFromStream(object.serialize());

        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
            return false;
        }
        sqlite3_exec(this->db, "END TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(this->db, "COMMIT;", NULL, NULL, NULL);
        return true;
    };

};

#endif