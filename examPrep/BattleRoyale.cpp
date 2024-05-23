#ifndef __PROGTEST__
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
 
using namespace std;
 
class CPos {
public:
    CPos(int y, int x, int hour)
        : m_hour(hour), m_posY(y), m_posX(x)
    {
    }
 
    int m_hour;
    int m_posY;
    int m_posX;
};
#endif /* __PROGTEST__ */

struct CCell{
    int closed_after = INT32_MAX;
    int x = -1;
    int y = -1;
    int travel_time = 0;
    CCell(){}
    CCell(int a, int b){
        x = a;
        y = b;
    }
};

class CBattleRoyale {
public:
    CBattleRoyale(unsigned m_height, unsigned m_width, const std::list<CPos> &zones) 
    {
        height = m_height;
        width = m_width;
        for(size_t i = 0; i < width;i++){
            field.push_back(vector<CCell>{});
        }
        for(size_t i = 0; i < width;i++){
            for(size_t j = 0; j < height;j++){
                field[i].push_back(CCell{int(i), int(j)});
            }
        }
        for(const CPos & i : zones){
            field[i.m_posX][i.m_posY].closed_after = i.m_hour;
        }
    }

    bool valid_pos(int x, int y) const{
        if((x < 0) || (x >= int(width))){
            return false;
        }
        if((y < 0) || (y >= int(height))){
            return false;
        }
        return true;
    }
 
    // des (if needed)
 
    unsigned findRoute(int ySt, int xSt, int yEn, int xEn) const 
    {
        vector<vector<CCell>> real_field = field;
        queue<CCell *> que;
        for(size_t i = 0; i < width;i++){
            for(size_t j = 0; j < height;j++){
                real_field[i][j].travel_time = INT32_MAX;
            }
        }

        que.push(&real_field[xSt][ySt]);
        real_field[xSt][ySt].travel_time = 0;
        if(real_field[xSt][ySt].closed_after == 0){
            throw logic_error("nejde");
        }

        while(!que.empty()){
            CCell & cur = *que.front();
            int x1 = cur.x - 1;
            int y1 = cur.y;

            int x2 = cur.x + 1;
            int y2 = cur.y;
            
            int x3 = cur.x;
            int y3 = cur.y - 1;
            
            int x4 = cur.x;
            int y4 = cur.y + 1;

            if(valid_pos(x1, y1)){
                //mby of by one error
                if(real_field[x1][y1].closed_after > cur.travel_time + 2){
                    if(real_field[x1][y1].travel_time > cur.travel_time + 1){
                        real_field[x1][y1].travel_time = cur.travel_time + 1;
                        que.push(&real_field[x1][y1]);
                    }
                }
            }
            if(valid_pos(x2, y2)){
                //mby of by one error
                if(real_field[x2][y2].closed_after > cur.travel_time + 2){
                    if(real_field[x2][y2].travel_time > cur.travel_time + 1){
                        real_field[x2][y2].travel_time = cur.travel_time + 1;
                        que.push(&real_field[x2][y2]);
                    }
                }
            }
            if(valid_pos(x3, y3)){
                //mby of by one error
                if(real_field[x3][y3].closed_after > cur.travel_time + 2){
                    if(real_field[x3][y3].travel_time > cur.travel_time + 1){
                        real_field[x3][y3].travel_time = cur.travel_time + 1;
                        que.push(&real_field[x3][y3]);
                    }
                }
            }
            if(valid_pos(x4, y4)){
                //mby of by one error
                if(real_field[x4][y4].closed_after > cur.travel_time + 2){
                    if(real_field[x4][y4].travel_time > cur.travel_time + 1){
                        real_field[x4][y4].travel_time = cur.travel_time + 1;
                        que.push(&real_field[x4][y4]);
                    }
                }
            }
            que.pop();
        }

        if(real_field[xEn][yEn].travel_time == INT32_MAX){
            throw logic_error("nejde");
        }
        return real_field[xEn][yEn].travel_time;
    }
 
private:
    unsigned height;
    unsigned width;
    vector<vector<CCell>> field;
};
 
#ifndef __PROGTEST__
int main() {
    CBattleRoyale x1(5, 2, {});
    CBattleRoyale r1(5, 5, {});
    assert(r1.findRoute(0, 0, 4, 0) == 4);
    assert(r1.findRoute(4, 4, 4, 4) == 0);
 
    CBattleRoyale r2(6, 7, {CPos(1, 0, 1), CPos(2, 1, 2), CPos(3, 2, 5)});
    assert(r2.findRoute(0, 0, 4, 0) ==  10);
 
 
    CBattleRoyale r3(8, 8, {CPos(0, 2, 1), CPos(3, 1, 2), CPos(2, 1, 0)});
    try {
	cout << r3.findRoute(2, 1, 4, 7) << endl;
        assert("Exception missing!" == nullptr);
    } catch (const logic_error &e) {
    } catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    assert(r3.findRoute(0,2,3,0)==5);
 
    CBattleRoyale b(5,5,{CPos(0,1,1),CPos(1,1,0)});
    assert(b.findRoute(0,0,2,2)==4);
    assert(b.findRoute(0,0,0,2)==6);
    assert(b.findRoute(3,3,3,3)==0);
    try{
        assert(b.findRoute(1,1,2,1)==1);
        assert("Kde vyjimka?"==nullptr);
    } catch (logic_error & x){}
    try{
        assert(b.findRoute(1,1,1,1)==0);
        assert("Kde vyjimka? xd"==nullptr);
    } catch (logic_error & x){}
 
    CBattleRoyale b1(5,5,{CPos(2,0,2), CPos(2,1,1),CPos(2,2,1), CPos(2,3,3),CPos(2,4,4)});
    try{
        cout << b1.findRoute(1,1,3,1) << endl;
        assert("Kde vyjimka?"==nullptr);
    } catch (logic_error & x){}
    /*
    */
}
#endif /* __PROGTEST__ */