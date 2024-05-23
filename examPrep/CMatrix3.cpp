#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */
 
template <typename T_>
struct tmp_row{
  T_ * ptr = nullptr;
  size_t limit_z;

  T_ & operator[](size_t z){
    if(z >= limit_z){
      throw out_of_range("achjo");
    }
    return ptr[z];
  }
};

template <typename T_>
struct tmp_2D{
  T_ ** ptr = nullptr;
  size_t limit_y;
  size_t limit_z;

  tmp_row<T_> operator[](size_t y){
    if(y >= limit_y){
      throw out_of_range("achjo");
    }
    tmp_row<T_> tmp;
    tmp.ptr = ptr[y];
    tmp.limit_z = limit_z;
    return tmp;
  }
};

template <typename T_>
class CMatrix3
{
    T_ *** field;
    unsigned size_1 = 0;
    unsigned size_2 = 0;
    unsigned size_3 = 0;

 
public:
 
    // constructor ( unsigned n1, unsigned n2, unsigned n3 )
    CMatrix3(unsigned n1, unsigned n2, unsigned n3){

      size_1 = n1;
      size_2 = n2;
      size_3 = n3;

      field = new T_**[size_1];
      for(size_t x = 0; x < size_1;x++){
        field[x] = new T_*[size_2];
        for(size_t y = 0; y < size_2;y++){
          field[x][y] = new T_[size_3];
        }
      }

    }
    // destructor
    ~CMatrix3(){
      for(size_t x = 0; x < size_1;x++){
        for(size_t y = 0; y < size_2;y++){
          delete[] field[x][y];
        }
        delete[] field[x];
      }
      delete[] field;
    }
    // copy constructor
    CMatrix3(const CMatrix3 & in){
      if(this == &in){
        return;
      }
      if(size_1 != 0){
        for(size_t x = 0; x < size_1;x++){
          for(size_t y = 0; y < size_2;y++){
            delete[] field[x][y];
          }
          delete[] field[x];
        }
        delete[] field;
      }

      size_1 = in.size_1;
      size_2 = in.size_2;
      size_3 = in.size_3;

      field = new T_**[size_1];
      for(size_t x = 0; x < size_1;x++){
        field[x] = new T_*[size_2];
        for(size_t y = 0; y < size_2;y++){
          field[x][y] = new T_[size_3];
        }
      }

      for(size_t x = 0; x < size_1;x++){
        for(size_t y = 0; y < size_2;y++){
          for(size_t z = 0; z < size_3;z++){
            field[x][y][z] = in.field[x][y][z];
          }
        }
      }

    }
    // operator =
    CMatrix3 & operator = (const CMatrix3 & in){
      if(this == &in){
        return *this;
      }
      if(size_1 != 0){
        for(size_t x = 0; x < size_1;x++){
          for(size_t y = 0; y < size_2;y++){
            delete[] field[x][y];
          }
          delete[] field[x];
        }
        delete[] field;
      }

      size_1 = in.size_1;
      size_2 = in.size_2;
      size_3 = in.size_3;

      field = new T_**[size_1];
      for(size_t x = 0; x < size_1;x++){
        field[x] = new T_*[size_2];
        for(size_t y = 0; y < size_2;y++){
          field[x][y] = new T_[size_3];
        }
      }

      for(size_t x = 0; x < size_1;x++){
        for(size_t y = 0; y < size_2;y++){
          for(size_t z = 0; z < size_3;z++){
            field[x][y][z] = in.field[x][y][z];
          }
        }
      }
      return *this;
    }
    // operator [][][]
    tmp_2D<T_> operator[](size_t x){
      if(x >= size_1){
        throw out_of_range("achjo");
      }
      
      tmp_2D<T_> tmp;
      tmp.limit_y = size_2;
      tmp.limit_z = size_3;

      tmp.ptr = field[x];
      return tmp;
    }
    // operator +
    // operator ==
};
 
#ifndef __PROGTEST__
int main ( void )
{
    CMatrix3<int> m ( 3, 4, 5 );
    CMatrix3<int> r (m);

    m[0][0][0] = 128;
    m[2][3][1] = 7;
    assert ( m[0][0][0] == 128 );
    assert ( m[2][3][1] == 7 );
 
    m[1][1][1] = m[0][0][0] + m[2][3][1];
    m[2][3][4] = 135;
    assert ( m[1][1][1] == m[2][3][4] );
 
    CMatrix3<int> copy ( m );
    try { copy[4][1][2]; assert ( "Missing exception for access i0 = 4, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[1][4][2]; assert ( "Missing exception for access i1 = 4, limit = 3" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[2][1][6]; assert ( "Missing exception for access i2 = 6, limit = 4" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
 
    copy[2][3][1] = 42;
    assert ( m[2][3][1] == 7 );
    assert ( copy[2][3][1] == 42 );
 
    CMatrix3<int> n ( 2, 2, 2 );
    n[0][1][0] = 56;
    n[1][0][1] = 42;
    m = n;
    n[0][1][0] = 0;
    assert ( m[0][1][0] == 56 );
    try { m[2][1][0]; assert ( "Missing exception for access i0 = 2, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
 
    CMatrix3<string> s ( 2, 1, 1 );
    s[0][0][0] = "Hello ";
    s[1][0][0] = "World!";
    assert ( ( s[0][0][0] + s[1][0][0] ) == string("Hello World!") );
    /*
    */
 
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */