#include"engine.h"

void Engine::getSteps(const Ally& A)
{
    next_move = available_moves;
    next_move->clear();
    for(int index = A == Ally::OWN ; index < 32; index += 2)
    {
        if(tiles[index]->getPosition() == VALHALLA)
        {
            continue;
        }
        const Piece P = tiles[index]->getPiece();
        Node* N = tiles[index]->getPosition();
        teleporting(N);
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

void Engine::teleporting(Node* start)
{
    Node * S = start->getRouter();
    for(S->start(); S->curr() != S->end(); S->next())
    {
        checkTeleport(start, S->curr(),3);
    }
}

void Engine::teleporting(Node* start, Node* teleporter, const int& step)
{
    for(teleporter->start(); teleporter->curr() != teleporter->end(); teleporter->next())
    {
        checkTeleport(start, teleporter->curr(),step);
        
    }
}

void Engine::checkTeleport(Node* n1, Node* n2, const int& step)
{
    if(n2->isEmpty())
    {
        next_move->bind(n1);
        next_move->bind(n2);
        (++next_move)->clear();
    }
    else if(isTeleporter(n2->getPiece(n1->getAlly())) && step > 0)
    {
        teleporting(n1,n2, step-1);
    }
}
