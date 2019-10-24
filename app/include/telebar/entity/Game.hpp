#ifndef _TELEBAR_ENTITY_GAME_
#define _TELEBAR_ENTITY_GAME_

#include <telebar/entity/Piece.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/interface/ORMInterface.hpp>

#include <vector>
#include <random>



class Game : public ORMInterface{
    struct Player{
        int userId = -1;
        std::vector<Piece> inHandPieces;
        bool hasTurn = false;
    };

private:
    int numberOfPlayers_;
    std::vector<Piece> inBoardPieces_;
    std::vector<Player> players_;
    std::vector<Piece> toStealPieces_;

public:
    void debug(){
        this->inBoardPieces_.emplace_back(this->getStealPiece());
    }

    Game();

    explicit Game(std::string stream);

    int getNumberOfPlayers() const;

    int getNumberOfPlayersCap() const;

    void setNumberOfPlayersCap(int numberOfPlayers);

    bool addPlayer(User user);

    Piece getStealPiece();

    bool canPutPieceToTheLeft(Piece piece) const;

    bool canPutPieceToTheRight(Piece piece) const;

    bool putPieceToTheLeft(Piece piece);

    bool putPieceToTheRight(Piece piece);

    bool canStealPiece();

    void startGame();

    std::string serialize() const override;

    bool deserialize(std::string stream) override;

    void prepareObjectForPlayer(int userId);

    std::string serializeForPlayer(int userId) const;


};

#endif