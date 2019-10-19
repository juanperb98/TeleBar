#ifndef _TELEBAR_INTERFACE_ORMINTERFACE_
#define _TELEBAR_INTERFACE_ORMINTERFACE_

#include <telebar/interface/SerializableInterface.hpp>

class ORMInterface: public SerializableInterface{
protected:
    int id_;
    std::string tableName_;
public:
    ORMInterface() : id_(-1), tableName_(""), SerializableInterface() {};
    const std::string& getTableName() const {return this->tableName_;};
    int getId() const {return this->id_;};
};

#endif