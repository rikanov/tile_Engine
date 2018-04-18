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
 ,progress_pointers(nullptr)
 ,router_pointers(nullptr)
 ,stack_pointer(connections)
 ,end_pointer(connections-1)
 ,check_pointer(connections)
 ,tile(new Tile*)
 ,size(0)
{
}
Node::Node()
 :Node(5)
 {    
 }
Node::Node(Tile** t)
 :Node()
 {
     tile = t;
 }

void Node::addRouter(Node *r)
{
    if(router_pointers == nullptr)
    {
        router_pointers = new Node;
    }
    router_pointers->bind(r);
}

void Node::setTile(Tile* t)
{
    *tile = t;
    t->setNode(this);
}

void Node::moveTile(Node* n)
{
    n->setTile(*tile);
}

void Node::purify()
{
    for(Node ** n = connections; n != stack_pointer; ++n)
    {
        delete (*n);
    }
    reset();
}

void Node::initProgress()
{
    progress_pointers = new Node;
    for(Node ** c = connections; c != stack_pointer; ++c)
    {
        Node * n = new Node(tile);
        progress_pointers->bind(n);
        (*c)->addRouter(n);
        for(Node ** ch = connections; ch != stack_pointer; ++ch)
        {
            if( c == ch)
            {
                continue;
            }
            n->bind(*ch);
        }
    }
}

Node::~Node()
{
    progress_pointers->purify();
    delete progress_pointers;
    delete router_pointers;
    delete tile;
}
