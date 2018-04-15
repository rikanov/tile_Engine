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

Board::Board()
 :NONE(new Tile)
 ,VALHALLA(new Node)
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
    VALHALLA->setTile(NONE);
}

void Board::init()
{
    std::cout<<"board init"<<std::endl;
    int row = 0;
    Node * n1 = new Node;
    Node * n2 = new Node;
    Node::connect(n1,n2);
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
        {std::cout<<"core "<<col<<' '<<row<<std::endl;
            Node::connect(board[col][row],board[col+1][row]);std::cout<<"done "<<row<<std::endl;
            if( row > 4 && col % 2 == 0)
            {
                Node::connect(board[col][row],board[col+1][row-1]);
            }
        }
    }
    std::cout<<"number of rows: "<<row<<std::endl;
}

void Board::setTile(Tile* t, int col, int row)
{
    board[col][row]->setTile(t);
}

Board::~Board()
{
    delete NONE;
    delete VALHALLA;
    for(int col = 0; col<15; ++col)
    {
        for(int row = 0; row<7; ++row)
        {
            delete board[col][row];
        }
    }
}

