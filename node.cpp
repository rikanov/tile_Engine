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
 :inner_store(new Node* [max_size+1])
 ,teleports(nullptr)
 ,environment(nullptr)
 ,tile(nullptr)
{
     reinit();
}
Node::Node()
 :Node(4)
 {    
 }
 
void Node::initTeleports()
{
    teleports = new Node(6);
    for(start(); notEnded(); next())
    {
        for(Node * n = curr()->start(); curr()->notEnded(); n = curr()->next())
        {
            if(n != this)
            {
                teleports->bind(n);
            }
        }
    }
}

void Node::initEnvironment()
{
    environment = new Node(20);
    environment->bind(this);
    value = 0;
    for(Node * next = environment->start(); environment->notEnded(); next = environment->next())
    {
        if(next->value == 3)
        {
            continue;
        }
        for(next->start(); next->notEnded(); next->next())
        {
            if(environment->imbue(next->curr()))
            {
                next->curr()->value = next->value +1;
            }
        }
    }
    for(environment->start(); environment->notEnded(); environment->next())
    {
        environment->curr()->value = 0;
    }
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

bool Node::imbue(Node* next)
{
    for(Node ** n = connections; n != stack_pointer; ++n)
    {
        if(*n == next)
        {
            return false;
        }
    }
    bind(next);
    return true;
}

void Node::append(const Node* N, const Node::AppendDirection& d)
{
    if(d == NORMAL)
    {
        for(Node* n=N->start(); N->curr() != N->end(); n = N->next())
        {
            bind(n);
        }
    }
    else
    {
        for(Node* n=N->back(); N->curr() != N->end(); n = N->prev())
        {
            bind(n);
        }            
    }
}
 
void Node::makeView(const int& inc)
{
    for(environment->start(); environment->notEnded(); environment->next())
    {
        if(environment->curr()->getSearchLevel() != inc)
        {
            environment->curr()->setSearchLevel(inc);
        }
    }
}

void Node::closeView(const int& inc)
{
    for(environment->start(); environment->notEnded(); environment->next())
    {
        if(environment->curr()->getSearchLevel() == inc)
        {
            environment->curr()->backSearchLevel();
        }
    }
}

Node::~Node()
{
    check_pointer = stack_pointer = connections = end_pointer = nullptr;
    delete[] inner_store;
    delete teleports;
    delete environment;
}
