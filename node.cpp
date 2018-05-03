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

#include "node.h"
#include "tile.h"
#include <iostream>

Node::Node(const int& max_size)
 :connections(new Node* [max_size])
 ,stack_pointer(connections)
 ,end_pointer(connections-1)
 ,check_pointer(connections)
 ,current_size(0)
{
}
Node::Node()
 :Node(3)
 {    
 }
 
void Node::setTile(Tile* t)
{
    tile = t;
    t->setNode(this);
}

void Node::moveTile(Node* n)
{
    n->setTile(tile);
}

int Node::getDef() const
{
    if( tile->getPiece() == Piece::NONE)
    {
        return -1;
    }
    const Ally A = getAlly();
    int result = isDefender(getPiece());
    if(result)
    {
        for(start(); curr() != end(); next())
        {
            result += curr()->getDef(A);
        }
    }
    return result;
}

bool Node::operator==(const Node& n) const
{
    if(current_size != n.size())
    {
        return false;
    }
    for(start(),n.start(); notEnded(); next(), n.next())
    {
        if(curr() != n.curr())
        {
            return false;
        }
    }
    return true;
}

Node * Node::find(Node* n) const
{
    Node * result;
    for(result = start(); notEnded(); result = next())
    {
        if( *result == *n)
        {
            break;
        }
    }
    return result;
}

Node::~Node()
{
    
}
