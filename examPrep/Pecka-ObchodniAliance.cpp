#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct CNode{
    int people = -1;
    string name;
    set<string> allyes;
    bool found = false;
    int group_index = -1;
};

bool operator < (const CNode & f, const CNode & s){
    return f.people < s.people;
}

class TradeAlliance {
public:
    // ? add(std::string, int)

    TradeAlliance(){
    }

    TradeAlliance(const TradeAlliance & in){
        cities = in.cities;
    }

    TradeAlliance & add(const string & s, int p){
        CNode tmp;
        tmp.people = p;
        tmp.name = s;
        cities.insert(tmp);
        return *this;
    }

    int countAllPeople() const{
        int cnt = 0;
        for(const CNode & i : cities){
            cnt += i.people;
        }
        return cnt;
    }

    set<CNode> cities;
};

bool operator < (const TradeAlliance & f, const TradeAlliance & s){
    return f.countAllPeople() > s.countAllPeople();
}

bool operator == (const TradeAlliance & f, const TradeAlliance & s){
    if(f.cities.size() != s.cities.size()){
        return false;
    }
    auto itr_f = f.cities.begin();
    auto itr_s = s.cities.begin();

    while(itr_f != f.cities.end()){
        if((itr_f->name != itr_s->name)||(itr_f->people != itr_s->people)){
            return false;
        }
        itr_f++;
        itr_s++;
    }
    return true;
}

class TradeOverview {
public:
    // update(string city, int cnt)
    void update(const string & name, int cnt){
        graf[name].people = cnt;
        graf[name].name = name;
    }
    // enterContract(string city1, string city2)
    void enterContract(const string & city1, const string & city2){
        if((graf.find(city1) == graf.end())||(graf.find(city2) == graf.end())){
            throw invalid_argument("neexistuje toto mesto");
        }

        graf[city1].allyes.insert(city2);
        graf[city2].allyes.insert(city1);
    }

    // terminateContract(string city1, string city2)
    void terminateContract(const string & city1, const string & city2){
        if((graf.find(city1) == graf.end())||(graf.find(city2) == graf.end())){
            throw invalid_argument("neexistuje toto mesto");
        }
        if(graf[city1].allyes.find(city2) == graf[city1].allyes.end()){
            throw invalid_argument("neexistuje toto mesto");
        }

        graf[city1].allyes.erase(city2);
        graf[city2].allyes.erase(city1);
    }

    void bfs(CNode * start, int flagger) const{
        queue<CNode *> que;
        start->found = true;
        start->group_index = flagger;

        que.push(start);

        while(!que.empty()){
            CNode & cur = *que.front();
            for(const string & i : cur.allyes){
                if(!graf[i].found){
                    graf[i].found = true;
                    graf[i].group_index = flagger;
                    que.push(&graf[i]);
                }
            }
            que.pop();
        }
    }

    // list<TradeAlliance> listAll()
    list<TradeAlliance> listAll() const{
        if(graf.empty()){
            return {};
        }
        //prepare for bfs
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.found = false;
            itr->second.group_index = -1;
        }

        bfs(&graf.begin()->second, 0);
        int cur_index = 1;

        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            if(itr->second.found == false){
                bfs(&itr->second, cur_index);
                cur_index++;
            }
        }

        map<int, TradeAlliance> tmp;
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            tmp[itr->second.group_index].add(itr->second.name, itr->second.people);
        }
        list<TradeAlliance> res;

        for(auto itr = tmp.begin(); itr != tmp.end(); itr++){
            res.push_back(itr->second);
        }
        res.sort([](const TradeAlliance & f, const TradeAlliance & s){
            return f.countAllPeople() > s.countAllPeople();
        });
        return res;
    }

    private:
    mutable map<string, CNode> graf; 

};

bool check(const TradeOverview& x, const std::list<TradeAlliance>& expected)
{
    auto tmp = x.listAll();
    auto itr_aliance = expected.begin();
    auto itr_trade = tmp.begin();
    if(tmp.size() != expected.size()){
        return false;
    }
    while(itr_aliance != expected.end()){
        if(!(*itr_aliance == *itr_trade)){
            return false;
        }
        itr_aliance++;
        itr_trade++;
    }
    return true;
}

int main()
{
    TradeOverview x;
    assert(check(x, (std::list<TradeAlliance>{})));

    x.update("A", 5000);
    x.update("B", 3000);
    x.update("C", 8000);
    x.update("D", 2000);
    check(x, (std::list<TradeAlliance>{TradeAlliance().add("C", 8000), TradeAlliance().add("A", 5000), TradeAlliance().add("B", 3000), TradeAlliance().add("D", 2000)}));
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("C", 8000), TradeAlliance().add("A", 5000), TradeAlliance().add("B", 3000), TradeAlliance().add("D", 2000)})));

    x.enterContract("A", "B");
    x.enterContract("C", "D");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("C", 8000).add("D", 2000), TradeAlliance().add("A", 5000).add("B", 3000)})));

    x.update("A", 12000);
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000).add("B", 3000), TradeAlliance().add("C", 8000).add("D", 2000)})));

    x.enterContract("A", "C");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000).add("C", 8000).add("B", 3000).add("D", 2000)})));

    x.enterContract("D", "B");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000).add("C", 8000).add("B", 3000).add("D", 2000)})));

    x.terminateContract("A", "B");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000).add("C", 8000).add("B", 3000).add("D", 2000)})));

    x.terminateContract("D", "C");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000).add("C", 8000), TradeAlliance().add("B", 3000).add("D", 2000)})));

    x.terminateContract("A", "C");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000), TradeAlliance().add("C", 8000), TradeAlliance().add("B", 3000).add("D", 2000)})));

    try {
        x.terminateContract("A", "C");
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument& e) {
    }
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000), TradeAlliance().add("C", 8000), TradeAlliance().add("B", 3000).add("D", 2000)})));

    x.terminateContract("B", "D");
    assert(check(x, (std::list<TradeAlliance>{TradeAlliance().add("A", 12000), TradeAlliance().add("C", 8000), TradeAlliance().add("B", 3000), TradeAlliance().add("D", 2000)})));

    try {
        x.terminateContract("B", "D");
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument& e) {
    }

    try {
        x.enterContract("A", "X");
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument& e) {
    }
}
