#ifndef _TELEBAR_ENTITY_GAMEACTION_
#define _TELEBAR_ENTITY_GAMEACTION_

#include <telebar/interface/SerializableInterface.hpp>
#include <telebar/entity/Piece.hpp>

class GameAction : public SerializableInterface {
private:
    std::string action_;
public:
    const std::string &getAction() const;

    const Piece &getPiece() const;

private:
    Piece piece_;


public:
    GameAction();

    GameAction(std::string action, const Piece& piece);

    explicit GameAction(std::string stream);

    std::string serialize() const override;

    bool deserialize(std::string stream) override;
};

#endif