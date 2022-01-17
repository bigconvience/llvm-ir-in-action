class Box
{
public:
    void SetA(double value)
    {
        _a = value;
    }

private:
    double _a;
};

class Square
{
public:
    void SetB(double value) {
        _b = value;
    }

private:
    double _b;
};

class Cube:public Box, public Square {
public:
    void SetC(double value) {
        SetA(value);
        SetB(value);
        _c = value;
    }
private:
    int _c;
};

int main( )
{

    Cube cube;
    cube.SetC(3);
    return 0;
}