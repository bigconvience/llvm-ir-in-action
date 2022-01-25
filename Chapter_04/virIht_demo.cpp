class BaseA
{
public:
    int a;

};

class BaseB: virtual BaseA
{
public:
    int b;
    int sumB() {
      return a + b;
    }
};

class BaseC: virtual BaseA
{
public:
    int c;
    int sumC() {
      return a + c;
    }
};

class Derived:
    public BaseB,
    public BaseC
{
public:
   int d;
   int sumDer() {
    return sumB() + sumC() + d;
   } 
};

int exeSum() {
    Derived derived;
    derived.d = 10;
   return derived.sumDer();
}

