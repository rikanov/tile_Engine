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

const int Engine::DEPTH_BOUNDARY = 20;

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
,z_register(new Node)
,assigned_view(nullptr)
,available_steps(new PreAllocatedNodes[DEPTH_BOUNDARY])
{
    start();
    setUI(B);
}

void Engine::start()
{
    int index = 0, member = 0;
    for(int i=0; i<DEPTH_BOUNDARY; ++i)
    {
        available_steps[i].init(NONE);
    }
    for(Position p : StartPositions)
    {
        // set enemies first 
        Tile * next = new Tile(Ally::FOE,p.crew);
        setTile(next,p.col, p.row);
        tiles[index++] = crew[0][member] = next;
        
        // set own army on the opposite side of board
        next = new Tile(Ally::OWN, p.crew);
        setTile(next, 8 + 2 * p.row - p.col, 7 - p.row);
        tiles[index++] = crew[1][member++] = next;
    }
}

void Engine::setUI(BoardView* v)
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

void Engine::setViewFromStep(Node* n) const
{
    assigned_view->selected.clear();
    for(n->start(); n->notEnded(); n->next())
    {
        SDL_Point next;
        next.x = n->curr()->getCol();
        next.y = n->curr()->getRow();
        assigned_view->selected.push_back(next);
    }
    assigned_view->moveSelection(n->value);
}

bool Engine::allowedMove(Node* N) const
{
    return available_steps[current_search_level].getWrapper()->find(N) != available_steps[current_search_level].getWrapper()->end();
}

void Engine::doStep(Node* step)
{
   // makeView(step);
    move->clear();
    move->bind(step);
    move->append(step);
    move->bind(step_history.curr());
    for(Node * n = move->back(); n != step; n = move->curr())
    {
        move->prev()->moveTile(n);
    }
    step_history.curr()->getTile()->deactivate();
    step_history.append(step,Node::REVERT);
    step_history.push();
    swap(); 
}

void Engine::undoStep()
{
    Node* step = step_history.last();
    step->getTile()->activate();
    move->clear();
    move->bind(step);
    move->append(step);
    for(Node * n = move->back(); n != step; n = move->curr())
    {
        move->prev()->moveTile(n);
    }
    step_history.pop();
    swap();
}

bool Engine::compareToView() const
{
    for(int index=0;index<32;++index)
    {
        if(tiles[index]->isActive() == false)
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
    n.setTile(NONE);
    while(true)
    {
        n.clear();
        getSteps(current_turn);
        assigned_view->select();
        if(assigned_view->exit_request)
        {
            break;
        }
        if(assigned_view->undo_request && step_history)
        { 
            setViewFromStep(step_history.last());
            undoStep();
            continue;
        } 
        getStepFromView(&n); 
        if(allowedMove(&n))
        {
            step_history.value(assigned_view->moveSelection());
            doStep(&n);
            //compareToView();
        }
        assigned_view->selected.clear();
    };
}

double Engine::evaluate() const
{
    double result[] = {0.0,0.0};
    for(int ally = 0; ally<2; ++ally)
    for(int index = 0; index<16; ++index)
    {
        Tile * next = crew[ally][index];
        if(next->isActive())
        {
            result[ally] += next->getValue();
        }
    }
    return current_turn == Ally::OWN ? result[1] / result[0] : result[0] / result[1];
}

void Engine::makeView(Node* step)
{
    const int inc = step->start()->value+1;
    for(; step->notEnded(); step->next())
    {
        step->curr()->makeView(inc);
    }
}

void Engine::closeView(Node* step, const int& level)
{
    for(step->start(); step->notEnded(); step->next())
    {
        step->curr()->closeView(level);
    }
}

Engine::~Engine()
{
    std::cout << "Stop engine... " << std::endl;
    delete[] available_steps;
    delete path;       std::cout << "Path deleted... " << std::endl;
    delete move;       std::cout << "Move deleted... " << std::endl;
    delete x_register; std::cout << "x_register deleted... " << std::endl;
    delete y_register; std::cout << "y_register deleted... " << std::endl;
    delete z_register; std::cout << "z_register deleted... " << std::endl;
    
    std::cout<<"Delete tiles ... "<<std::endl;
    for(Tile * t: tiles)
    {
        delete t;
    }
    std::cout<<"Done. "<<std::endl;
    delete assigned_view;
    std::cout <<"Engine has been ended."<<std::endl;
}
