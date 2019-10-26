#ifndef PIECE_H
#define PIECE_H

#include <telebar/interface/SerializableInterface.hpp>

#include <iostream>

class Piece : public SerializableInterface{

private:
    int r_;
    int l_;

public:
    Piece ();

    Piece(const int &number1, const int &number2);

    explicit Piece(std::string stream);

    bool isValid() const;

    int getLeft() const;

    int getRight() const;

    bool operator==(const Piece &p) const;

    bool operator!=(const Piece &p) const;

    bool isCompatible(const Piece &p) const;

    void flip();

    std::string serialize() const override;

    bool deserialize(std::string stream) override;

    int getValue() const;


};


#endif