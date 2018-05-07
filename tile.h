/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef TILE_H
#define TILE_H

#include "enums.h"

class Node;
class Tile
{
    static const char* TileNamesToText[];
    Ally opponent;
    Piece piece;
    Node * position;
    bool is_active;
public:
    Tile();
    Tile(const Ally& A, const Piece& piece);
    void setNode(Node *);
    Node * getPosition() const
    {
        return position;
    }
    const Ally& getAlly() const
    {
        return opponent;
    }
    const Piece& getPiece() const
    {
        return piece;
    }
    const char* getName() const
    {
        return TileNamesToText[static_cast<int>(piece)];
    }
    bool empty() const
    {
        return opponent == Ally::NONE;
    }
    bool isActive() const
    {
        return is_active;
    }
    void activate()
    {
        is_active = true;
    }
    void deactivate()
    {
        is_active = false;
    }
};

#endif // TILE_H
