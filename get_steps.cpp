#include"engine.h"

void Engine::getSteps(const Ally& A)
{
    available_nodes.clear();
    for(int index = A == Ally::OWN ; index < 32; index += 2)
    {
        if(tiles[index]->getPosition() == VALHALLA)
        {
            continue;
        }
        const Piece P = tiles[index]->getPiece();
        start_node = tiles[index]->getPosition();
        teleporting(start_node);
        if(isAttacker(P))
        {
            
        } // no else
        if(isHunter(P))
        {
            
        } // no else
        if(isTeleporter(P))
        {
            
        }
    }
}
void Engine::teleporting(Node* from)
{
    for(from->start(); from->notEnded(); from->next())
    {
        if(from->curr() == path->last())
        {
            continue;
        }
        if(from->curr()->getPiece() == Piece::NONE)
        {
            available_nodes.bind(start_node);
            available_nodes.bind(from->curr());
            available_nodes.push();
        }
        else if(isTeleporter(from->curr()->getPiece(current_turn)) && path->size() < 3)
        {
            path->bind(from);
            teleporting(from->curr());
            path->pop();
        }
    }
}
