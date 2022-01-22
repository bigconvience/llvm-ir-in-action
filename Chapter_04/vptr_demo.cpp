
#include <iostream>
 
using namespace std;
 
class Square
{
   public:
      double width;
      virtual double getArea() {
          return width * width;
      }

      void set( double dimen) {
         width = dimen;
      }  
};

class Rectangle:public Square {
   public:
      double length;
      double getArea() {
         return width * length;
      }

      void setLength(double len) {
         length = len;
      }
};

double increaseArea(Square *ps) {
   return ps->getArea() + 1;
}


int main( )
{
   Rectangle rec;
   Rectangle *recPtr = &rec;
   recPtr->set(3.0);
   recPtr->setLength(4.0);
   double area = recPtr->getArea();
   cout << "rec area " << area <<endl;
   return 0;
}
