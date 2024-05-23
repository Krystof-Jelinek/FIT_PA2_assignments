#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
 
using namespace std;

struct CNode{
    bool visited;
    string name;
    set<string> neighbours;
};


class CTrain
{
public:
    void Add(istringstream &is);
    int Count(void) const;
 
    void bfs(CNode * start) const{
        queue<CNode *> que;
        que.push(start);
        start->visited = true;

        while(!que.empty()){
            CNode & cur = *que.front();
            for(const string & i : cur.neighbours){
                if(!graf[i].visited){
                    graf[i].visited = true;
                    que.push(&graf[i]);
                }
            }
            que.pop();
        }
    }

private:
    mutable map<string, CNode> graf;
};
 
void CTrain::Add(istringstream &is)
{
    string cur;
    string before = "";
    while(getline(is, cur)){
        if(cur == ""){
            continue;
        }
        if(before == ""){
            graf[cur].name = cur;
            before = cur;
        }
        else{
            graf[cur].name = cur;
            graf[cur].neighbours.insert(before);
            graf[before].neighbours.insert(cur);
            before = cur;
        }
    }
}
 

int CTrain::Count(void) const
{
    if(graf.empty()){
        return 0;
    }

    //prepare graf
    for(auto itr = graf.begin(); itr != graf.end(); itr++){
        itr->second.visited = false;
    }

    int cnt = 1;
    bfs(&graf.begin()->second);
    for(auto itr = graf.begin(); itr != graf.end(); itr++){
        if(!itr->second.visited){
            bfs(&itr->second);
            cnt++;
        }
    }
    return cnt;
}
 
int main()
{
    CTrain t0;
    istringstream iss;
    assert(t0.Count() == 0);
 
    iss.clear();
    iss.str("Newton\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
 
    iss.clear();
    iss.str("Newton\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
 
    iss.clear();
    iss.str("\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
 
    iss.clear();
    iss.str("Newton\nNewton\nNewton\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
 
    iss.clear();
    iss.str("Black Hill\n");
    t0.Add(iss);
    assert(t0.Count() == 2);
 
    iss.clear();
    iss.str("Wood Side\n");
    t0.Add(iss);
    cout <<t0.Count() << endl; 
    assert(t0.Count() == 3);
 
    iss.clear();
    iss.str("Green Hill\n");
    t0.Add(iss);
    assert(t0.Count() == 4);
 
    iss.clear();
    iss.str("Lakeside\n");
    t0.Add(iss);
    assert(t0.Count() == 5);
 
    iss.clear();
    iss.str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
 
    iss.clear();
    iss.str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
    t0.Add(iss);
    assert(t0.Count() == 2);
 
    iss.clear();
    iss.str("Waterton West\nWaterton Central\nWaterton East\nWaterton Woods\nLittle Waterton\nWaterton West\n");
    t0.Add(iss);
    assert(t0.Count() == 3);
 
    iss.clear();
    iss.str("Little Waterton\nLakeside Central\n");
    t0.Add(iss);
    assert(t0.Count() == 2);
 
    iss.clear();
    iss.str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
 
    iss.clear();
    iss.str("Sodor Ironworks\nSodor Steamworks\nKnapford\nMaron\n");
    t0.Add(iss);
    assert(t0.Count() == 2);
 
    iss.clear();
    iss.str("Tidmouth\nGordons Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
    t0.Add(iss);
    assert(t0.Count() == 2);
 
    iss.clear();
    iss.str("Newton\nNewton Crossroad\nTidmouth\nBrendam Docks\n");
    t0.Add(iss);
    assert(t0.Count() == 1);
    return 0;
}