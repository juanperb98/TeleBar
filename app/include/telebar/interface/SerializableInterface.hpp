#ifndef _TELEBAR_INTERFACE_SERIALIZABLEINTERFACE_
#define _TELEBAR_INTERFACE_SERIALIZABLEINTERFACE_

#include <vector>
#include <string>
#include <tuple>

class SerializableInterface{
public:
    virtual std::string serialize() const = 0;
    virtual bool deserialize(std::string stream) = 0;
    const std::vector<std::tuple<std::string, std::string>>& getTuplesFromStream(std::string stream) const;
};

#endif