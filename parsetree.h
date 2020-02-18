/*
 * parsetree.h
 */
#ifndef PARSETREE_H_
#define PARSETREE_H_
#include <vector>
#include <map>
#include <string>
#include "parse.h"
#include "val.h"

using std::vector;
using std::map;
using std::string;

// NodeType represents all possible types
enum NodeType {
   ERRTYPE,
   INTTYPE,
   STRTYPE
};

// a "forward declaration" for a class to hold values
class Val;
extern map < string, Val > symbol;
extern void RunTimeError(string);

class ParseTree {

   public:
      int linenum;
   ParseTree * left;
   ParseTree * right;

   ParseTree(int linenum, ParseTree * l = 0, ParseTree * r = 0): linenum(linenum), left(l), right(r) {}

   virtual~ParseTree() {
      delete left;
      delete right;
   }

   int GetLineNumber() const {
      return linenum;
   }

   int Traverse(int input, int(ParseTree:: * func)(void)) {
      if (left) input += left -> Traverse(input, func);
      if (right) input += right -> Traverse(input, func);
      return Visit(input, func);
   }

   virtual int Visit(int input, int(ParseTree:: * func)(void)) {
      return input;
   }
   virtual bool IsIdent() const {
      return false;
   }
   virtual bool IsVar() const {
      return false;
   }
   virtual string GetId() const {
      return "";
   }
   virtual int IsBang() const {
      return 0;
   }
   virtual bool IsLet() const {
      return false;
   }
   virtual Val EVAL(map < string, Val > & symbol) = 0;

   int CheckLetBeforeUse(map < string, bool > &
      var) {
      int cnt = 0;
      if (left) cnt += left -> CheckLetBeforeUse(var);
      if (right) cnt += right -> CheckLetBeforeUse(var);
      if (IsLet())
         var [this -> GetId()] = true;

      return cnt;

   }

};

class StmtList: public ParseTree {

   public: StmtList(ParseTree * l, ParseTree * r): ParseTree(0, l, r) {}

   virtual Val EVAL(map < string, Val > & symbol) {
      left -> EVAL(symbol);
      if (right) {
         right -> EVAL(symbol);
      }
      return Val();
   }

};

class Let: public ParseTree {
      string id;

      public:
         Let(Lex & t, ParseTree * e): ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}

      string GetId() const {
         return id;
      }
	bool IsLet() const { return true; }
    
     virtual Val EVAL(map <string, Val> &symbol)
    {
               Val result = left->EVAL(symbol);

           if(symbol.count(id) && !symbol.empty())
           {
               
             symbol[id] = result;
           }
           else
           {             
             symbol.insert (std::pair<string, Val>(id, result));
           }
       
        return Val();
  
    } 
};

class Print : public ParseTree {
public:
	Print(int line, ParseTree *e) : ParseTree(line, e) {}
    
    virtual Val EVAL(map <string, Val> &symbol)
    {
      cout << left->EVAL(symbol);
      return Val();
    }
};

class Loop : public ParseTree {

public:
   Loop(int line, ParseTree *e, ParseTree *s): ParseTree(line, e, s) {}

virtual Val EVAL(map < string, Val > & symbol) {
   Val results = right -> EVAL(symbol);
   right -> EVAL(symbol);
   right -> EVAL(symbol);
   right -> EVAL(symbol);

   return Val();
}
};

class If: public ParseTree {

   public: If(int line, ParseTree * e, ParseTree * s): ParseTree(line, e, s) {}

   virtual Val EVAL(map < string, Val > & symbol) {
      Val results = left -> EVAL(symbol);
      if (results.ValInt() == 1) {
         if (results.ValInt() == 1) {
            return right -> EVAL(symbol);
         }
      }

      return Val();
   }
};

class PlusExpr: public ParseTree {
   public: PlusExpr(int line, ParseTree *l, ParseTree *r): ParseTree(line, l, r) {}
   virtual Val EVAL(map < string, Val > & symbol) {
      return left -> EVAL(symbol) + right -> EVAL(symbol);
   }
};

class MinusExpr: public ParseTree {
   public: MinusExpr(int line, ParseTree *l, ParseTree *r): ParseTree(line, l, r) {}

   virtual Val EVAL(map < string, Val > & symbol) {
      return left -> EVAL(symbol)-right -> EVAL(symbol);
   }

};

class TimesExpr: public ParseTree {
   public: TimesExpr(int line, ParseTree * l, ParseTree * r): ParseTree(line, l, r) {}

   virtual Val EVAL(map < string, Val > & symbol) {
      return left -> EVAL(symbol) * right -> EVAL(symbol);
   }
};

class DivideExpr: public ParseTree {
   public: DivideExpr(int line, ParseTree *l, ParseTree *r): ParseTree(line, l, r) {}

   virtual Val EVAL(map < string, Val > & symbol) {
      return left -> EVAL(symbol) / right -> EVAL(symbol);
   }
};

class BangExpr: public ParseTree {
   public: BangExpr(int line, ParseTree *l): ParseTree(line, l) {}
   virtual int IsBang() const {
      return 1;
   }

   virtual Val EVAL(map < string, Val > & symbol) {
      return !left -> EVAL(symbol);
   }
};

class IConst: public ParseTree {
   int val;

   public:
      IConst(Lex & t): ParseTree(t.GetLinenum()) {
         val = stoi(t.GetLexeme());
      }
   virtual Val EVAL(map < string, Val > & symbol) {
      return Val(val);
   }
};

class SConst: public ParseTree {
   string val;

   public:
      SConst(Lex & t): ParseTree(t.GetLinenum()) {
         val = t.GetLexeme();
      }

   virtual Val EVAL(map < string, Val > & symbol) {
      return Val(val);
   }
};

class Ident: public ParseTree {
   string id;

   public:
      Ident(Lex & t): ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

   bool IsIdent() const {
      return true;
   }
   string GetId() const {
      return id;
   }

   virtual Val EVAL(map < string, Val > & symbol) {
    if (!symbol.empty() && symbol.count(id)) {
         return symbol[id];
      } else {
         RunTimeError("Symbol " + id + " not defined");
         return Val();
      } 
   }
};
#endif 
/* PARSETREE_H_ */
