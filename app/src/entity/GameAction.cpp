#include <telebar/entity/GameAction.hpp>

GameAction::GameAction() {

};

GameAction::GameAction(std::string action, const Piece& piece) {
    this->action_ = action;
    this->piece_.deserialize(piece.serialize());
}

std::string GameAction::serialize() const {
    return std::string("{action:") + this->action_ + "|piece:[" + this->piece_.serialize() + "]}";
}

bool GameAction::deserialize(std::string stream) {
    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "action")
            this->action_ = std::get<1>(tuple);

        else if (std::get<0>(tuple) == "piece")
            this->piece_ = Piece(std::get<1>(tuple).substr(1, std::get<1>(tuple).size()-2));
    }

    return true;
}

const std::string &GameAction::getAction() const {
    return action_;
}

const Piece &GameAction::getPiece() const {
    return piece_;
}

GameAction::GameAction(std::string stream) {
    this->deserialize(stream);
}
