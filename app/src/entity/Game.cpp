#include <telebar/entity/Game.hpp>

Game::Game(){
    this->tableName_ = "game";
    this->id_ = -1;
    this->numberOfPlayersCap_ = 2 ;
}


Game::Game(std::string stream) {
    this->tableName_ = "game";
    this->deserialize(stream);
}

int Game::getNumberOfPlayersCap() const {
    return numberOfPlayersCap_;
}

void Game::setNumberOfPlayersCap(int numberOfPlayers) {
    numberOfPlayersCap_ = numberOfPlayers;
}

bool Game::addPlayer(User user) {
    if (this->players_.size() >= this->numberOfPlayersCap_)
        return false;
    Player player;
    player.userId = user.getId();
    this->players_.emplace_back(player);
    return true;
}

std::vector<int> Game::getPlayersIds() const {
    std::vector<int> retval;
    for(auto& player : this->players_)
        retval.emplace_back(player.userId);
    return retval;
}

Piece Game::getStealPiece() {
    std::random_device rd;
    int index = rd() % this->toStealPieces_.size();
    Piece piece = this->toStealPieces_[index];
    this->toStealPieces_.erase(this->toStealPieces_.begin() + index);
    return piece;
}

bool Game::currentPlayerPutPieceToTheLeft(Piece piece) {
    Piece flipablePiece(piece.serialize());
    //std::cout<<flipablePiece.serialize()<<"\n";
    if (!currentPlayerCanPutPieceToTheLeft(flipablePiece)) {
        flipablePiece.flip();
        //std::cout<<flipablePiece.serialize()<<"\n";
        if (!currentPlayerCanPutPieceToTheLeft(flipablePiece)) {
            fprintf(stderr, "ERROR, cant place piece in the board\n");
            return false;
        }
    }

    if (!removePieceToCurrentPlayer(piece)) {
        fprintf(stderr, "ERROR, Current player dont have that piece\n");
        return false;
    }

    this->inBoardPieces_.emplace(this->inBoardPieces_.begin(), piece);
    nextTurn();
    return true;
}

bool Game::currentPlayerPutPieceToTheRight(Piece piece) {
    Piece flipablePiece(piece.serialize());
    //std::cout<<flipablePiece.serialize()<<"\n";
    if (!currentPlayerCanPutPieceToTheRight(flipablePiece)) {
        flipablePiece.flip();
        //std::cout<<flipablePiece.serialize()<<"\n";
        if (!currentPlayerCanPutPieceToTheRight(flipablePiece)) {
            fprintf(stderr, "ERROR, cant place piece in the board\n");
            return false;
        }
    }

    if (!removePieceToCurrentPlayer(piece)) {
        fprintf(stderr, "ERROR, Current player dont have that piece\n");
        return false;
    }

    this->inBoardPieces_.emplace_back(piece);
    nextTurn();
    return true;
}

bool Game::currentPlayerCanPutPieceToTheLeft(Piece piece) const {
    std::cout<<"player:"<<piece.serialize()<<"board"<<this->inBoardPieces_[0].serialize()<<(this->inBoardPieces_[0].getRight() == piece.getLeft())<<"\n";
    if (currentPlayerHasPiece(piece))
        return this->inBoardPieces_[0].getLeft() == piece.getRight();
    return false;
}

bool Game::currentPlayerCanPutPieceToTheRight(Piece piece) const {
    std::cout<<"board"<<this->inBoardPieces_[this->inBoardPieces_.size() - 1].serialize()<<"player:"<<piece.serialize()<<(this->inBoardPieces_[this->inBoardPieces_.size() - 1].getRight() == piece.getLeft())<<"\n";
    if (currentPlayerHasPiece(piece))
        return this->inBoardPieces_[this->inBoardPieces_.size() - 1].getRight() == piece.getLeft();
    return false;
}

bool Game::currentPlayerCanStealPiece() const {
    if (currentPlayerCanPlace())
        return false;
    std::cout<<"canSteal\n";
    return !this->toStealPieces_.empty();
}

void Game::startGame() {
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 6 ; ++j) {
            this->toStealPieces_.emplace_back(Piece(i, j));
        }
    }

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
    stream += "numberOfPlayers:" + std::to_string(this->getNumberOfPlayersCap()) + "|";
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
    {
        stream += "|isFull:";
        stream += this->players_.size() < this->numberOfPlayersCap_ ? std::to_string(0) : std::to_string(1);
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
            this->setNumberOfPlayersCap(atoi(std::get<1>(tuple).c_str()));

        else if (std::get<0>(tuple) == "inBoardPieces") {
            this->inBoardPieces_.clear();
            for (auto &pieceStream : this->getStreamsFromSerializedInput(
                    std::get<1>(tuple).substr(1, std::get<1>(tuple).size() - 2)))
                this->inBoardPieces_.emplace_back(Piece(pieceStream));
        }

        else if (std::get<0>(tuple) == "toStealPieces") {
            this->toStealPieces_.clear();
            for (auto& pieceStream : this->getStreamsFromSerializedInput(std::get<1>(tuple).substr(1, std::get<1>(tuple).size() - 2)))
                this->toStealPieces_.emplace_back(Piece(pieceStream));
        }


        else if (std::get<0>(tuple) == "players") {
            this->players_.clear();
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
            this->players_[i].inHandPieces.clear();
    }
}

std::string Game::serializeForPlayer(int userId) const {
    Game newGame (this->serialize());
    newGame.prepareObjectForPlayer(userId);
    return newGame.serialize();
}

int Game::getNumberOfPlayers() const {
    return this->players_.size();
}

bool Game::hasTurn(int userId) const {
    for (auto& player : this->players_) {
        if (player.userId == userId)
            return player.hasTurn;
    }
    fprintf(stderr, "ERROR, player not in game, cant check for turn");
    return false;
}


bool Game::currentPlayerCanPlace() const {
    // gets the playing user
    Player playingUser = this->players_[getCurrentPlayerIndex()];
    std::cout<<"cheking if player can place\n";
    for (Piece& inHandPiece : playingUser.inHandPieces) {
        Piece piece(inHandPiece.serialize());
        std::cout<<piece.serialize()<<"\n";
        if (currentPlayerCanPutPieceToTheLeft(piece) || currentPlayerCanPutPieceToTheRight(piece))
            return true;
        piece.flip();
        std::cout<<piece.serialize()<<"\n";
        if (currentPlayerCanPutPieceToTheLeft(piece) || currentPlayerCanPutPieceToTheRight(piece))
            return true;
    }
    std::cout<<"cant place\n";
    return false;
}

bool Game::currentPlayerHasPiece(Piece piece) const {
    Player playingUser = this->players_[getCurrentPlayerIndex()];
    bool has = false;
    for (Piece& inHandPiece : playingUser.inHandPieces) {
        if (inHandPiece == piece)
            has = true;
    }
    piece.flip();
    for (Piece& inHandPiece : playingUser.inHandPieces) {
        if (inHandPiece == piece)
            has = true;
    }
    return has;

}

bool Game::removePieceToCurrentPlayer(Piece piece) {
    if (!currentPlayerHasPiece(piece))
        return false;


    for (int i = 0; i < this->players_[getCurrentPlayerIndex()].inHandPieces.size(); ++i) {
        if (this->players_[getCurrentPlayerIndex()].inHandPieces[i] == piece){
            this->players_[getCurrentPlayerIndex()].inHandPieces.erase(this->players_[getCurrentPlayerIndex()].inHandPieces.begin()+i);
            std::cout<<"removing piece to the player\n";
            return true;
        }
    }
    return false;

}

bool Game::currentPlayerCanPass() const {
    if (currentPlayerCanPlace())
        return false;

    if (currentPlayerCanStealPiece())
        return false;

    return true;
}

bool Game::currentPlayerSteal() {
    if (!currentPlayerCanStealPiece())
        return false;

    this->players_[getCurrentPlayerIndex()].inHandPieces.emplace_back(getStealPiece());

    nextTurn();
    return true;
}

bool Game::currentPlayerPass() {
    if (!currentPlayerCanPass())
        return false;

    Player playingUser = this->players_[getCurrentPlayerIndex()];

    nextTurn();
    return true;
}

void Game::nextTurn() {
    Player playingUser = this->players_[getCurrentPlayerIndex()];
}

int Game::getCurrentPlayerIndex() const {
    int currentPlayerIndex = -1;
    Player playingUser;
    for (int i = 0; i < this->getNumberOfPlayers(); ++i) {
        if (this->players_[i].hasTurn) {
            currentPlayerIndex = i;
        }
    }
    return currentPlayerIndex;
}


