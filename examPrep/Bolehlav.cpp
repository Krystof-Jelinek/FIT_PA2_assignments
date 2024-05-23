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
    bool is_a_station = false;
    string name;
    set<string> neighbours;
};

struct CState{
    string cur_at;
    map<string, bool> state;
    int cur_cost;
};

bool operator ==(const CState & f, const CState & s){
    return((f.cur_at == s.cur_at)&&(f.state == s.state));
}

bool operator !=(const CState & f, const CState & s){
    return !((f.cur_at == s.cur_at)&&(f.state == s.state));
}

class CTrain
{
public:
    void Add(istringstream &is);

    bool alreadyexists(const CState & in) const{
        for(const CState & i : saved_states){
            if((i == in)&&(i.cur_cost <= in.cur_cost)){
                return true;
            }
        }
        return false;
    }

    bool isEnd(const CState & in) const{
        for(auto itr = in.state.begin(); itr != in.state.end(); itr++){
            if(itr->second == false){
                return false;
            }
        }
        return true;
    }

    list<string> createPath(CState end, string start, list<string> stations) const{
        list<string> res;
        CState cur = end;
        CState begin;
        begin.cur_at = start;
        begin.cur_cost = 0;
        for(const string & i : stations){
            begin.state[i] = false;
        }

        while(cur != begin){
            res.push_back(cur.cur_at);
            CNode & holding = graf[cur.cur_at];
            for(const string & i : holding.neighbours){
                if(holding.is_a_station){
                    CState next = cur;
                    next.cur_at = i;
                    next.cur_cost = cur.cur_cost - 1;
                    next.state[cur.cur_at] = false;
                    if(alreadyexists(next)){
                        cur = next;
                        break;
                    }
                }
                else{
                    CState next = cur;
                    next.cur_at = i;
                    next.cur_cost = cur.cur_cost - 1;
                    if(alreadyexists(next)){
                        cur = next;
                        break;
                    }
                }
            }
        }
        res.push_back(begin.cur_at);
        reverse(res.begin(), res.end());
        for(auto itr = res.begin(); itr != res.end();){
            if(!graf[*itr].is_a_station){
                itr = res.erase(itr);
                continue;
            }
            itr++;
        }
        return res;
    }

    list<string> getResult(list<string> stations, string start) const{
        //prepare graf

        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.is_a_station = false;
        }

        for(const string & i : stations){
            graf[i].is_a_station = true;
        }

        saved_states.clear();
        CState begin;
        begin.cur_at = start;
        begin.cur_cost = 0;
        for(const string & i : stations){
            begin.state[i] = false;
        }

        queue<CState> que;
        que.push(begin);
        saved_states.push_back(begin);

        while(!que.empty()){
            CState & cur = que.front();
            CNode & cur_node = graf[cur.cur_at];

            if(isEnd(cur)){
                cout <<"cena: " <<  cur.cur_cost << endl;
                return createPath(cur, start, stations);
            }


            for(const string & i : cur_node.neighbours){
                CState tmp_new = cur;
                if(graf[i].is_a_station){
                    tmp_new.state[i] = true;
                }
                tmp_new.cur_at = i;
                tmp_new.cur_cost = cur.cur_cost + 1;
                if(alreadyexists(tmp_new)){
                    continue;
                }
                else{
                    que.push(tmp_new);
                    saved_states.push_back(tmp_new);
                }
            }
            que.pop();
        }
        return {};
    }
 
private:
    mutable map<string, CNode> graf;
    mutable vector<CState> saved_states;
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
 

int main()
{
    CTrain t0;
    istringstream iss;
    iss.str("5\n1\n0\n2\n3\n4\n");
    t0.Add(iss);

    auto res = t0.getResult({"1", "2", "3", "4"},"0");
    for(string i : res){
        cout << i << endl;
    }


    CTrain t1;
    istringstream sis;
    sis.str("3\n4\n0\n1\n");
    t1.Add(sis);
    sis.clear();
    sis.str("2\n0");
    t1.Add(sis);
    auto res2 =  t1.getResult({"1", "2", "3"},"0");
    for(string i : res2){
        cout << i << endl;
    }



    {
        CTrain t1;
        istringstream sis;
        sis.str("1\n2\n3\n4\n");
        t1.Add(sis);
        sis.clear();
        sis.str("10\n5\n4\n");
        t1.Add(sis);
        sis.clear();
        sis.str("4\n6\n7\n8\n9\n");
        t1.Add(sis);
        sis.clear();
        auto res2 =  t1.getResult({"1", "9", "10"},"4");
        for(string i : res2){
            cout << i << endl;
        }
    }

    /*
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
    */
    return 0;
}