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
        teleportingMoves(start_node);
        if(isAttacker(P))
        {
            marchingMoves(start_node);
        } // no else
        if(isHunter(P))
        {
            
        } // no else
        if(isTeleporter(P))
        {
            
        }
    }
}
void Engine::teleportingMoves(Node* from)
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
            teleportingMoves(from->curr());
            path->pop();
        }
    }
}

void Engine::marchingMoves(Node* from)
{
    const Node * last = path->last();
    path->bind(from);
    if(isMarching())
    {
        for(from->start(); from->notEnded(); from->next())
        {
            if(from->curr() != last)
            {
                marchingMoves(from->curr());
            }
        }
    }
    else if(
           (from->getAlly() != start_node->getAlly() && from->getDef() < path->size()-1)
        && (path->size() >2 || from->getAlly() != Ally::NONE) // avoid basic steps redundancy
           )
    {
        available_nodes.append(path);
        available_nodes.push();
    }
    path->pop();
}

bool Engine::isMarching() const
{
    switch(path->size())
    {
        case 1:
            return true;
        case 2:
            return isMarchingGroup(path->at(0)->getPiece(current_turn),path->at(1)->getPiece(current_turn));
        case 3:
            return isMarchingGroup(path->at(0)->getPiece(current_turn),path->at(1)->getPiece(current_turn),path->at(2)->getPiece(current_turn));
        default:
            return false;
    }
}
