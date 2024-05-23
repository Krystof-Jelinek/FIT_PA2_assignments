#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <vector>
using namespace std;


class CFlightList {
 struct CAirport;
public:
    CFlightList(){
        m_First = nullptr;
        m_Last = nullptr;
    }
    CFlightList(const CFlightList & src){
        CAirport * cur = src.m_First;
        m_First = nullptr;
        m_Last = nullptr;
        map<CAirport *, CAirport *> ptr_map;
        while(cur){
            if(!m_First){
                m_First = new CAirport{cur->m_Name};
                m_First->m_Next = nullptr;
                m_Last = m_First;
                ptr_map.insert(make_pair(cur, m_First));
            }
            else{
                m_Last->m_Next = new CAirport{cur->m_Name};
                m_Last = m_Last->m_Next;
                ptr_map.insert(make_pair(cur, m_Last));
            }
            cur = cur->m_Next;
        }
        cur = src.m_First;
        while(cur){
            for(int i = 0; i < cur->m_Nr;i++){
                ptr_map[cur]->insert(ptr_map[cur->m_Dest[i]]);
            }
            cur = cur -> m_Next;
        }
    }

    ~CFlightList(){
        if(m_First){
            m_First->killMeAndMySons();
        }
    }
    
    CAirport * findPort(const string & in){
        CAirport * cur = m_First;
        while(cur){
            if(cur->m_Name == in){
                return cur;
            }
            cur = cur->m_Next;
        }
        return nullptr;
    }

    void addToList(const string & name){
        if(!m_First){
            m_First = new CAirport{name};
            m_First->m_Next = nullptr;
            m_Last = m_First;
        }
        else{
            m_Last->m_Next = new CAirport{name};
            m_Last = m_Last->m_Next;
        }
    }

    bool Add (const string & a, const string & b){
        if(a == b){
            return false;
        }

        if(m_First == nullptr){
            m_First = new CAirport{a};
            m_Last = new CAirport{b};
            m_First->m_Next = m_Last;
            m_First->insert(m_Last);
            m_Last->insert(m_First);
            return true;
        }

        CAirport * a_ptr = findPort(a);
        CAirport * b_ptr = findPort(b);

        if(a_ptr == nullptr){
            a_ptr = new CAirport{a};
            m_Last->m_Next = a_ptr;
            m_Last = a_ptr;
        }

        if(b_ptr == nullptr){
            b_ptr = new CAirport{b};
            m_Last->m_Next = b_ptr;
            m_Last = b_ptr;
        }

        if(a_ptr->has(b)){
            return false;
        }

        a_ptr->insert(b_ptr);
        b_ptr->insert(a_ptr);
        return true;
    }
    friend ostream & operator << (ostream & os, const CFlightList & src);
private:
    struct CAirport {
        static const int MAXSIZE = 100;
        CAirport(string Nm_Name) : m_Name(Nm_Name), m_Nr(0), m_Next(NULL) {};
        string m_Name;
        int m_Nr;
        CAirport * m_Next;
        CAirport * m_Dest [MAXSIZE];
        void insert(CAirport * in){
            if(m_Nr == MAXSIZE){
                throw out_of_range("neni misto");
            }
            m_Dest[m_Nr] = in;
            m_Nr++;
        }

        void killMeAndMySons(){
            if(m_Next){
                m_Next->killMeAndMySons();
            }
            delete this;
        }

        bool has(const string & name){
            for(int i = 0; i < m_Nr; i++){
                if(m_Dest[i]->m_Name == name){
                    return true;
                }
            }
            return false;
        }
    };
    friend class CTester;
    CAirport * m_First;
    CAirport * m_Last;

};
 
ostream & operator << (ostream & os, const CFlightList & src) {
    for (CFlightList::CAirport * tmp = src.m_First; tmp; tmp = tmp->m_Next) {
        os << tmp->m_Name << "[" << tmp->m_Nr << "]:";
        for (int i=0; i!=tmp->m_Nr; i++) {
            os << tmp->m_Dest[i]->m_Name << "[" << tmp->m_Dest[i]->m_Nr << "]";
            os << ", ";
        }
        os << endl;
    }
    return os;
}

int main(void){
    CFlightList f0;
    assert(f0.Add("Prague", "Berlin") == true);
    assert(f0.Add("Prague", "Berlin") == false);
    assert(f0.Add("Prague", "London") == true);
    assert(f0.Add("London", "Berlin") == true);
    assert(f0.Add("Berlin", "London") == false);
    assert(f0.Add("Berlin", "Berlin") == false);
    assert(f0.Add("New York", "Oslo") == true);

    for(int i = 0; i < 1000; i++){
        f0.Add("A" + to_string(i), to_string(i));
    }
    for(int i = 0; i < 1000; i++){
        f0.Add("A" + to_string(i), to_string(i + 1));
        f0.Add("A" + to_string(i), to_string(i + 2));
        f0.Add("A" + to_string(i), to_string(i + 3));
        f0.Add("A" + to_string(i), to_string(i + 4));
        f0.Add("A" + to_string(i), to_string(i + 5));
    }
    
    cout << f0 << endl;
    /*
    Prague[2]:Berlin[2], London[2], 
    Berlin[2]:Prague[2], London[2], 
    London[2]:Prague[2], Berlin[2], 
    New York[1]:Oslo[1], 
    Oslo[1]:New York[1],
    */
 
    CFlightList f1 (f0);
    CFlightList f2 (f0);
    CFlightList f3 (f0);
    CFlightList f4 (f0);
    CFlightList f5 (f0);
    CFlightList f6 (f0);
    CFlightList f7 (f0);
    CFlightList f8 (f0);
    CFlightList f9 (f0);
    CFlightList f10 (f0);
    for(int i = 0; i < 200; i++){
        CFlightList f10 (f0);
        f10.findPort("toejedno");
    }

 
    assert(f1.Add("Paris", "Wien") == true);
 
    cout << f0 << endl;
    /*
    Prague[2]:Berlin[2], London[2], 
    Berlin[2]:Prague[2], London[2], 
    London[2]:Prague[2], Berlin[2], 
    New York[1]:Oslo[1], 
    Oslo[1]:New York[1],
    */
    cout << f1 << endl;
    /*
    Prague[2]:Berlin[2], London[2], 
    Berlin[2]:Prague[2], London[2], 
    London[2]:Prague[2], Berlin[2], 
    New York[1]:Oslo[1], 
    Oslo[1]:New York[1], 
    Paris[1]:Wien[1], 
    Wien[1]:Paris[1], 
    */
}