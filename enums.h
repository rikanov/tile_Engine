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

enum class State
{
    DEFFENSIVE,
    OFFENSIVE
};

#endif
