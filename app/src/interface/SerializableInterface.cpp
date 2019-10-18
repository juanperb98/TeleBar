#include <telebar/interface/SerializableInterface.hpp>

const std::vector<std::tuple<std::string, std::string>>& SerializableInterface::getTuplesFromStream(std::string stream) const {
    if (stream[0] != '{') {
        fprintf(stderr, "ERROR, could not deserialize User, expected '{' at the start of stream");
        return *(new std::vector<std::tuple<std::string, std::string>>());
    }
    if (stream[stream.size() - 1] != '}') {
        fprintf(stderr, "ERROR, could not deserialize User, expected '}' at the end of stream");
        return *(new std::vector<std::tuple<std::string, std::string>>());
    }

    stream = stream.substr(1, stream.size() - 2);

    int fieldDelimiterIndex;
    int tagDelimiterIndex;

    std::string field;
    std::string tag;
    std::string value;

    auto tuples = new std::vector<std::tuple<std::string, std::string>>();


    while (stream.size() > 1) {
        fieldDelimiterIndex = stream.find('|');
        if (fieldDelimiterIndex != std::string::npos) {
            field = stream.substr(0, fieldDelimiterIndex);
            stream = stream.substr(fieldDelimiterIndex + 1, stream.size());
        } else {
            field = stream;
            stream = "";
        }

        tagDelimiterIndex = field.find(':');
        if (tagDelimiterIndex == std::string::npos) {
            fprintf(stderr, "ERROR, could not deserialize User, expected ':' as tag:value separator");
            return *(new std::vector<std::tuple<std::string, std::string>>());
        }

        tag = field.substr(0, tagDelimiterIndex);
        value = field.substr(tagDelimiterIndex + 1, stream.size() - 1);

        tuples->emplace_back(tag, value);
    }

    return *tuples;
}
