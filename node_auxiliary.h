#ifndef NODE_AUXILIARY__H
#define NODE_AUXILIARY__H

class NodeAuxiliary 
{
protected:
    int col = -1, row = -1;
    bool initialized = false;
    int * depth;
    int * depth_pointer;
    
public:
    
    int value = 0;
    double ai_score = 0.0;
    
    NodeAuxiliary(): depth(new int[10])
    {
        depth_pointer = depth;
        *depth_pointer = 0;
    }
    virtual ~NodeAuxiliary()
    {
        delete[] depth;
    }
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
    void setSearchLevel(const int& n)
    {
        *(++depth_pointer) = n;
    }
    void backSearchLevel()
    {
        --depth_pointer;
    }
    int getSearchLevel() const
    {
        return *depth_pointer;
    }
};
#endif
