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

#ifndef NODE_H
#define NODE_H

#include "node_auxiliary.h"
#include "tile.h"

class Node: public NodeAuxiliary
{
    short size;
    Node * connections[5] = {};
    Node ** stack_pointer;
    Node ** end_pointer;
    Node ** check_pointer;

    Tile * tile;
    
public:
    Node();
    void clear()
    {
        stack_pointer = check_pointer = connections;
        size = 0;
    }
    void reset()
    {
        check_pointer = connections;
    }
    const Node* end() const
    {
        return *end_pointer;
    }
    Node* next()
    {
        return check_pointer != stack_pointer ? *(check_pointer++) : *end_pointer;
    }
    void bind(Node * next)
    {
        *(stack_pointer++) = next;
        ++size;
    }
    
    void setTile(Tile*);
    void moveTile(Node*);
    
    static void connect(Node* n1, Node* n2)
    {
        n1->bind(n2);
        n2->bind(n1);
    }
};

#endif // NODE_H
