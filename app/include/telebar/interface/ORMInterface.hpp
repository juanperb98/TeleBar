#ifndef _TELEBAR_INTERFACE_ORMINTERFACE_
#define _TELEBAR_INTERFACE_ORMINTERFACE_

#include <string>
#include <telebar/entity/User.hpp>

class ORMInterface{
private:
    int id;
    std::string tablename;

public:
    //virtual bool save() const = 0;
    //static virtual std::vector<> all() const = 0;
    //static virtual std::vector<> getById(int id) const = 0;
};

#endif