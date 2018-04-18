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
    
    mutable 
    Node ** check_pointer;
    Node ** connections;
    Node ** stack_pointer;
    Node ** end_pointer;
    Tile ** tile;
    
    Node * progress_pointers;
    Node * router_pointers;   
    
    void addRouter(Node*);

    public:
    Node();
    Node(const int& size);
    Node(Tile**);
    ~Node();
    void clear()
    {
        stack_pointer = check_pointer = connections;
        size = 0;
    }
    void reset()
    {
        check_pointer = connections;
    }
    bool empty() const
    {
        return connections == stack_pointer;
    }
    const Node* end() const
    {
        return *end_pointer;
    }
    Node* start() const
    {
        check_pointer = connections;
        return curr();
    }
    Node* curr() const
    {
        return (check_pointer != stack_pointer) ? *check_pointer : *end_pointer;
    }
    Node* next() const
    {
        return (check_pointer != stack_pointer) ? *(++check_pointer) : *end_pointer;
    }
    Node* prev() const
    {
        return (check_pointer != end_pointer) ? *(--check_pointer) : *end_pointer;
    }
    Node* last() const
    {
        return *(stack_pointer-1);
    }
    void bind(Node * next)
    {
        *(stack_pointer++) = next;
        ++size;
    }
    void append(const Node* N)
    {
        for(Node* n=N->start(); N->curr() != N->end(); n = N->next())
        {
            bind(n);
        }
    }
    void initProgress();
    void purify(); // might crash the program!
    void setTile(Tile*);
    void moveTile(Node*);
    
    static void connect(Node* n1, Node* n2)
    {
        n1->bind(n2);
        n2->bind(n1);
    }
};

#endif // NODE_H
