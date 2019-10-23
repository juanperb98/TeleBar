#ifndef PLAYER_H
#define PLAYER_H

#include <telebar/entity/Piece.hpp>
#include <vector>

class Player{
private:
    int playerId_;
    std::vector<Piece> pieces_;


public:
    inline Player(int id) : playerId_(id){}

    inline int getPlayerId();

    inline std::vector<Piece> getPieces(){
        return this->pieces_;
    }

    inline void getPiece(const Piece& piece){
      this->pieces_.push_back(piece);
    }

};

#endif