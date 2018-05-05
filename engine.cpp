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
,path(new Node)
,move(new Node(6))
,x_register(new Node)
,y_register(new Node)
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
    return available_nodes.getWrapper()->find(N) != available_nodes.getWrapper()->end();
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
        getSteps(current_turn);
        assigned_view->select();
        n.clear();
        getStepFromView(&n); 
        if(allowedMove(&n))
        {
            doStep(&n);
            assigned_view->moveSelection();
            //compareToView();
        }
        assigned_view->selected.clear();
    };
}

Engine::~Engine()
{
    delete path;
    delete x_register;
    delete y_register;
    for(Tile * t: tiles)
    {
        delete t;
    }
    delete assigned_view;
}
