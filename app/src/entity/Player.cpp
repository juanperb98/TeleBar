#include <telebar/entity/Player.hpp>


bool Player::checkPiece(int left, int right)
{
    for (size_t i = 0; i < this->_pieces.size(); i++)
    {
        if ((this->_pieces[i].getNumber1() == left or this->_pieces[i].getNumber2() == left) && (this->_pieces[i].getNumber1() == right or this->_pieces[i].getNumber2() == right))
            return true;
    }
    return false;
}



bool Player::removePiece(Piece &piece)
{
    for (size_t i = 0; i < this->_pieces.size(); i++)
    {
        if (piece == this->_pieces[i])
        {
            this->_pieces.erase(this->_pieces.begin() + i);
            return true;
        }
    }
    return false;
}
