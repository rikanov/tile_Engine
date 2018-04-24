#ifndef ENUMS__H
#define ENUMS__H

enum class Ally
{
    FOE = -1,
    NONE,
    OWN
};

enum class Piece
{
    NONE = 0,
    AURADIN    = 1,
    HUNTER     = 2,
    TELEPORTER = 4,
    WARRIOR    = AURADIN | HUNTER,
    GUARDIAN   = AURADIN | TELEPORTER,
    ASSASSIN   = HUNTER  | TELEPORTER,
    COMMANDER  = AURADIN | HUNTER | TELEPORTER
};

inline bool archetype(const Piece& p1, const Piece& p2)
{
    return (static_cast<int>(p1) & static_cast<int>(p2)) != 0;
}
inline bool archetype(const Piece& p1, const Piece& p2, const Piece& p3)
{
    return (static_cast<int>(p1) & static_cast<int>(p2) & static_cast<int>(p3)) != 0;
}
#endif
