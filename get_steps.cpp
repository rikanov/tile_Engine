#include"engine.h"

void Engine::getSteps(const Ally& A)
{
    available_moves.clear();
    for(int index = A == Ally::OWN ; index < 32; index += 2)
    {
        if(tiles[index]->isActive() )
        {
            const Piece P = tiles[index]->getPiece();
            start_node = tiles[index]->getPosition();
            teleportingMoves(start_node);
            if(isAttacker(P))
            {
                marchingMoves(start_node);
            } // no else
            if(isHunter(P))
            {
                rangedAttacks();            
            } // no else here
            if(isTeleporter(P))
            {
                teleporterMoves();
            }
        }
    }
}

void Engine::teleporterMoves()
{
    Node * t = start_node->getTeleports();
    for(Node * n = t->start(); t->notEnded(); n = t->next())
    {
        if(n->empty())
        {
            available_moves.bind(start_node);
            available_moves.bind(n);
            available_moves.push();
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
            available_moves.bind(start_node);
            available_moves.bind(from->curr());
            available_moves.push();
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
        available_moves.append(path);
        available_moves.push();
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

void Engine::rangedAttacks()
{
    for(start_node->start(); start_node->notEnded(); start_node->next())
    {
        if(start_node->curr()->empty())
        {
            Node::swap(start_node,start_node->curr());
            getRangedTargets(start_node->curr());
            for(y_register->start(); y_register->notEnded(); y_register->next())
            {
                available_moves.bind(start_node);
                available_moves.bind(start_node->curr());
                available_moves.bind(y_register->curr());
                available_moves.push();
            }
            Node::swap(start_node,start_node->curr());
        }
    }
}

void Engine::getRangedTargets(Node* from)
{
    y_register->clear();
    z_register->clear();
    int ranged_synergy;
    bool expanded;
    for(Node* n = from->start(); from->notEnded(); n = from->next())
    {
        ranged_synergy = 0;
        expanded = false;
        if(n->empty() == false)
        {
            continue;
        }
        x_register->getConnectionsFrom(n);
        for(Node* ne = x_register->start(); x_register->notEnded(); ne = x_register->next())
        {
            if(ne->empty())
            {
                if(expanded)
                {
                    z_register->clear();
                    break;
                }
                else
                {
                    expanded = true;
                    for(Node* exp = ne->start();ne->notEnded();exp = ne->next())
                    {
                        if(exp->empty())
                        {
                            if(exp != n)
                            {
                                z_register->clear();
                                break;
                            }
                        }
                        else if(exp->getAlly() == current_turn)
                        {
                            ranged_synergy += isHunter(exp->getPiece());
                        }
                        else
                        {
                            z_register->bind(exp);
                        }
                    }
                }
            }
            else if(ne->getAlly() == current_turn)
            {
                ranged_synergy += isHunter(ne->getPiece());
            }
            else
            {
                z_register->bind(ne);
            }
        }
        for(z_register->start(); z_register->notEnded(); z_register->next())
        {
            if(z_register->curr()->getDef() < ranged_synergy)
            {
                y_register->bind(z_register->curr());
            }
        }
    }
}

