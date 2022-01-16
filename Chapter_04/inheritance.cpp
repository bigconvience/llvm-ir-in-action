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

class Square: public Box
{
public:
    void SetB(int value)
    {
        SetA(value);
        _b = value;
    }

protected:
    int _b;
};

int main( )
{

    Square square;
    square.SetB(3);
    return 0;
}