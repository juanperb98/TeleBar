#include <telebar/entity/Piece.hpp>

#include <iostream>

using std::ostream;

ostream& operator<<(ostream &os, const Piece &p){
    os << "|" << p.getLeft() << "·" << p.getRight() << "|";
}

Piece::Piece() {
    r_ = -1;
    l_ = -1;
}

Piece::Piece(const int &l, const int &r) {
    l_ = l;
    r_ = r;
}

Piece::Piece(std::string stream) {
    this->deserialize(stream);
}

bool Piece::isValid() const {
    return (r_ >= 0) && (r_ <= 6) && (l_ >= 0) && (l_ <= 6);
}

int Piece::getLeft() const {
    return l_;
}

int Piece::getRight() const {
    return r_;
}

bool Piece::operator==(const Piece &p) const {
    return ((r_ == p.r_) && (l_ == p.l_));
}

bool Piece::isCompatible(const Piece &p) const {
    return((r_ == p.r_) \
 || (l_ == p.l_) \
 || (r_ == p.l_) \
 || (l_ == p.r_));
}

void Piece::flip() {
    int aux = r_;
    r_ = l_;
    l_ = aux;
}

std::string Piece::serialize() const {
    return std::string("{L:") + std::to_string(this->l_) + "|R:" + std::to_string(this->r_) + "}";
}

bool Piece::deserialize(std::string stream) {
    std::vector<std::tuple<std::string, std::string>> tuples = this->getTuplesFromStream(stream);

    for (auto& tuple : tuples) {
        if (std::get<0>(tuple) == "L")
            this->l_= (atoi(std::get<1>(tuple).c_str())) ;
        else if (std::get<0>(tuple) == "R")
            this->r_ = (atoi(std::get<1>(tuple).c_str()));
    }

    return true;
}

int Piece::getValue() const {
    if (r_ == 6 && r_ == l_) return 500;
    else if (r_ == 5 && r_ == l_) return 499;
    else if (r_ == 4 && r_ == l_) return 498;
    else if (r_ == 3 && r_ == l_) return 497;
    else if (r_ == 2 && r_ == l_) return 496;
    else if (r_ == 1 && r_ == l_) return 495;
    else if (r_ == 0 && r_ == l_) return 494;
    else return r_ + l_;
}

bool Piece::operator!=(const Piece &p) const {
    return !((*this)==p);
}


