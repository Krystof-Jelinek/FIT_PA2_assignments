#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include <queue>
#include <sstream>
using namespace std;
 

struct CStation{
    string name;
    set<string> neigbours;
    set<int> were_here;
    map<int, int> payed;

    bool containsall_indexes(int upperlimit){
        for( int i = 0 ; i < upperlimit;i++){
            if(were_here.find(i) == were_here.end()){
                return false;
            }
        }
        return true;
    }

    int countprice(){
        int cnt = 0;
        for(auto itr = payed.begin(); itr != payed.end(); itr++){
            cnt += itr->second;
        }
        return cnt;
    }
};

class CTrain
{
  public:
                             CTrain                        ( void )
    {
      // todo
    }
                            ~CTrain                        ( void )
    {
      // todo
    }
    void                     Add                           ( istream & is )
    {
        string name;
        string before;
        bool first = true;
      while(getline(is, name)){
        if(first){
            first = false;
            graf[name].name = name;
            before = name;
        }
        else{
            graf[name].name = name;
            graf[name].neigbours.insert(before);
            graf[before].neigbours.insert(name);
            before = name;
        }
      }
    }
    set<string>              Dest                          ( const vector<string> & stations,
                                                             int                  & cost )
    {
        //setup
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.payed.clear();
            itr->second.were_here.clear();
        }

        queue<CStation *> que;

        int identify_index = 0;
        for(const string & i : stations){
            graf[i].were_here.insert(identify_index);
            graf[i].payed[identify_index] = 0;
            identify_index++;
            que.push(&graf[i]);
        }

        while(!que.empty()){
            CStation & cur = *que.front();

            for(const string & i : cur.neigbours){
                for(int j : cur.were_here){
                    if(graf[i].were_here.find(j) == graf[i].were_here.end()){
                        graf[i].were_here.insert(j);
                        graf[i].payed[j] = cur.payed[j] + 1;
                        que.push(&graf[i]);
                    }
                }
            }
            que.pop();
        }

        set<string> res;

        int min_price = INT32_MAX;

        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            if(itr->second.containsall_indexes(identify_index)){
                if(itr->second.countprice() < min_price){
                    min_price = itr->second.countprice();
                    res.clear();
                    res.insert(itr->first);
                }
                if(itr->second.countprice() == min_price){
                    res.insert(itr->first);
                }
            }
        }
        if(!res.empty()){
            cost = min_price;
        }
        return res;
    }
  private:
    map<string, CStation> graf;
};
 
int main ( void )
{
  CTrain t0;
  istringstream iss;
 
  iss . clear ();
  iss . str (
    "1\n"
    "2\n"
    "3\n"
    "4\n"
    "5\n"
    "6\n"
    "7\n"
    "8\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "12\n"
    "11\n"
    "4\n"
    "9\n"
    "10\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "15\n"
    "11\n"
    "13\n"
    "14\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "7\n"
    "16\n"
    "17\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "20\n"
    "19\n"
    "17\n"
    "18\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "21\n"
    "22\n"
    "23\n"
    "25\n"
    "24\n"
    "21\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "26\n"
    "27\n"
    "28\n" );
  t0 . Add ( iss );
  iss . clear ();
  iss . str (
    "27\n"
    "29\n"
    "30\n" );
  t0 . Add ( iss );
 
  int cost = 5381;
  assert ( t0 . Dest ({ "2", "5", "11" }, cost ) == set <string> ({ "4" })
           && cost == 4 );
  assert ( t0 . Dest ({ "12", "10" }, cost ) == set <string> ({ "10", "9", "4", "11", "12" })
           && cost == 4 );
  assert ( t0 . Dest ({ "2", "9", "19" }, cost ) == set <string> ({ "4" })
           && cost == 9 );
  assert ( t0 . Dest ({ "15", "12", "13" }, cost ) == set <string> ({ "11" })
           && cost == 3 );
  cost = 5381;
  auto res = t0 . Dest ({ "21", "26" }, cost );
  assert ( t0 . Dest ({ "21", "26" }, cost ) == set <string> ()
           && cost == 5381 );
  cost = 1256;
  assert ( t0 . Dest ({ "10", "28" }, cost ) == set <string> ()
           && cost == 1256 );
  assert ( t0 . Dest ({ "21", "25" }, cost ) == set <string> ({ "21", "24", "25" })
           && cost == 2 );
  assert ( t0 . Dest ({ "21", "21" }, cost ) == set <string> ({ "21" })
           && cost == 0 );
  assert ( t0 . Dest ({ "23", "21" }, cost ) == set <string> ({ "21", "22", "23" })
           && cost == 2 );
  assert ( t0 . Dest ({ "12", "20" }, cost ) == set <string> ({ "12", "11", "4", "5", "6", "7", "16", "17", "19", "20" })
           && cost == 9 );
  assert ( t0 . Dest ({ "50" }, cost ) == set <string> ({ "50" })
           && cost == 0 );
  assert ( t0 . Dest ({ "13", "10" }, cost ) == set<string> ({ "10", "11", "13", "4", "9" })
           && cost == 4 );
  assert ( t0 . Dest ({ "17", "19" }, cost ) == set<string> ({ "17", "19" })
           && cost == 1 );
  cost = 1999;
  assert ( t0 . Dest ({ "29", "25" }, cost ) == set<string> ()
           && cost == 1999 );
  assert ( t0 . Dest ({ "8", "5" }, cost ) == set<string> ({ "5", "6", "7", "8" })
           && cost == 3 );
  assert ( t0 . Dest ({ "4", "4" }, cost ) == set<string> ({ "4" })
           && cost == 0 );  
 
    /*
    */

  return 0;
}