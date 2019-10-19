#ifndef _TELEBAR_INTERFACE_SERIALIZABLEINTERFACE_
#define _TELEBAR_INTERFACE_SERIALIZABLEINTERFACE_

#include <vector>
#include <string>
#include <tuple>

class SerializableInterface{
public:
    virtual std::string serialize() const = 0;
    virtual bool deserialize(std::string stream) = 0;
    std::vector<std::tuple<std::string, std::string>> getTuplesFromStream(const std::string& input) const;
    std::vector<std::string> getStreamsFromSerializedInput(const std::string& serializedInput) const;
    std::vector<std::vector<std::tuple<std::string, std::string>>> getTuplesCollectionFromStream(const std::string& stream) const;
};

#endif