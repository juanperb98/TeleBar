#ifndef _TELEBAR_INTERFACE_ORMINTERFACE_
#define _TELEBAR_INTERFACE_ORMINTERFACE_

#include <string>

template <class Object>

class ORMInterface {
protected:
    int id_;
    std::string tablename_;

public:
    virtual bool save() const = 0;
    static std::vector<Object> all() {return *(new std::vector<Object>);}; // TODO: this can be implemented in this class
    static std::vector<Object> getById(int id) {return *(new std::vector<Object>);}; // TODO: This can be implemented in this class
};

#endif