//Main compiler page
//created by Sean McCrobie

#include <iostream>
#include <string>

using namespace std;

/*Data Types for R0
exp = num | (- exp) | (+ exp exp) | var | (let ([var exp]) exp)
type idetifiers in INT form
(1) = num
(2) = negate
(3) = ADD
*/
struct exp {
  int n;
  int type;
  friend ostream& operator<<(ostream& os, const exp& temp) {
    os << temp.n;
    return os;
  }
};

struct neg;
struct add;

struct num: public exp {
  num(int temp) {type = 1;n = temp;} 
  void operator=(const exp& temp) {this->n = temp.n;}
  void operator=(const int& temp) {this->n = temp;}
};

struct neg: public exp {
  neg(exp& x){
    switch(x.type) {
    case 1 :
      this->type = 2;
      this->n = -(x.n);
      break;
    case 2:
      
  
};

struct add: public exp {
  add(exp& x, exp& y){n = x.n+y.n;}
};

int main () {
  num x(44);
  num y(30);
  neg b(x);
  cout << b << endl;
  x = add(num(10),y);
  cout << x << endl;
  cout << y << endl;
  return 0;
}
