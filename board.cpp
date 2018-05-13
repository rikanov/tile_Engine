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

#include<iostream>
#include "board.h"

static const double COST_DISTANCE = 0.05;

Board::Board()
 :NONE(new Tile(Ally::NONE,Piece::NONE))
{
    std::cout<<"board constructor"<<std::endl;
    for(int col = 0; col<15; ++col)
    {
        for(int row = 0; row<8; ++row)
        {
            Node * next = new Node;
            next->setTile(NONE);
            next->setPosition(col,row);
            board[col][row] = next;
        }
    }
    init();
}

void Board::init()
{
    std::cout<<"board init"<<std::endl;
    int row = 0;
    // make connections between the nodes
    for(int wide = 8; row < 4; ++row, wide += 2)
    {
        for(int col = 0; col < wide; ++col)
        {
            Node::connect(board[col][row],board[col+1][row]);
            if( row > 0 && col % 2 == 1) 
            {
                Node::connect(board[col][row],board[col-1][row-1]);
            }
        }
    }
    for(int col = 0; col < 15; ++col)
    {
        if(col % 2 ==0)
        {
            Node::connect(board[col][row],board[col][row-1]);
        }
    }
    for(int wide = 14; row < 8; ++row, wide -= 2)
    {
        for(int col = 0; col < wide; ++col)
        {
            Node::connect(board[col][row],board[col+1][row]);
            if( row > 4 && col % 2 == 0)
            {
                Node::connect(board[col][row],board[col+1][row-1]);
            }
        }
        if(row > 4)
        {
            Node::connect(board[wide][row],board[wide+1][row-1]);
        }
    } 
    // store 2-steps distant nodes 
    for(int col=0; col<15; ++col)
    {
        for(int row=0; row<8; ++row)
        {
            board[col][row]->initTeleports();
            board[col][row]->initEnvironment();
        }
    }
    // distance from the centre
    Node BFS(96);
    for(int col=6;col<9;++col)
    {
        for(int row=3;row<5;++row)
        {
            board[col][row]->ai_score=1.0;
            BFS.bind(board[col][row]);
        }
    }
    for(Node * n = BFS.start(); BFS.notEnded(); n = BFS.next())
    {
        for(Node * ne = n->start(); n->notEnded(); ne = n->next())
        {
            if(ne->ai_score == 0.0)
            {
                ne->ai_score = n->ai_score - COST_DISTANCE;
                BFS.bind(ne);
            }
        }
    }
}

void Board::setTile(Tile* t, int col, int row)
{
    board[col][row]->setTile(t);
}

Board::~Board()
{
    delete NONE;
    for(int col = 0; col<15; ++col)
    {
        for(int row = 0; row<8; ++row)
        {
            delete board[col][row];
            board[col][row] = nullptr;
        }
    }
}

