#include <telebar/entity/Piece.hpp>

#include <iostream>

using std::ostream;

ostream& operator<<(ostream &os, const Piece &p){
    os << "|" << p.getNumber1() << "·" << p.getNumber2() <<"|";
}
