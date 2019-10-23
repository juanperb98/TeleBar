#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <cassert>



    class Piece{

    private:
        int _number1;
        int _number2;
    public:
        inline Piece(const int &number1, const int &number2){
            if((number1 < 1) || (number1 > 6) || (number2 < 1) || (number2 > 6))
            _number1 = number1;
            _number2 = number2;
        }


        inline int getNumber1() const {
            return _number1;
        }

        inline int getNumber2() const {
            return _number2;

        }

        inline bool operator==(const Piece &p) const{
            return(((_number1 == p._number1) && (_number2==p._number2)) || ((_number1 == p._number2) && (_number2==p._number1))); 
        }
        void invert(){
            int aux = _number1;
            _number1 = _number2;
            _number2 = aux;
        }
    };
#endif