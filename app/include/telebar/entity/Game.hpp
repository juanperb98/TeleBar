#ifndef _TELEBAR_ENTITY_GAME_
#define _TELEBAR_ENTITY_GAME_

#include <telebar/entity/Piece.hpp>
#include <telebar/entity/User.hpp>
#include <telebar/interface/ORMInterface.hpp>

#include <vector>
#include <random>

struct Player{
    int userId = -1;
    std::vector<Piece> inHandPieces;
    bool hasTurn = false;
    std::string username = "";
};

class Game : public ORMInterface{



private:
    int numberOfPlayersCap_;
    std::vector<Piece> inBoardPieces_;
    std::vector<Player> players_;
    std::vector<Piece> toStealPieces_;

protected:
    Piece getStealPiece();

    bool removePieceToCurrentPlayer(Piece piece);

    bool currentPlayerCanStealPiece() const;

    bool currentPlayerCanPass() const;

    bool currentPlayerCanPutPieceToTheLeft(Piece piece) const;

    bool currentPlayerCanPutPieceToTheRight(Piece piece) const;

    /**
     * checks if the current player can place a piece at the start or end of the board from it's hand
     * @return true if the action can be made, false otherwise
     */
    bool currentPlayerCanPlace() const;

    void nextTurn();

    int getCurrentPlayerIndex() const;

public:
    Game();

    explicit Game(std::string stream);

    int getNumberOfPlayers() const;

    int getNumberOfPlayersCap() const;

    void setNumberOfPlayersCap(int numberOfPlayers);

    bool addPlayer(User user);

    std::vector<int> getPlayersIds() const;

    bool currentPlayerPutPieceToTheLeft(Piece piece);

    bool currentPlayerPutPieceToTheRight(Piece piece);

    bool currentPlayerSteal();

    bool currentPlayerPass();

    const std::vector<Piece> &getInBoardPieces() const;

    const std::vector<Player> &getPlayers() const;

    bool currentPlayerHasPiece(Piece piece) const;

    void startGame();

    bool hasTurn(int userId) const;

    std::string serialize() const override;

    bool deserialize(std::string stream) override;

    void prepareObjectForPlayer(int userId);

    const std::vector<Piece> &getToStealPieces() const;

    std::string serializeForPlayer(int userId) const;

    bool hasEnded() const;

    Player getWinningPlayer() const;
};

#endif