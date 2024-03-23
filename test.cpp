#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#endif /* __PROGTEST__ */

using namespace std;

template <typename DT>
class myvector{
  public:
  size_t capacity;
  size_t size;
  DT * arr;

  myvector(){
    capacity = 2;
    size = 0;
    arr = new DT[capacity];
  }

  ~myvector() {
    delete[] arr;
  }

  void resize() {
    capacity *= 2;
    DT *temp = new DT[capacity];
    for (size_t i = 0; i < size; ++i) {
        temp[i] = arr[i];
    }
    delete[] arr;
    arr = temp;
  }

  void push_back(const DT & element) {
    if (size >= capacity) {
      resize();
    }
    arr[size] = element;
    size++;
  }

  DT& operator[](size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index in vector out of range");
    }
    return arr[index];
  }

  DT& at(size_t index) const{
    if (index >= size) {
      throw std::out_of_range("Index in vector out of range");
    }
    return arr[index];
  }

  void insert(size_t index, const DT &element) {
    if (index > size) {
      throw std::out_of_range("Index in vector out of range");
    }
    if (size >= capacity) {
      resize();
    }
      // Shift elements to make space for the new element
    for (size_t i = size; i > index; --i) {
      arr[i] = arr[i - 1];
    }
    arr[index] = element;
    size++;
  }

  void erase(size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index in vector out of range");
    }
    // Shift elements to overwrite the element at the specified index
    for (size_t i = index; i < size - 1; ++i) {
      arr[i] = arr[i + 1];
    }
    size--;
  }

  myvector(const myvector& other) : capacity(other.capacity), size(other.size), arr(new DT[other.capacity]) {
    for (size_t i = 0; i < other.size; ++i) {
      arr[i] = other.arr[i];
    }
  }

  myvector& operator=(const myvector& other) {
    if (this != &other) {
      delete[] arr;
      capacity = other.capacity;
      size = other.size;
      arr = new DT[capacity];
      for (size_t i = 0; i < other.size; ++i) {
        arr[i] = other.arr[i];
      }
    }
    return *this;
  }

};

class CBaseString{
  public:
  char * value;
  int length;

  CBaseString(){
    value = new char[1];
    value[0] = '\0';
    length = 0;
  }

  CBaseString(const char * in){
    length = strlen(in);
    value = new char[length + 1];
    strcpy(value, in);
  }

  CBaseString(const CBaseString & in){
    length = in.length;
    value = new char[length + 1];
    strcpy(value, in.value);
  }

  CBaseString & operator =(const CBaseString & in){
    if (this != &in) { // Check for self-assignment
      delete[] value; // Deallocate old memory
      length = in.length;
      value = new char[length + 1]; // Allocate new memory
      strcpy(value, in.value); // Copy content
    }
    return *this;
  }

  ~CBaseString() {
    //cout << "Destroyed:" << value << endl;
    delete[] value;
  }

};

class CStringPart{
  public:
  int begin;
  int end;
  int length;
  shared_ptr<CBaseString> basestring;

  CStringPart(){
    begin = 0;
    end = 0;
    length = 0;
    basestring = make_shared<CBaseString>();
  }

  CStringPart(const char * in){
    basestring = make_shared<CBaseString>(in);
    begin = 0;
    end = basestring.get()->length - 1;
    length = basestring.get()->length;
  }

  CStringPart(const CStringPart & in){
    begin = in.begin;
    end = in.end;
    length = in.length;
    basestring = in.basestring;
  }

  CStringPart& operator=(const CStringPart &in) {
    if (this != &in) { // Check for self-assignment
      begin = in.begin;
      end = in.end;
      length = in.length;
      basestring = in.basestring;
    }
    return *this;
  }

};

class CPatchStr
{
  public:

    myvector<CStringPart> field;
    size_t string_length;
    bool empty;

    CPatchStr (){
      CStringPart tmp;
      field.push_back(tmp);
      empty = true;
      string_length = 0;
    }
    
    CPatchStr ( const char * str ){
      CStringPart tmp(str);
      field.push_back(tmp);
      string_length = tmp.length;
      if(tmp.length > 0){
        empty = false;
      }
      else{
        empty = true;
      }
    }

    CPatchStr(const CPatchStr & in){
      field = in.field;
      empty = in.empty;
      string_length = in.string_length;
    }

    CPatchStr & operator = (const CPatchStr & in){
      if(this != &in){
        this->field = in.field;
        this->empty = in.empty;
        this->string_length = in.string_length;
      }
      return *this;
    }

    void insertPatchintovector(size_t position,const CPatchStr & in){
      if(in.empty){
        return;
      }
      if(position == field.size){
        for(size_t i = 0; i < in.field.size; i++){
          field.push_back(in.field.at(i));
          string_length += in.field.at(i).length;
        }
        return;
      }
      for(size_t i = 0; i < field.size; i++){
        if(i == position){
          size_t j;
          for(j = in.field.size - 1;j > 0; j--){
            field.insert(position, in.field.at(j));
            string_length += in.field.at(j).length;
          }
          if(j == 0){
            field.insert(position, in.field.at(0));
            string_length += in.field.at(0).length;
            return;
          }
        }
      }
    }

    CPatchStr   subStr    ( size_t            from,
                            size_t            len ) const;

    CPatchStr & append    ( const CPatchStr & src ){
      if(src.empty == true){
        return *this;
      }
      if(this->empty && !src.empty){
        this->field.size = 0;
        this->empty = false;
      }
      for(size_t i = 0; i < src.field.size; i++){
        this->field.push_back(src.field.at(i));
        this->string_length += src.field.at(i).length;
      }
      return *this;
    }

    CPatchStr & insert    ( size_t            pos,
                            const CPatchStr & src )
    {
      if(pos > string_length){
        throw std::out_of_range("Index in insert out of range");
      }
      if(pos == string_length){
        append(src);
        return *this;
      }
      size_t counter = 0;
      for(size_t i = 0; i < field.size; i++){
        if(counter == pos){
          insertPatchintovector(i, src);
          return *this;
        }
        if(counter > pos){
            //zkurvena mrdka realne dopici cotoe kurva
        }
        counter += field[i].length;
      }

      throw std::out_of_range("skipped for cycle in insert that shouldnt happen");
    }


    CPatchStr & remove    ( size_t            from,
                            size_t            len );

    char      * toStr     () const{
      if(empty){
        char * tmp = new char[1];
        tmp[0] = '\0';
        return tmp;
      }
      char * res = new char[string_length + 1];
      size_t str_index = 0;
      for(size_t i = 0; i < field.size; i++){
        CStringPart & curent = field.at(i);
        for(int j = curent.begin; j <= curent.end; j++){
          res[str_index] = curent.basestring.get()->value[j];
          str_index++;
        }
      }
      res[str_index] = '\0';
      return res;
    }
};

#ifndef __PROGTEST__
bool stringMatch ( char       * str,
                   const char * expected )
{
  bool res = std::strcmp ( str, expected ) == 0;
  delete [] str;
  return res;
}

int main ()
{
  /*
  myvector<CStringPart> test;
  CStringPart a("tmp");
  CStringPart b("BBB");
  CStringPart c("CCC");
  CStringPart d("DDD");

  test.push_back(a);
  test.push_back(b);
  test.push_back(c);
  cout << test[2].basestring.get()->value << endl;
  test.insert(0, d);
  cout << test[0].basestring.get()->value << endl;
  cout << test[2].basestring.get()->value << endl;
  test.erase(0);
  cout << test[0].basestring.get()->value << endl;
  */

  /*CPatchStr tmp("ABCD");
  CPatchStr e("E");
  CPatchStr f("F");
  CPatchStr x("");


  e.append(f);

  tmp.append(e);
  tmp.append(x);

  x.append(e);*/

  CPatchStr tmp("ABC");
  CPatchStr three("F");

  CPatchStr one("D");
  CPatchStr two("E");
  one.append(two);


  tmp.append(three);
  tmp.insert(2, one);


  char * a = tmp.toStr();
  cout << a << endl;
  delete [] a;
 
  /*
  char tmpStr[100];

  CPatchStr a ( "test" );
  assert ( stringMatch ( a . toStr (), "test" ) );
  std::strncpy ( tmpStr, " da", sizeof ( tmpStr ) - 1 );
  a . append ( tmpStr );
  assert ( stringMatch ( a . toStr (), "test da" ) );
  std::strncpy ( tmpStr, "ta", sizeof ( tmpStr ) - 1 );
  a . append ( tmpStr );
  assert ( stringMatch ( a . toStr (), "test data" ) );
  std::strncpy ( tmpStr, "foo text", sizeof ( tmpStr ) - 1 );
  CPatchStr b ( tmpStr );
  assert ( stringMatch ( b . toStr (), "foo text" ) );
  CPatchStr c ( a );
  assert ( stringMatch ( c . toStr (), "test data" ) );
  CPatchStr d ( a . subStr ( 3, 5 ) );
  assert ( stringMatch ( d . toStr (), "t dat" ) );
  d . append ( b );
  assert ( stringMatch ( d . toStr (), "t datfoo text" ) );
  d . append ( b . subStr ( 3, 4 ) );
  assert ( stringMatch ( d . toStr (), "t datfoo text tex" ) );
  c . append ( d );
  assert ( stringMatch ( c . toStr (), "test datat datfoo text tex" ) );
  c . append ( c );
  assert ( stringMatch ( c . toStr (), "test datat datfoo text textest datat datfoo text tex" ) );
  d . insert ( 2, c . subStr ( 6, 9 ) );
  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
  b = "abcdefgh";
  assert ( stringMatch ( b . toStr (), "abcdefgh" ) );
  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
  assert ( stringMatch ( d . subStr ( 4, 8 ) . toStr (), "at datfd" ) );
  assert ( stringMatch ( b . subStr ( 2, 6 ) . toStr (), "cdefgh" ) );
  try
  {
    b . subStr ( 2, 7 ) . toStr ();
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::out_of_range & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  a . remove ( 3, 5 );
  assert ( stringMatch ( a . toStr (), "tesa" ) );

  */

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
