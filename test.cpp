#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>
#endif /* __PROGTEST__ */

bool isValidNumber(const std::string &input){
  if (input.empty()) return false;

  for (char c : input) {
    if (!std::isdigit(c) && c != '-') {
        return false;
    }
  }

 return true;
}

std::string removeLeadingZeroes(const std::string &input) {
    size_t nonZeroIndex = input.find_first_not_of('0'); // Find index of first non-zero digit
    if (nonZeroIndex == std::string::npos) {
        return "0"; // If input contains only zeroes, return "0"
    } else {
        return input.substr(nonZeroIndex); // Extract substring starting from first non-zero digit
    }
}

std::string returnValidNumber(const std::string & input){
  if(input == ""){
    throw std::invalid_argument("Invalid input: " + input);
  }
  std::string ret;
  for (char c : input) {
        if (std::isdigit(c) || c == '-'){
            ret += c;
        }
        else{
          return ret;
        }
    }
  return ret;
}

std::string reverseString(const std::string& str) {
    std::string reversed = str; 
    std::reverse(reversed.begin(), reversed.end());
    return reversed;
}

std::string add(std::string & a, std::string & b){
  if(a.size() < b.size()){
    std::swap(a,b);
  }
  int j = a.size()-1;
    for(int i=b.size()-1; i>=0; i--, j--)
        a[j]+=(b[i]-'0');

    for(int i=a.size()-1; i>0; i--)
    {
        if(a[i] > '9')
        {
            int d = a[i]-'0';
            a[i-1] = ((a[i-1]-'0') + d/10) + '0';
            a[i] = (d%10)+'0';
        }
    }
    if(a[0] > '9')
    {
        std::string k;
        k+=a[0];
        a[0] = ((a[0]-'0')%10)+'0';
        k[0] = ((k[0]-'0')/10)+'0';
        a = k+a;
    }
    return a;
}

bool isSmaller(std::string str1, std::string str2) 
{ 
    // Calculate lengths of both string 
    int n1 = str1.length(), n2 = str2.length(); 
  
    if (n1 < n2) 
        return true; 
    if (n2 < n1) 
        return false; 
  
    for (int i = 0; i < n1; i++) 
        if (str1[i] < str2[i]) 
            return true; 
        else if (str1[i] > str2[i]) 
            return false; 
  
    return false; 
} 

std::string findDiff(std::string str1, std::string str2) 
{ 
    // Before proceeding further, make sure str1 
    // is not smaller 
    if (isSmaller(str1, str2)) 
        swap(str1, str2); 
  
    // Take an empty std::string for storing result 
    std::string str = ""; 
  
    // Calculate length of both std::string 
    int n1 = str1.length(), n2 = str2.length(); 
  
    // Reverse both of strings 
    reverse(str1.begin(), str1.end()); 
    reverse(str2.begin(), str2.end()); 
  
    int carry = 0; 
  
    // Run loop till small std::string length 
    // and subtract digit of str1 to str2 
    for (int i = 0; i < n2; i++) { 
        // Do school mathematics, compute difference of 
        // current digits 
  
        int sub 
            = ((str1[i] - '0') - (str2[i] - '0') - carry); 
  
        // If subtraction is less than zero 
        // we add then we add 10 into sub and 
        // take carry as 1 for calculating next step 
        if (sub < 0) { 
            sub = sub + 10; 
            carry = 1; 
        } 
        else
            carry = 0; 
  
        str.push_back(sub + '0'); 
    } 
  
    // subtract remaining digits of larger number 
    for (int i = n2; i < n1; i++) { 
        int sub = ((str1[i] - '0') - carry); 
  
        // if the sub value is -ve, then make it positive 
        if (sub < 0) { 
            sub = sub + 10; 
            carry = 1; 
        } 
        else
            carry = 0; 
  
        str.push_back(sub + '0'); 
    } 
  
    // reverse resultant std::string 
    reverse(str.begin(), str.end()); 
  
    return str; 
} 

std::string multiplyTwoNumbers(std::string num1, std::string num2) {
   if (num1 == "0" || num2 == "0") {
      return "0";
   }
   std::string product(num1.size() + num2.size(), 0);
   for (int i = int(num1.size()) - 1; i >= 0; i--) {
      for (int j = int(num2.size()) - 1; j >= 0; j--) {
            int n = (num1[i] - '0') * (num2[j] - '0') + product[i + j + 1];
            product[i + j + 1] = n % 10;
            product[i + j] += n / 10;
      }
   }
   for (int i = 0; i < int(product.size()); i++) {
      product[i] += '0';
   }
   if (product[0] == '0') {
      return product.substr(1);
   }
   return product;
}

class CBigInt
{
  private:
    std::string number;
    char sign;
  public:
  
    // default constructor
    CBigInt(){
      number = "0";
      sign = '+';
    }
    // copying/assignment/destruction
    // int constructor
    CBigInt(int input){
      number = std::to_string(input);
      if(number[0] == '-'){
        sign = '-';
        number.erase(0,1);
      }
      else{
        sign = '+';
      }
    }
    // string constructor
    CBigInt(const std::string & input){
      if(!isValidNumber(input)){
        throw std::invalid_argument("Invalid input: " + input);
      }
      number = removeLeadingZeroes(input);
      if(number[0] == '-'){
        sign = '-';
        number.erase(0,1);
      }
      else{
        sign = '+';
      }
    }
    
    CBigInt(const char * tmp){
      std::string input = tmp;
      if(!isValidNumber(input)){
        throw std::invalid_argument("Invalid input: " + input);
      }
      number = removeLeadingZeroes(input);
      if(number[0] == '-'){
        sign = '-';
        number.erase(0,1);
      }
      else{
        sign = '+';
      }
    }
    // operator +, any combination {CBigInt/int/string} + {CBigInt/int/string}

    CBigInt operator +(const CBigInt & other) const{
      CBigInt result;
      std::string x = this->number;
      std::string y = other.number;
      CBigInt abs_x = x;
      CBigInt abs_y = y;

      if((this->sign == '+' && other.sign == '+')||(this->sign == '-' && other.sign == '-')){
        result.number = add(x,y);
        if(other.sign == '-'){
          result.sign = '-';
        }
      }
      else{
        if(abs_x > abs_y && this->sign == '-'){
          result.sign = '-';
        }
        if(abs_x < abs_y && other.sign == '-'){
          result.sign = '-';
        }
        result.number = removeLeadingZeroes(findDiff(x,y));
      }

      return result;
    }

    // operator *, any combination {CBigInt/int/string} * {CBigInt/int/string}
    CBigInt operator *(const CBigInt & other) const{
      std::string x = this->number;
      std::string y = other.number;

      CBigInt result = multiplyTwoNumbers(x,y);
      if(this->sign == '-' && other.sign == '+'){
        result.sign = '-';
      }
      if(this->sign == '+' && other.sign == '-'){
        result.sign = '-';
      }
      if(result.number == "0"){
        result.sign = '+';
      }
      return result;

    }
    
    // operator +=, any of {CBigInt/int/string}
    CBigInt& operator+=(const CBigInt& other) {
        *this = *this + other; // Use the existing + operator
        return *this;
    }

    // operator *=, any of {CBigInt/int/string}
    CBigInt& operator*=(const CBigInt& other) {
        *this = *this * other; // Use the existing * operator
        return *this;
    }

    // comparison operators, any combination {CBigInt/int/string} {<,<=,>,>=,==,!=} {CBigInt/int/string}
    bool operator <(const CBigInt & other) const{
      if(sign == '+' && other.sign == '-'){
        return false;
      }
      if(sign == '-' && other.sign == '+'){
        return true;
      }
      if(sign == '+' && other.sign == '+'){
          if(number.length() > other.number.length()){
            return false;
          }
          if(number.length() < other.number.length()){
            return true;
          }
          return number < other.number;
      }
      else{
          if(number.length() > other.number.length()){
            return true;
          }
          if(number.length() < other.number.length()){
            return false;
          }
          return number > other.number;
      }
    }

    bool operator >(const CBigInt & other) const{
      return other < (*this);
    }

    bool operator ==(const CBigInt & other) const{
      return ((!((*this) < other))&&(!(other < (*this))));
    }
    
    bool operator !=(const CBigInt & other) const{
      return !((*this) == other);
    }
    
    bool operator <=(const CBigInt & other) const{
      return (((*this) < other)||(((*this) == other)));
    }

    bool operator >=(const CBigInt & other) const{
      return (((*this) > other)||(((*this) == other)));
    }

    // output operator <<
    friend std::ostream& operator<<(std::ostream& os, const CBigInt& bigint) {
        if(bigint.sign == '-'){
          os << '-';
        }
        os << bigint.number;
        return os;
    }
    // input operator >>
    friend std::istream& operator>>(std::istream& is, CBigInt& bigint) {
        std::string input;
        bool valid = false;
        while (std::isspace(is.peek())) {
          is.get();
        }

        while (std::isdigit(is.peek())) {
        input.push_back(is.get());
        valid = true;
        }
     
        if (!valid) {
        is.setstate(std::ios::failbit);
        return is;
        }

        bigint = CBigInt(returnValidNumber(input)); // Use existing constructor to convert input to CBigInt
        return is;
    }
};


#ifndef __PROGTEST__

static bool equal ( const CBigInt & x, const char val [] )
{
  std::ostringstream oss;
  oss << x;
  return oss . str () == val;
}
static bool equalHex ( const CBigInt & x, const char val [] )
{
  return true; // hex output is needed for bonus tests only
  // std::ostringstream oss;
  // oss << std::hex << x;
  // return oss . str () == val;
}

int main ()
{

  CBigInt tmp(5241);
  //CBigInt a("1542");
  //CBigInt b("0001542");
  CBigInt c("01542");
  CBigInt d("00000");
  CBigInt e("00");
  CBigInt f("0");
  CBigInt g;

  CBigInt test = c;
  CBigInt test2 = -52;
  CBigInt test3 = "-52";
  
  //std::cout << test << std::endl;
  std::istringstream iss("00123456a78");
  iss >> test;

  assert((CBigInt("652") < CBigInt("653")) == true);
  assert((CBigInt("652") < CBigInt("651")) == false);
  assert((CBigInt("-652") < CBigInt("653")) == true);
  assert((CBigInt("652") < CBigInt("-653")) == false);
  assert((CBigInt(652) < CBigInt(653)) == true);
  assert((CBigInt(652) < CBigInt(651)) == false);
  assert((CBigInt(65) < CBigInt(653)) == true);
  assert((CBigInt(6525) < CBigInt(653)) == false);
  assert((CBigInt(-652) < CBigInt(-653)) == false);
  assert((CBigInt("-652") < CBigInt("-653")) == false);
  assert((CBigInt(-652) < CBigInt(-651)) == true);
  assert((CBigInt(-65) < CBigInt(-651)) == false);
  assert((CBigInt(-6522) < CBigInt(-651)) == true);
  assert((CBigInt(-6522) < "52") == true);
  assert((CBigInt(-6522) == CBigInt(-651)) == false);
  assert((CBigInt(-6522) != CBigInt(-651)) == true);
  assert((CBigInt(-6522) > CBigInt(-651)) == false);
  assert((CBigInt(-651) >= CBigInt(-651)) == true);
  assert((CBigInt(-65) >= CBigInt(-651)) == true);
  assert((CBigInt(-652) >= CBigInt(-651)) == false);
  assert((CBigInt(-651) <= CBigInt(-651)) == true);
  assert((CBigInt(-65) <= CBigInt(-651)) == false);
  assert((CBigInt(-652) <= CBigInt(-651)) == true);

  //a = 44;
  //b = 10050;
  //g = a * "55";

  //std::cout << g << std::endl;


  
  CBigInt a, b;
  std::istringstream is;
  a = 10;
  a += 20;
  assert ( equal ( a, "30" ) );
  a *= 5;
  assert ( equal ( a, "150" ) );
  b = a + 3;
  assert ( equal ( b, "153" ) );
  b = a * 7;
  assert ( equal ( b, "1050" ) );
  assert ( equal ( a, "150" ) );
  assert ( equalHex ( a, "96" ) );

  a = 10;
  a += -20;
  assert ( equal ( a, "-10" ) );
  a *= 5;
  assert ( equal ( a, "-50" ) );
  b = a + 73;
  assert ( equal ( b, "23" ) );
  b = a * -7;
  assert ( equal ( b, "350" ) );
  assert ( equal ( a, "-50" ) );
  assert ( equalHex ( a, "-32" ) );

  a = "12345678901234567890";
  a += "-99999999999999999999";
  assert ( equal ( a, "-87654321098765432109" ) );
  a *= "54321987654321987654";
  assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
  a *= 0;
  assert ( equal ( a, "0" ) );
  a = 10;
  b = a + "400";
  assert ( equal ( b, "410" ) );
  b = a * "15";
  assert ( equal ( b, "150" ) );
  assert ( equal ( a, "10" ) );
  assert ( equalHex ( a, "a" ) );

  is . clear ();
  is . str ( " 1234" );
  assert ( is >> b );
  assert ( equal ( b, "1234" ) );
  is . clear ();
  is . str ( " 12 34" );
  assert ( is >> b );
  assert ( equal ( b, "12" ) );
  is . clear ();
  is . str ( "999z" );
  assert ( is >> b );
  assert ( equal ( b, "999" ) );
  is . clear ();
  is . str ( "abcd" );
  assert ( ! ( is >> b ) );
  is . clear ();
  is . str ( "- 758" );
  assert ( ! ( is >> b ) );
  a = 42;
  try
  {
    a = "-xyz";
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::invalid_argument & e )
  {
    assert ( equal ( a, "42" ) );
  }

  a = "73786976294838206464";
  assert ( equal ( a, "73786976294838206464" ) );
  assert ( equalHex ( a, "40000000000000000" ) );
  assert ( a < "1361129467683753853853498429727072845824" );
  assert ( a <= "1361129467683753853853498429727072845824" );
  assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
  assert ( a != "1361129467683753853853498429727072845824" );
  assert ( ! ( a < "73786976294838206464" ) );
  assert ( a <= "73786976294838206464" );
  assert ( ! ( a > "73786976294838206464" ) );
  assert ( a >= "73786976294838206464" );
  assert ( a == "73786976294838206464" );
  assert ( ! ( a != "73786976294838206464" ) );
  assert ( a < "73786976294838206465" );
  assert ( a <= "73786976294838206465" );
  assert ( ! ( a > "73786976294838206465" ) );
  assert ( ! ( a >= "73786976294838206465" ) );
  assert ( ! ( a == "73786976294838206465" ) );
  assert ( a != "73786976294838206465" );
  a = "2147483648";
  assert ( ! ( a < -2147483648 ) );
  assert ( ! ( a <= -2147483648 ) );
  assert ( a > -2147483648 );
  assert ( a >= -2147483648 );
  assert ( ! ( a == -2147483648 ) );
  assert ( a != -2147483648 );
  a = "-12345678";
  assert ( ! ( a < -87654321 ) );
  assert ( ! ( a <= -87654321 ) );
  assert ( a > -87654321 );
  assert ( a >= -87654321 );
  assert ( ! ( a == -87654321 ) );
  assert ( a != -87654321 );

  

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
