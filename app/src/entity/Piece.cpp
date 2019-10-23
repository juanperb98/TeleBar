#include <telebar/entity/Piece.hpp>

#include <iostream>

using std::ostream;

ostream& operator<<(ostream &os, const Piece &p){
    os << "|" << p.getNumber1() << "·" << p.getNumber2() <<"|";
}

Piece::Piece(const int &number1, const int &number2) {
    number1_ = number1;
    number2_ = number2;
}

Piece::Piece(std::string stream) {
    this->deserialize(stream);
}

bool Piece::isValid() const {
    return (number1_ >= 0) && (number1_ <= 6) && (number2_ >= 0) && (number2_ <= 6);
}

int Piece::getNumber1() const {
    return number1_;
}

int Piece::getNumber2() const {
    return number2_;
}

bool Piece::operator==(const Piece &p) const {
    return ((number1_ == p.number1_) && (number2_ == p.number2_));
}

bool Piece::isCompatible(const Piece &p) const {
    return((number1_ == p.number1_) \
                || (number2_ == p.number2_) \
                || (number1_ == p.number2_) \
                || (number2_ == p.number1_));
}

void Piece::flip() {
    int aux = number1_;
    number1_ = number2_;
    number2_ = aux;
}

std::string Piece::serialize() const {
    return std::string("{L:") + std::to_string(this->number1_) + "|R:" + std::to_string(this->number2_) + "}";
}

bool Piece::deserialize(std::string stream) {
    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "L")
            this->number1_= (atoi(std::get<1>(tuple).c_str())) ;
        else if (std::get<0>(tuple) == "R")
            this->number2_ = (atoi(std::get<1>(tuple).c_str()));
    }

    return true;
}

int Piece::getValue() const {
    if (number1_==6 && number1_==number2_) return 500;
    else if (number1_==5 && number1_==number2_) return 499;
    else if (number1_==4 && number1_==number2_) return 498;
    else if (number1_==3 && number1_==number2_) return 497;
    else if (number1_==2 && number1_==number2_) return 496;
    else if (number1_==1 && number1_==number2_) return 495;
    else if (number1_==0 && number1_==number2_) return 494;
    else return number1_ + number2_;
}


