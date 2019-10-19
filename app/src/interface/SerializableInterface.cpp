#include <telebar/interface/SerializableInterface.hpp>

std::vector<std::tuple<std::string, std::string>> SerializableInterface::getTuplesFromStream(const std::string& stream) const {
    std::string input = stream;
    if (input[0] != '{') {
        fprintf(stderr, "ERROR, could not deserialize User, expected '{' at the start of stream");
        return *(new std::vector<std::tuple<std::string, std::string>>());
    }
    if (input[input.size() - 1] != '}') {
        fprintf(stderr, "ERROR, could not deserialize User, expected '}' at the end of stream");
        return *(new std::vector<std::tuple<std::string, std::string>>());
    }

    input = input.substr(1, input.size() - 2);

    int fieldDelimiterIndex;
    int tagDelimiterIndex;

    std::string field;
    std::string tag;
    std::string value;

    auto tuples = new std::vector<std::tuple<std::string, std::string>>();


    while ( !input.empty() ) {
        fieldDelimiterIndex = input.find('|');
        if (fieldDelimiterIndex != std::string::npos) {
            field = input.substr(0, fieldDelimiterIndex);
            input = input.substr(fieldDelimiterIndex + 1, input.size());
        } else {
            field = input;
            input.clear();
        }

        tagDelimiterIndex = field.find(':');
        if (tagDelimiterIndex == std::string::npos) {
            fprintf(stderr, "ERROR, could not deserialize User, expected ':' as tag:value separator");
            return *(new std::vector<std::tuple<std::string, std::string>>());
        }

        tag = field.substr(0, tagDelimiterIndex);
        value = field.substr(tagDelimiterIndex + 1, input.size() - 1);

        tuples->emplace_back(tag, value);
    }

    return *tuples;
}

std::vector<std::string> SerializableInterface::getStreamsFromSerializedInput(const std::string& serializedInput) const {
    std::vector<std::string> streams;
    std::string stream;
    std::string input = serializedInput;
    int streamDelimiterIndex;
    while ( !input.empty() ) {
        streamDelimiterIndex = input.find(',');
        if (streamDelimiterIndex != std::string::npos) {
            stream = input.substr(0, streamDelimiterIndex);
            input = input.substr(streamDelimiterIndex + 1, input.size());
        } else {
            stream = input;
            input.clear();
        }
        streams.emplace_back(stream);
    }

    return streams;
}

std::vector<std::vector<std::tuple<std::string, std::string>>>
SerializableInterface::getTuplesCollectionFromStream(const std::string &stream) const {
    std::vector<std::vector<std::tuple<std::string, std::string>>> tuplesCollection;

    for (auto& splitedStream : getStreamsFromSerializedInput(stream)) {
        tuplesCollection.emplace_back(getTuplesFromStream(splitedStream));
    }

    return tuplesCollection;
}
