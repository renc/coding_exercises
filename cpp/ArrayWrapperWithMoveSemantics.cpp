/* 

https://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html
*/ 

#include <iostream>
#include <utility>

class MetaData
{
public:
    MetaData (int size, const std::string& name)
        : _name( name )
        , _size( size )
    {
        std::cout << "MetaData ctor.\n";
    }
 
    // copy constructor
    MetaData (const MetaData& other)
        : _name( other._name )
        , _size( other._size )
    {
        std::cout << "MetaData copy ctor.\n";
    }
 
    // move constructor
    MetaData (MetaData&& other)
        : _name( std::move(other._name) )
        , _size( other._size )
    {
        std::cout << "MetaData move ctor.\n";
    }
    MetaData & operator = (MetaData && other)
    {
        _name = std::move(other._name);
        _size = other._size;
        std::cout << "MetaData move assig op.\n";
    }
    std::string getName () const { return _name; }
    int getSize () const { return _size; }
private:
    std::string _name;
    int _size;
};

class ArrayWrapper
{
public:
    // default constructor produces a moderately sized array
    ArrayWrapper ()
        : _p_vals( new int[ 64 ] )
        , _metadata( 64, "ArrayWrapper" )
    {}
 
    ArrayWrapper (int n)
        : _p_vals( new int[ n ] )
        , _metadata( n, "ArrayWrapper" )
    {}
 
    // move constructor
    ArrayWrapper (ArrayWrapper&& other)
        : _p_vals( other._p_vals  )
        , _metadata( std::move(other._metadata) )
    {
        other._p_vals = NULL;
        std::cout << "ArrayWrapper move ctor.\n";
    }
    ArrayWrapper& operator =(ArrayWrapper && other)
    {
        _p_vals = other._p_vals;
        _metadata = std::move(other._metadata); 
        // this fail, if MetaData does not define move assignment operator
        std::cout << "ArrayWrapper move assig op.\n";
    }
    // copy constructor
    ArrayWrapper (const ArrayWrapper& other)
        : _p_vals( new int[ other._metadata.getSize() ] )
        , _metadata( other._metadata )
    {
        for ( int i = 0; i < _metadata.getSize(); ++i )
        {
            _p_vals[ i ] = other._p_vals[ i ];
        }
        std::cout << "ArrayWrapper copy ctor.\n";
    }
    ~ArrayWrapper ()
    {
        delete [] _p_vals;
    }
private:
    int *_p_vals;
    MetaData _metadata;
};

ArrayWrapper getArrayWrapper()
{
    ArrayWrapper awTemp;
    return awTemp;
}
void dosth(const ArrayWrapper &aw)
{
    std::cout << "dosth(const ArrayWrapper &aw) called.\n";
}
void dosth(ArrayWrapper &&aw)
{
    std::cout << "dosth(ArrayWrapper &&aw) called.\n";
}
int main()
{
    std::cout << "--1:\n";
    ArrayWrapper &&aw1(getArrayWrapper());
    std::cout << "--2:\n";
    ArrayWrapper &&aw2 = getArrayWrapper();
    std::cout << "to dosth:\n";
    dosth(aw1);
    dosth(aw2);
    return 0;
}