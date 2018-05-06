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

#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"
#include "preallocated_nodes.h"
#include "../View2D/view2d.h"

class Engine: public Board
{
    Ally current_turn;
    BoardView * assigned_view;
    void getStepFromView(Node*) const;
    void setViewFromStep(Node*) const;
    bool allowedMove(Node*) const;
    bool isMarching() const;
    bool compareToView() const;
    void swap()
    {
        current_turn = current_turn == Ally::OWN ? Ally::FOE : Ally::OWN;
    }
    struct Position 
    {
        Piece crew;
        const int col;
        const int row;
    };
    static const Position StartPositions[];
    
    PreAllocatedNodes step_history;
    PreAllocatedNodes available_moves;
    
    Node * start_node;
    Node * path;
    Node * move;
    Node * x_register; // for temporarily operations 
    Node * y_register; // for temporarily operations
    Node * z_register; // for temporarily operations
    Tile * tiles[32] = {};
   
    void teleportingMoves(Node *);
    void marchingMoves(Node *);
    void rangedAttacks();
    void getRangedTargets(Node *); // collect preys to y_register node
    
public:
    Engine(const Ally& A, BoardView* B);
    ~Engine();
    void start();
    void setView(BoardView * v);
    void getSteps(const Ally&);
    void doStep(Node* step);
    void undoStep();
    void loop();
};

#endif // ENGINE_H
