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
struct helper{
    int x = 0;
    int y_ofset = 0;
    int start_y = 0;
    int end_y = 0;

    T_ ** ptr;

    T_ & operator[](int y){
        if(y < start_y || y > end_y){
            throw out_of_range("njede");
        }

        int second = y - y_ofset;
        return ptr[x][second];
    }
};


template <typename T_>
class CMatrix
{
    T_ ** table = nullptr;
    int start_x = 0;
    int start_y = 0;
    int end_x = 0;
    int end_y = 0;
    int rows = 0;
    int cols = 0;
 
public:
 
    // constructor ( unsigned n1, unsigned n2, unsigned n3 )
    CMatrix(int x1, int x2, int y1, int y2){
        if(x1 > x2 || y1 > y2){
            throw logic_error("nejde");
        }
        start_x = x1;
        end_x = x2;
        start_y = y1;
        end_y = y2;

        rows = abs(x1 - x2) + 1;
        cols = abs(y1 - y2) + 1;

        table = new T_*[rows];
        for(int i = 0; i < rows; i++){
            table[i] = new T_[cols];
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                table[i][j] = T_{};
            }
        }
    }
    // destructor
    ~CMatrix(){
        for(int i = 0; i < rows; i++){
            delete[] table[i];
        }
        delete[] table;
    }
    // copy constructor
    CMatrix(const CMatrix & in){
        if(this == &in){
            return;
        }
        if(table){
            for(int i = 0; i < rows; i++){
                delete[] table[i];
            }
            delete[] table;
        }

        start_x = in.start_x;
        end_x = in.end_x;
        start_y = in.start_y;
        end_y = in.end_y;

        rows = abs(in.start_x - in.end_x) + 1;
        cols = abs(in.start_y - in.end_y) + 1;

        table = new T_*[rows];
        for(int i = 0; i < rows; i++){
            table[i] = new T_[cols];
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                table[i][j] = in.table[i][j];
            }
        }
    }
    // operator =
    CMatrix & operator = (const CMatrix & in){
        if(this == &in){
            return *this;
        }
        if(table){
            for(int i = 0; i < rows; i++){
                delete[] table[i];
            }
            delete[] table;
        }

        start_x = in.start_x;
        end_x = in.end_x;
        start_y = in.start_y;
        end_y = in.end_y;

        rows = abs(in.start_x - in.end_x) + 1;
        cols = abs(in.start_y - in.end_y) + 1;

        table = new T_*[rows];
        for(int i = 0; i < rows; i++){
            table[i] = new T_[cols];
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                table[i][j] = in.table[i][j];
            }
        }
        return *this;
    }
    // operator [][]
    helper<T_> operator[](int x){
        if(x < start_x || x > end_x){
            throw out_of_range("njede");
        }
        int first = x - start_x;
        int y_off = start_y;

        helper<T_> tmp;
        tmp.ptr = table;
        tmp.x = first;
        tmp.y_ofset = y_off;
        tmp.start_y = start_y;
        tmp.end_y = end_y;

        return tmp;
    }
    // operator +
    // operator ==
    bool operator == (const CMatrix & in){
        if((in.end_x != end_x)||
            (in.start_x != start_x)||
            (in.end_y != end_y)||
            (in.start_y != start_y)
        ){
            return false;
        }
        for(int i = 0; i < in.rows;i++){
            for(int j = 0; j < in.cols;j++){
                if(table[i][j] != in.table[i][j]){
                    return false;
                }
            }
        }
        return true;
    }

};

int main(void){
    
    CMatrix<int> a(-3,-1,-2,2);
    a[-3][-2] = 5;
    assert(a[-3][-2] == 5);
    a[-2][-2] = 4;
    assert(a[-2][-2] == 4);
    a[-2][2] = 3;
    assert(a[-2][2] == 3);

    CMatrix<int> b = a;
    a[-3][-2] = 69;
    assert(b[-3][-2] == 5);
    a[-2][-2] = 69;
    assert(b[-2][-2] == 4);
    a[-2][2] = 69;
    cout << b[-2][2] << endl;
    assert(b[-2][2] == 3);
    
    // Integer test
    CMatrix<int> m1( -2, 2, -2, 2 );
    assert( ( m1[-2][-2] = 5 ) == 5 );
    assert( ( m1[2][2] = 10 ) == 10 );
 
    try {
        int x = m1[3][3];
    } catch ( const std::out_of_range& e ) {
        std::cout << e.what() << std::endl;
    }
 
    // Double test
    CMatrix<double> m2( -2, 2, -2, 2 );
    assert( ( m2[-2][-2] = 5.5 ) == 5.5 );
    assert( ( m2[2][2] = 10.5 ) == 10.5 );
 
    try {
        double x = m2[3][3];
    } catch ( const std::out_of_range& e ) {
        std::cout << e.what() << std::endl;
    }
 
    // String test
    CMatrix<std::string> m3( -2, 2, -2, 2 );
    assert( ( m3[-2][-2] = "test1" ) == "test1" );
    assert( ( m3[2][2] = "test2" ) == "test2" );
 
    try {
        std::string x = m3[3][3];
    } catch ( const std::out_of_range& e ) {
        std::cout << e.what() << std::endl;
    }
 
    // Test copy constructor and equality operator
    CMatrix<int> m4( -2, 2, -2, 2 );
    m4[-2][-2] = 5;
    m4[2][2] = 10;
    CMatrix<int> m5( m4 );
    assert( m4 == m5 );
 
    // Test assignment operator and equality operator
    CMatrix<int> m6( -2, 2, -2, 2 );
    m6[-2][-2] = 5;
    m6[2][2] = 10;
    CMatrix<int> m7 = m6;
    assert( m6 == m7 );


    return 0;
}
