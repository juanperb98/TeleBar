#include <telebar/interface/SerializableInterface.hpp>
#include <iostream>

std::vector<std::tuple<std::string, std::string>> SerializableInterface::getTuplesFromStream(const std::string& stream) const {
    std::string input = stream;
    if (input[0] != '{') {
        fprintf(stderr, "ERROR, could not deserialize object, expected '{' at the start of stream\n");
        return *(new std::vector<std::tuple<std::string, std::string>>());
    }
    if (input[input.size() - 1] != '}') {
        fprintf(stderr, "ERROR, could not deserialize object, expected '}' at the end of stream\n");
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
        int arrayCompensation = 0;

        if (input.find('[') != std::string::npos && fieldDelimiterIndex > input.find('[')) {
            int oppenedArraysCount = 0;
            bool first = true;
            int lastArraySympolIndex = 0;
            while (oppenedArraysCount != 0 || first) {
                first = false;
                int nextArrayOpenIndex = input.find('[', lastArraySympolIndex + 1);
                int nextArrayCloseIndex = input.find(']', lastArraySympolIndex + 1);
                if (nextArrayCloseIndex == std::string::npos)
                    nextArrayCloseIndex = 9999999;
                if (nextArrayOpenIndex == std::string::npos)
                    nextArrayOpenIndex = 9999999;

                if (nextArrayOpenIndex == nextArrayCloseIndex && nextArrayCloseIndex == 9999999)
                    break;

                if (nextArrayOpenIndex < nextArrayCloseIndex){
                    lastArraySympolIndex = nextArrayOpenIndex;
                    oppenedArraysCount ++;
                }else{
                    lastArraySympolIndex = nextArrayCloseIndex;
                    oppenedArraysCount --;
                }
                arrayCompensation = 1;
            }
            fieldDelimiterIndex = lastArraySympolIndex;
        }
        // FIXME: clean this pasta party
        if (fieldDelimiterIndex != std::string::npos) {
            field = input.substr(0, fieldDelimiterIndex + arrayCompensation);
            if (fieldDelimiterIndex + 1 >= input.size() )
                arrayCompensation = 0;
            input = input.substr(fieldDelimiterIndex + 1 + arrayCompensation, input.size());
        } else {
            field = input;
            input.clear();
        }

        tagDelimiterIndex = field.find(':');
        if (tagDelimiterIndex == std::string::npos) {
            fprintf(stderr, "ERROR, could not deserialize object, expected ':' as tag:value separator\n");
            return *(new std::vector<std::tuple<std::string, std::string>>());
        }

        tag = field.substr(0, tagDelimiterIndex);
        value = field.substr(tagDelimiterIndex + 1, field.size());
        /*std::cout<<"field<"<<field<<">\n";
        std::cout<<"input<"<<input<<">\n";
        std::cout<<"tag<"<<tag<<">\n";
        std::cout<<"value<"<<value<<">\n\n";*/
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
        int oppenedArraysCount = 0;
        bool first = true;
        int lastArraySympolIndex = 0;
        streamDelimiterIndex = input.find(',');
        int arrayCompensation = 0;

        if (input.find('[') != std::string::npos && streamDelimiterIndex > input.find('[')) {
            while (oppenedArraysCount != 0 || first) {
                first = false;
                int nextArrayOpenIndex = input.find('[', lastArraySympolIndex + 1);
                int nextArrayCloseIndex = input.find(']', lastArraySympolIndex + 1);
                if (nextArrayCloseIndex == std::string::npos)
                    nextArrayCloseIndex = 9999999;
                if (nextArrayOpenIndex == std::string::npos)
                    nextArrayOpenIndex = 9999999;

                if (nextArrayOpenIndex == nextArrayCloseIndex && nextArrayCloseIndex == 9999999)
                    break;

                if (nextArrayOpenIndex < nextArrayCloseIndex){
                    lastArraySympolIndex = nextArrayOpenIndex;
                    oppenedArraysCount ++;
                }else{
                    lastArraySympolIndex = nextArrayCloseIndex;
                    oppenedArraysCount --;
                }
                arrayCompensation = 1;

            }
            streamDelimiterIndex = input.find(',', lastArraySympolIndex + 1);
        }



        if (streamDelimiterIndex != std::string::npos) {
            stream = input.substr(0, streamDelimiterIndex);
            input = input.substr(streamDelimiterIndex + 1, input.size());
        } else {
            stream = input;
            input.clear();
        }
        /*std::cout<<"stream<"<<stream<<">\n";
        std::cout<<"input<"<<input<<">\n";*/
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
