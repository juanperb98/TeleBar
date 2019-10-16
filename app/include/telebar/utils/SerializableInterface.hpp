#ifndef _TELEBAR_UTILS_SERIALIZABLE_INTERFACE_
#define _TELEBAR_UTILS_SERIALIZABLE_INTEFCACE_

class SerializableInterface{
public:
    virtual std::string serialize() const = 0;
    virtual bool deserialize(std::string stream) = 0;
};

#endif