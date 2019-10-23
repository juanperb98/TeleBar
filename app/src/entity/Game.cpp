#include <telebar/entity/Game.hpp>

Game::Game(){
    this->tableName_ = "game";
    this->id_ = -1;
    this->numberOfPlayers_ = 2 ;
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 6 ; ++j) {
            this->toStealPieces_.emplace_back(Piece(i, j));
        }
    }
}


Game::Game(std::string stream) {
    this->tableName_ = "game";
    this->deserialize(stream);
}

int Game::getNumberOfPlayers() const {
    return numberOfPlayers_;
}

void Game::setNumberOfPlayers(int numberOfPlayers) {
    numberOfPlayers_ = numberOfPlayers;
}

bool Game::addPlayer(User user) {
    if (this->players_.size() >= this->numberOfPlayers_)
        return false;
    Player player;
    player.userId = user.getId();
    this->players_.emplace_back(player);
    return true;
}

Piece Game::getStealPiece() {
    if (!canStealPiece())
        return Piece(-1, -1);
    std::random_device rd;
    int index = rd() % this->toStealPieces_.size();
    Piece piece = this->toStealPieces_[index];
    this->toStealPieces_.erase(this->toStealPieces_.begin() + index);
    return piece;
}

bool Game::putPieceToTheLeft(Piece piece) {
    if (!canPutPieceToTheLeft(piece))
        return false;
    this->inBoardPieces_.emplace(this->inBoardPieces_.begin(), piece);
    return true;
}

bool Game::putPieceToTheRight(Piece piece) {
    if (!canPutPieceToTheRight(piece))
        return false;
    this->inBoardPieces_.emplace_back(piece);
    return true;
}

bool Game::canPutPieceToTheLeft(Piece piece) const {
    return this->inBoardPieces_[0].isCompatible(piece);
}

bool Game::canPutPieceToTheRight(Piece piece) const {
    return this->inBoardPieces_[this->inBoardPieces_.size() - 1].isCompatible(piece);
}

bool Game::canStealPiece() {
    return !this->toStealPieces_.empty();
}

void Game::startGame() {
    for (auto &player : this->players_) {
        for (int i = 0; i < 7; ++i) {
            player.inHandPieces.emplace_back(this->getStealPiece());
        }
    }
    this->inBoardPieces_.emplace_back(this->getStealPiece());

    // checks the first player
    int firstPlayerIndex = 0;
    int firstPlayerWinningValue = 0;

    for (int i = 0; i < players_.size(); ++i) {
        for (auto &piece : players_[i].inHandPieces) {
            if (firstPlayerWinningValue < piece.getValue()) {
                firstPlayerIndex = i;
                firstPlayerWinningValue = piece.getValue();
            }
        }
    }
    this->players_[firstPlayerIndex].hasTurn = true;
}

std::string Game::serialize() const {
    std::string stream;
    stream += "{";

    stream += "id:" + std::to_string(this->getId()) + "|";
    stream += "numberOfPlayers:" + std::to_string(this->getNumberOfPlayers()) + "|";
    {
        stream += "inBoardPieces:[";
        for (int i = 0; i < this->inBoardPieces_.size() ; ++i) {
            if (i != 0)
                stream += ",";
            stream += this->inBoardPieces_[i].serialize();
        }
        stream += "]|";
    }
    {
        stream += "toStealPieces:[";
        for (int i = 0; i < this->toStealPieces_.size() ; ++i) {
            if (i != 0)
                stream += ",";
            stream += this->toStealPieces_[i].serialize();
        }
        stream += "]|";
    }
    {
        stream += "players:[";
        for (int i = 0; i < players_.size(); ++i) {
            if (i != 0)
                stream += ",";
            stream += "{";
            stream += "userId:" + std::to_string(this->players_[i].userId) + "|";
            {
                stream += "inHandPieces:[";
                for (int j = 0; j < this->players_[i].inHandPieces.size() ; ++j) {
                    if (j != 0)
                        stream += ",";
                    stream += this->players_[i].inHandPieces[j].serialize();
                }
                stream += "]|";
            }
            stream += "hasTurn:" + (this->players_[i].hasTurn ? std::to_string(1) : std::to_string(0));
            stream += "}";
        }

        stream += "]";
    }

    stream += "}";
    return stream;
}

bool Game::deserialize(std::string stream) {
    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "id")
            this->id_ = atoi(std::get<1>(tuple).c_str());

        else if (std::get<0>(tuple) == "numberOfPlayers")
            this->setNumberOfPlayers( atoi(std::get<1>(tuple).c_str()));

        else if (std::get<0>(tuple) == "inBoardPieces")
            for (auto& pieceStream : this->getStreamsFromSerializedInput(std::get<1>(tuple).substr(1, std::get<1>(tuple).size() - 2)))
                this->inBoardPieces_.emplace_back(Piece(pieceStream));

        else if (std::get<0>(tuple) == "toStealPieces")
            for (auto& pieceStream : this->getStreamsFromSerializedInput(std::get<1>(tuple).substr(1, std::get<1>(tuple).size() - 2)))
                this->toStealPieces_.emplace_back(Piece(pieceStream));

        else if (std::get<0>(tuple) == "players") {
            Player* player;
            for (std::string &playerStream : this->getStreamsFromSerializedInput(std::get<1>(tuple).substr(1, std::get<1>(tuple).size() - 2))) {
                player = new Player;
                player->inHandPieces.clear();
                for (auto& playerTuple :  this->getTuplesFromStream(playerStream)) {
                    if (std::get<0>(playerTuple) == "userId")
                        player->userId = atoi(std::get<1>(playerTuple).c_str());

                    else if (std::get<0>(playerTuple) == "hasTurn")
                        player->hasTurn = (bool)atoi(std::get<1>(playerTuple).c_str());

                    else if (std::get<0>(playerTuple) == "inHandPieces")
                        for (auto& pieceStream : this->getStreamsFromSerializedInput(std::get<1>(playerTuple).substr(1, std::get<1>(playerTuple).size() - 2)))
                            player->inHandPieces.emplace_back(Piece(pieceStream));
                }
                this->players_.emplace_back(*player);
            }
        }
    }

    return true;
}

void Game::prepareObjectForPlayer(int userId) {
    this->toStealPieces_.clear();
    for (int i = 0; i < this->getNumberOfPlayers(); ++i) {
        if (this->players_[i].userId == userId)
            this->players_.erase(this->players_.begin() + i);
    }
}

std::string Game::serializeForPlayer(int userId) const {
    Game newGame = *this;
    newGame.prepareObjectForPlayer(userId);
    return newGame.serialize();
}

