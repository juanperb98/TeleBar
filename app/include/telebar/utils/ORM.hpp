#ifndef _TELEBAR_UTILS_ORM_
#define _TELEBAR_UTILS_ORM_

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <sqlite3.h>

#include <telebar/interface/ORMInterface.hpp>

class ORM{
protected:
    //MYSQL* connection;
    sqlite3* db;

private:
    std::string stream;

private:
    static int genericCallback(void *NotUsed, int argc, char **argv, char **azColName) {
        for(int i = 0; i<argc; i++)
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        printf("\n"); return 0;
    }

    static int selectCallback(void *stream, int argc, char **argv, char **azColName) {
        auto buffer = (std::string*) stream;
        if (!buffer->empty())
            (*buffer) += ",";
        (*buffer) += "{";
        for(int i = 0; i<argc; i++) {
            //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
            if (i != 0)
                (*buffer) += "|";
            (*buffer) += std::string(azColName[i]) + ":" + std::string(argv[i]);
        }
        (*buffer) += "}";
        return 0;
    }


public:
    explicit ORM(std::string dbName) {
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
        ObjectType object;
        std::string querry = "CREATE TABLE "+ object.getTableName() +"(id INT PRIMARY KEY NOT NULL";
        auto tuples = object.getTuplesFromStream(object.serialize());
        for (int i = 1; i < tuples.size(); ++i) {
            querry += ", " + std::get<0>(tuples[i]) + " CHAR(255)";
        }
        querry += ");";

        std::cout<<querry<<"\n";

        char *zErrMsg = 0;
        int rc;
        char *sql;

        rc = sqlite3_exec(db, querry.c_str(), nullptr, 0, &zErrMsg);

        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    };



    template <class ObjectType>
    bool save(ObjectType object) {
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

        std::cout<<querry<<"\n";


        char *zErrMsg = 0;
        int rc;
        char *sql;
        rc = sqlite3_exec(db, querry.c_str(), selectCallback, &this->stream, &zErrMsg);
        if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }

        return true;
    };



    template <class ObjectType>
    std::vector<ObjectType> all() {
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
            return objects;
        }
        auto streams = object.getStreamsFromSerializedInput(this->stream);
        for (auto& auxStream : streams) {
            object.deserialize(auxStream);
            objects.emplace_back(object);
        }
        return objects;
    };

    template <class ObjectType>
    ObjectType getById(int id) {
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
            return object;
        }
        object.deserialize(this->stream);
        return object;
    };



    template <class ObjectType>
    std::vector<ObjectType> getByField(const std::string& field, const std::string& value) {
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
            return objects;
        }
        auto streams = object.getStreamsFromSerializedInput(this->stream);
        for (auto& auxStream : streams) {
            object.deserialize(auxStream);
            objects.emplace_back(object);
        }
        return objects;
    };

    template <class ObjectType>
    bool update(ObjectType object) {
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
            return false;
        }
        return true;
    };


    template <class ObjectType>
    bool remove(ObjectType object) {
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
            return false;
        }
        return true;
    };

};

#endif