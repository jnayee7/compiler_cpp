#ifndef VAL_H
# define VAL_H
#include<sstream>
#include <string>
#include <iostream>
using namespace std;

class Val {
   int i;
   string s;
   enum ValType {
      ISINT,
      ISSTR,
      ISERR
   }
   vt;

   public:
   Val(): i(0), vt(ISERR) {}
   Val(int i): i(i), vt(ISINT) {}
   Val(string s): i(0), s(s), vt(ISSTR) {}
   Val(ValType vt, string errmsg): i(0), s(errmsg), vt(ISERR) {}

   ValType getVt() const {
      return vt;
   }

   bool isErr() const {
      return vt == ISERR;
   }
   bool isInt() const {
      return vt == ISINT;
   }
   bool isStr() const {
      return vt == ISSTR;
   }

   int ValInt() const {
      if (isInt()) return i;
      throw "This Val is not an Int";
   }
   string ValString() const {
      if (isStr()) return s;
      throw "This Val is not a Str";
   }

   friend ostream & operator << (ostream & out,
      const Val & v) {

      if (v.isErr())
         out << "RUNTIME ERROR: " << v.s;

      if (v.isStr())
         out << v.s;

      else if (v.isInt())
         out << v.i;

      return out;
   }

   string GetErrMsg() const {
      if (isErr()) return s;
      throw "This Val is not an Error";
   }

   Val operator + (const Val & op) const {
      if (isInt() && op.isInt())
         return ValInt() + op.ValInt();
      if (isStr() && op.isStr())
         return ValString() + op.ValString();
      return Val(ISERR, "Type mismatch on operands of +");
   }

   Val operator - (const Val & op) const {
      if (isInt() && op.isInt())
         return ValInt() - op.ValInt();
      return Val(ISERR, "Type mismatch on operands of -");
   }

   Val operator * (const Val & op) const {
      if (isInt() && op.isInt())
         return ValInt() * op.ValInt();

      else if (isStr() && op.isInt()) {
         if (op.ValInt() >= 0) {
            string temp = "";

            for (int i = 0; i < op.ValInt(); i++) {
               temp += ValString();
            }
            return temp;
         } else {
            return Val(ISERR, "Cannot multiply a string by a negative integer");

         }

      } else if (isInt() && op.isStr()) {
         if (ValInt() >= 0) {
            string temp = "";

            for (int i = 0; i < ValInt(); i++) {
               temp += op.ValString();
            }
            return temp;
         } else {
            return Val(ISERR, "Cannot multiply a string by a negative integer");

         }
      }
      return Val(ISERR, "Type mismatch on operands of *");
   }

   Val operator / (const Val & op) const {
      if (isInt() && op.isInt()) {
         if (op.isInt() == 0) {
            return Val(ISERR, "Cannot divide by 0");

         } else {
            return ValInt() / op.ValInt();
         }
      }

      return Val(ISERR, "Type mismatch on operands of /");
   }

   Val operator!() const {
      int val;
      if (isInt()) {
         if (ValInt() < 0) {
            val = ValInt() * -1;
            int last_value = 0;
            int reverse = 0;

            while (val > 0) {
               last_value = val % 10;
               reverse = reverse * 10 + last_value;

               val /= 10;
            }

            return reverse * -1;
         } else {
            val = ValInt();
            int last_value = 0;
            int reverse = 0;

            while (val > 0) {
               last_value = val % 10;
               reverse = reverse * 10 + last_value;

               val /= 10;
            }

            return reverse;
         }

      }
      return Val(ISERR, "Type mismatch on operands of !");
   }

};

#endif