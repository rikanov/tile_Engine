#ifndef NODE_AUXILIARY__H
#define NODE_AUXILIARY__H

class NodeAuxiliary 
{
protected:
    int col, row;
    bool initialized = false;
    
public:
    
    int value = 0;
    
    NodeAuxiliary(){}
    void setPosition(const int& c, const int& r)
    {
        if(!initialized)
        {
            col = c;
            row = r;
            initialized = true;
        }
    }
    const int& getCol() const
    {
        return col;
    }
    const int& getRow() const
    {
        return row;
    }
};
#endif
