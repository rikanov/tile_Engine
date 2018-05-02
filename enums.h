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

inline bool isMarchingGroup(const Piece& p1, const Piece& p2)
{
    return 3 & static_cast<int>(p1) & static_cast<int>(p2);
}
inline bool isMarchingGroup(const Piece& p1, const Piece& p2, const Piece& p3)
{
    return 3 & static_cast<int>(p1) & static_cast<int>(p2) & static_cast<int>(p3);
}
inline bool isDefender(const Piece& p)
{
    return static_cast<int>(p) & 1;
}
inline bool isAttacker(const Piece& p)
{
    return static_cast<int>(p) & 3;
}
inline bool isAuradin(const Piece& p)
{
    return static_cast<int>(p) & 1;
}
inline bool isHunter(const Piece& p)
{
    return static_cast<int>(p) & 2;
}
inline bool isTeleporter(const Piece& p)
{
    return static_cast<int>(p) & 4;
}
#endif
