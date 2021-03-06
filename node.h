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
public:    
    enum AppendDirection
    {
        NORMAL,
        REVERT
    };
protected:
    int current_size;
    mutable 
    Node ** check_pointer;
    Node ** connections;
    Node ** stack_pointer;
    Node ** end_pointer;
    Node ** inner_store;
    
    Node * teleports;
    Node * environment;
    
    Tile * tile;
   
    public:
    Node();
    Node(const int& size);
    ~Node();
    void initTeleports();
    void initEnvironment();
    void getConnectionsFrom(const Node * from)
    {
        check_pointer = from->connections;
        connections   = from->connections;
        stack_pointer = from->stack_pointer;
        end_pointer   = from->end_pointer;
        current_size  = from->current_size;
    }
    void reinit()
    {
        current_size = 0;
        end_pointer = inner_store;
        check_pointer = stack_pointer = connections = end_pointer +1;
    }
    void clear()
    {
        stack_pointer = check_pointer = connections;
        current_size = 0;
    }
    bool notEnded() const
    {
        return check_pointer != stack_pointer;
    }
    void reset()
    {
        check_pointer = connections;
    }
    bool single() const
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
    const int& size() const
    {
        return current_size;
    }
    Node* curr() const
    {
        return (check_pointer != stack_pointer) ? *check_pointer : *end_pointer;
    }
    Node* next() const
    {
        check_pointer += (check_pointer != stack_pointer);
        return curr();
    }
    Node* prev() const
    {
        return (check_pointer != end_pointer) ? *(--check_pointer) : *end_pointer;
    }
    Node* last() const
    {
        return *(stack_pointer-1);
    }
    Node* at(const int& index) const
    {
        return *(connections+index);
    }
    void bind(Node * next)
    {
        *(stack_pointer++) = next;
        ++current_size;
    }
    bool imbue(Node * next);
    void pop()
    {
        if(current_size > 0)
        {
            --current_size;
            --stack_pointer;
        }
    }
    Node * back() const
    {
        check_pointer = stack_pointer - 1;
        return *check_pointer;
    }
    void append(const Node* N, const AppendDirection& d = NORMAL);
    const Ally& getAlly() const
    {
        return tile->getAlly();
    }
    const Piece& getPiece() const
    {
        return tile->getPiece();
    }
    const Piece getPiece(const Ally& A) const
    {
        return getAlly() == A ? tile->getPiece() : Piece::NONE;
    }
    int getDef() const;
    int getDef(const Ally& A ) const
    {
        return isDefender(getPiece(A));
    }
    const char * getName() const
    {
        return tile ? tile->getName() : "not defined";
    }
    bool empty() const
    {
        return tile->empty();
    }
    Node* getTeleports() const
    {
        return teleports;
    }
    Tile* getTile() const
    {
        return tile;
    }
    void setTile(Tile*);
    void moveTile(Node*);
    bool operator == (const Node& n) const;
    Node * find(Node * n) const;
    static void connect(Node* n1, Node* n2)
    {
        n1->bind(n2);
        n2->bind(n1);
    }
    void makeView(const int& inc);
    void closeView(const int& inc);
    static void swap(Node* n1, Node* n2)
    {
        Tile* tmp(n1->tile);
        n1->setTile(n2->tile);
        n2->setTile(tmp);
    }
};

#endif // NODE_H
