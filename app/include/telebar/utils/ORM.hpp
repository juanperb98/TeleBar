#ifndef _TELEBAR_UTILS_ORM_
#define _TELEBAR_UTILS_ORM_

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <mysql/mysql.h>

#include <telebar/interface/ORMInterface.hpp>
#include <telebar/entity/User.hpp>


class ORM{
protected:
    MYSQL* connection;

public:
    ORM() {
        /*int port = stoi(std::string(getenv("DB_PORT")));
        char* user = getenv("DB_USER");
        char* password = getenv("DB_PASSWORD");
        char* dbname = getenv("DB_NAME");
        char* host = "localhost";*/

        int port = 3306;
        char user[50] = "admin";
        char password[50] = "password";
        char dbname[50] = "telebar";
        char host[50] = "localhost";

        this->connection = new MYSQL;
        this->connection->host = host;
        this->connection->user = user;
        this->connection->passwd = password;
        this->connection->db = dbname;
        this->connection->port = port;
    };

    template <class ObjectType>
    bool createTable(ObjectType object) {
        std::cout<<object.getTableName()<<"\n";
    };

    template <class ObjectType>
    bool save(ObjectType object) {
        std::cout<<object.serialize()<<"\n";
    };

    template <class ObjectType>
    std::vector<ObjectType> all() {
        std::vector<ObjectType> objects;

        return objects;
    };

    template <class ObjectType>
    std::vector<ObjectType> getById(int id) {
        std::vector<ObjectType> objects;

        return objects;
    };

    template <class ObjectType>
    std::vector<ObjectType> getByField(const std::string& field) {
        std::vector<ObjectType> objects;

        return objects;
    };
};

#endif