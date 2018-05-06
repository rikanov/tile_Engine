#include "node.h"
#include <iostream>

static const int STEPS_NUMBER_UPPER_LIMIT = 320;

class PreAllocatedNodes
{
    Node stored_data[STEPS_NUMBER_UPPER_LIMIT];
    Node * wrapper;
    Node * next_step;
public:
    PreAllocatedNodes()
     :wrapper(new Node(STEPS_NUMBER_UPPER_LIMIT))
     , next_step(&stored_data[0]) 
    {
        next_step->clear();
    }
    ~PreAllocatedNodes()
    {
        delete wrapper;
    }
    operator bool() const
    {
        return !(wrapper->single());
    }
    void clear()
    {
        wrapper->clear();
        next_step = stored_data;
        next_step->clear();
    }
    void push()
    {
        wrapper->bind(next_step++);
        next_step->clear();
    }
    void pop()
    {
        wrapper->pop();
    }
    void bind(Node * n)
    {
        next_step->bind(n);
    }
    void append(Node * n, const Node::AppendDirection& d = Node::NORMAL)
    {
        next_step->append(n,d);
    }
    Node* curr() const
    {
        return next_step;
    }
    Node* last() const
    {
        return wrapper->last();
    }
    void value(const int& v)
    {
        next_step->value = v;
    }
    Node * getWrapper() const
    {
        return wrapper;
    }
    void init(Tile* t)
    {
        for(Node n : stored_data)
        {
            n.setTile(t);
        }
    }
};
