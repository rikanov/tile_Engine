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

#include <iostream>
#include "engine.h"

const Engine::Position Engine::StartPositions[] =
{
    { Piece::HUNTER      , 1, 0},
    { Piece::HUNTER      , 2, 0},
    { Piece::HUNTER      , 3, 0},
    { Piece::HUNTER      , 3, 1},
    { Piece::WARRIOR     , 4, 0},
    { Piece::ASSASSIN    , 4, 1},
    { Piece::GUARDIAN    , 6, 1},
    { Piece::AURADIN     , 5, 0},
    { Piece::AURADIN     , 6, 0},
    { Piece::AURADIN     , 7, 0},
    { Piece::AURADIN     , 7, 1},    
    { Piece::TELEPORTER  , 5, 2},
    { Piece::TELEPORTER  , 6, 2},
    { Piece::TELEPORTER  , 7, 2},
    { Piece::TELEPORTER  , 7, 3},
    { Piece::COMMANDER   , 5, 1},
};

Engine::Engine(const Ally& A, BoardView* B)
:Board()
,current_turn(A)
,move(new Node(6))
,assigned_view(nullptr)
{
    start();
    setView(B);
}

void Engine::start()
{
    int index = 0;
    for(Position p : StartPositions)
    {
        // set enemies first 
        Tile * next = new Tile(Ally::FOE,p.crew);
        setTile(next,p.col, p.row);
        tiles[index++] = next;
        
        // set own army on the opposite side of board
        next = new Tile(Ally::OWN, p.crew);
        setTile(next, 8 + 2 * p.row - p.col, 7 - p.row);
        tiles[index++] = next;
    }
}

void Engine::setView(BoardView* v)
{
    assigned_view = v;
    for(Tile * t: tiles)
    {
        const Node* p = t->getPosition();
        assigned_view->createPieces(t->getAlly(),p->getCol(),p->getRow(),t->getName());
    }
}

void Engine::getStepFromView(Node* n) const
{
    for(SDL_Point p : assigned_view->selected)
    {
        n->bind(board[p.x][p.y]);
    }
}

bool Engine::allowedMove(Node* N) const
{
    if(N->size() < 2 || N->start()->getAlly() != current_turn || N->last()->getAlly() == current_turn)
    {
        return false;
    }
    return isMarching(current_turn, N);
}

bool Engine::isMarching(const Ally& A, Node* N) const
{
    bool group = false;
    Piece line[3] = {};
    int index = 0;
    int attack = 0;
    const int def = N->last()->isEmpty() ? -1 : N->last()->getDef();
    for(Node* n = N->start(); n != N->last(); n = N->next())
    {
        if(n->getAlly() != A)
        {
        std::cout<<(A == Ally::OWN ? "OWN" : "FOE")<<' '<<(n->getAlly() == Ally::OWN ? "OWN" : "FOE")<<std::endl;
            return false;
        }
        line[index++] = n->getPiece();
        attack += archetype(n->getPiece(),Piece::WARRIOR);
    }
    switch(N->size())
    {
        case 4:
            group = archetype(line[0],line[1],line[2]);
            break;
        case 3:
            group = archetype(line[0],line[1]);
            break;
        case 2:
            group = true;
            break;
        default:
            return false;
    }
    return(group && attack > def);    
}

void Engine::doStep(const Node* step)
{
    move->clear();
    move->bind(EMPTY);
    move->append(step);
    move->bind(VALHALLA);
    for(Node * n = move->back(); n != EMPTY; n = move->curr())
    {
        move->prev()->moveTile(n);
    }
    swap();
}

bool Engine::compareToView() const
{
    std::cout<<"Start compare... " <<std::endl;
    for(int index=0;index<32;++index)
    {
        if(tiles[index]->getPosition() == VALHALLA)
        {
            continue;
        }
        const Node * p = tiles[index]->getPosition();
        if(index + 1 != assigned_view->getHandle(p->getCol(),p->getRow()))
        {
            std::cerr << "mismatch error: " << p->getCol() << ':' << p->getRow() << ' ' << (index+1) << " != " << assigned_view->getHandle(p->getCol(),p->getRow()) << std::endl;
            return false;
        }
    }
    std::cout<<"compare Ok. " <<std::endl;
    return true;
}

void Engine::loop()
{
    Node n(4);
    while(true)
    {
        assigned_view->select();
        n.clear();
        getStepFromView(&n);
        if(allowedMove(&n))
        {
            doStep(&n);
            assigned_view->moveSelection();
            compareToView();
        }
        assigned_view->selected.clear();
    };
}

Engine::~Engine()
{
    for(Tile * t: tiles)
    {
        delete t;
    }
    delete assigned_view;
}
