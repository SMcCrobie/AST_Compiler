#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

//Declarations
vector<string>::iterator numbers();


//Var
struct var {
  string label;
  static int counter;
  static void reset() { counter = 0;}
  var(){};
  var (const char* label) {counter++; this->label = label;
  this->label = this->label + "_" + to_string(counter);}
  var (string& label) {counter++;this->label = label;}
  var (var* base) {counter++;this->label = base->label + "_" + to_string(counter);}
  string toString() {return this->label;}
  friend ostream& operator<<(ostream& os, var& temp){
    os << temp.toString();
    return os;
  }
  friend ostream& operator<<(ostream& os, var* temp){
    os << temp->toString();
    return os;
  }



};
int var::counter = 0; 


//R EXPRESSION STRUCT
struct RExp {
  //vinterface fucntions
  virtual string toString(){return "FAIL";};
  virtual int eval (vector<string>::iterator it, map <var*, int>* env){return -1;};
  virtual RExp* uniq(map<var*, var*>* rho){return NULL;};

  //output overloading
  friend ostream& operator<<(ostream& os, RExp& temp){
    os << temp.toString();
    return os;
  } 
  friend ostream& operator<<(ostream& os, RExp* temp){
    os << temp->toString();
    return os;
  }
};


//R EXPRESSION DECENDENTS
struct RInt : public RExp {
  int n;
  RInt (int n) {this->n = n;}
  string toString(){return to_string(this->n);}
  int eval (vector<string>::iterator it, map <var*, int>* env){return n;}
  RExp* uniq(map<var*, var*>* rho){return this;}
};

struct RRead: public RExp {
  RRead(){}
  string toString(){return "(read)";}
  int eval (vector<string>::iterator it, map <var*, int>* env){
    it++;
    return stoi(*it);
  }
  RExp* uniq(map<var*, var*>* rho){return this;}
};

struct RNeg: public RExp {
  RExp* exp;
  RNeg(RExp* exp) {this->exp = exp;}
  string toString(){return "(- " + this->exp->toString() + ")";}
  int eval(vector<string>::iterator it, map <var*, int>* env){
    return -1* exp->eval(it, env);
  }
  RExp* uniq(map<var*, var*>* rho){return new RNeg(exp->uniq(rho));}
};

struct RAdd: public RExp {
  RExp* lexp;
  RExp* rexp;
  RAdd(RExp* lexp, RExp* rexp){this->lexp = lexp;this->rexp = rexp;}
  string toString(){
    return "(+ " + this->lexp->toString() + " " + this->rexp->toString() + ")";}
  int eval(vector<string>::iterator it, map <var*, int>* env){
    return lexp->eval(it, env) + rexp->eval(it, env);
  }
  RExp* uniq(map<var*, var*>* rho){
    return new RAdd(lexp->uniq(rho), rexp->uniq(rho));
  }
};

struct RRef: public RExp {
  var* v;
  RRef (var* v) {this->v = v;}
  string toString() {return this->v->toString();}
  int eval(vector<string>::iterator it, map <var*, int>* env){
    return (*env)[v];//object address is the key
  }
  RExp* uniq(map<var*, var*>* rho){
    return new RRef((*rho)[v]);
  }
};

struct RLet: public RExp {
  var* v;
  RExp* ve;
  RExp* be;
  RLet (var* v, RExp* ve, RExp* be) {
    this->v = v; this->ve = ve; this->be = be;}
  string toString() {
    return "(let ([" + v->label + " " + ve->toString() + "]) " + be->toString() + " )";}
  int eval(vector<string>::iterator it, map <var*, int>* env){
    int vval = ve->eval(it, env);
    env->insert(pair<var*,int>(v, vval));
    return be->eval(it, env);
  }
  RExp* uniq(map<var*, var*>* rho){
    var* vp = new var(v);
    map<var*, var*>* temp = new map<var*, var*>;
    *temp = *rho;
    pair<map<var*,var*>::iterator,bool> ret;
    ret = rho->insert(pair<var*, var*>(v, vp));
    if (ret.second==false) {
      map<var*, var*>* rho1 = new map<var*, var*>;
      *rho1 = *rho;
      (*rho1)[v] = vp;
      return new RLet(vp, ve->uniq(temp), be->uniq(rho1));
    }
    else
      return new RLet(vp, ve->uniq(temp), be->uniq(rho));
  }
};


//C EXPRESSION STRCUT
struct CArg {
  //interface fucntions
  virtual string toString(){return "FAIL";};
  
  //output overloading
  friend ostream& operator<<(ostream& os, CArg& temp){
    os << temp.toString();
    return os;
  }
  friend ostream& operator<<(ostream& os, CArg* temp){
    os << temp->toString();
    return os;
  }
};







//More Declarations
void test ( int ans, RExp* e );



//Evaluatos and evalutor needs
int eval(RExp* e) {
  map<var*, int>* env = new map <var*, int>;  //using object addresses as key component
  return e->eval(numbers(), env);
}

RExp* uniq(RExp* e){
  map<var*, var*>* rho = new map<var*, var*>; //using object addresses as keys
  return e->uniq(rho);
} 

vector<string>::iterator numbers() {
  vector<string>* str_vec = new vector<string>;
  std::vector<std::string> temp;
  temp.push_back("22");
  temp.push_back("15");
  temp.push_back("0");
  temp.push_back("55");
  *str_vec = temp;
  vector<string>::iterator it = str_vec->begin();
  return it;
}



// Fast constructors
static RExp* I(int n) { return new RInt(n); }
static var*  V(const char* v) {return new var(v);}
static RExp* R(var* x) { return new RRef(x); }
static RExp* N(RExp* e) { return new RNeg(e); }
static RExp* A(RExp* l, RExp* r) { return new RAdd(l, r); }
static RExp* Let(var* x, RExp* xe, RExp* be) { return new RLet(x, xe, be); }








int main(){
  RInt* a = new RInt(10);
  RRead* b = new RRead();
  RNeg* c = new RNeg(b);
  RAdd* d = new RAdd(new RInt(15),new RRead());
  string str = "hello";
  // var* v = new var(str);
  //RRef f(v);
  RExp* alpha = A(I(8),I(10));//18
  RExp* beta = A(N(I(8)), I(10));//2
  var* x = V("x");
  var* y = V("y");
  RExp* charlie = Let(x, I(8), A(R(x), I(5)));
  RExp* delta = Let(x, I(8), A( Let(x, I(10), R(x)), R(x)));
  
  cout << a << " " << b << " " << c << " " << d <<endl;
  //  cout << " " << f<< endl;
  cout << new RLet(new var("xor"),
		   new RRead(),
		   new RAdd(new RInt(100),
			    new RNeg(new RRef(new var("pointer"))))) << endl;
  cout << (Let(V("x"), I(8), A(N(R(V("xor"))), I(10)))) << endl;

  
  
  //TESTING
  cout << "***************" << endl;
  test(10, a);
  test(11, a);
  cout << "***************" << endl;
  test(18, alpha);
  test(14, alpha);
  cout << "***************" << endl;
  test(2, beta);
  test(22, beta);
  cout << "***************" << endl;
  test(13, charlie);
  test(8, charlie);
  cout << "***************" << endl;
  test(18, delta);
  test(14, delta);
  return 0;
  
}


// Run all tests at once
void test ( int ans, RExp* e ) {
  var temp;
  temp.reset();
  cout << e << " =>";
  int act = eval(e);
  if(act == ans)
    cout << "success" << endl;
  else
    cout << "Fail: actual = " << act << ", desired =" << ans << endl;
  
  RExp* u = uniq(e);
  cout << u << " =>";
  act = eval(u);
  if(act == ans)
    cout << "success" << endl;
  else
    cout << "Fail: actual = " << act << ", desired =" << ans << endl;
  return;
}
