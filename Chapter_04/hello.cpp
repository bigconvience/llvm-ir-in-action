
#include <iostream>
 
using namespace std;
 
class Square
{
   public:
      double width;
      virtual double getArea(void) {
          return width * width;
      }
      void set( double dimen) {
         width = dimen;
      }  
};

class Rectangle:public Square {
   public:
      double length;
      double getArea(void) {
         return width * length;
      }
      void setLength(double len) {
         length = len;
      }
};


/**
 * https://www.geeksforgeeks.org/virtual-function-cpp
 * 
 * */
int main( )
{
   Square Square1;        // 声明 Square1，类型为 Square
   Square1.set(5.0);
 
   double volume = Square1.getArea();
   cout << "Square1 volumn " << volume <<endl;

   Rectangle rec;
   rec.set(3.0);
   rec.setLength(4.0);
   Rectangle *recPtr = &rec;
   double area = recPtr->getArea();
   cout << "rec area " << area <<endl;
   return 0;
}
