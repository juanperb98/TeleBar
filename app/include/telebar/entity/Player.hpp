#ifndef PLAYER_H
#define PLAYER_H

#include <telebar/entity/Piece.hpp>
#include <vector>

class Player{


  private:
    int _id;
    std::vector<Piece> _pieces;


  public:


    inline Player(int id) : _id(id){}

    inline int getID()
    {
        return this->_id;
    }

    inline std::vector<Piece> getPieces()
    {
        return this->_pieces;
    }

    inline void pickPiece(const Piece& piece)
    {
      this->_pieces.push_back(piece);
    }

    bool checkPiece(int left, int right);

    bool removePiece(Piece& piece);
};

#endif