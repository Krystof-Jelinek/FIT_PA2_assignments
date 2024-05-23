#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;
 
class Cbase {
public:
    Cbase(string sector, string species, int when) : m_Sector(std::move(sector)), m_Species(std::move(species)), m_When(when) {}
    ~Cbase() = default;
 
    string m_Sector;
    string m_Species;
    int m_When;
};
 
#endif /* __PROGTEST__ */

struct CNode{
    string m_name = "";
    string m_species = "";
    bool home_planet = false;
    bool closed_planet = false;
    int reach_year = INT32_MAX;
    set<string> neighbours;
};

 
class CUniverse {
public:
 
 
    CUniverse & addConnection(const string & from, const string & to) {
        planets[from].neighbours.insert(to);
        planets[from].m_name = from;

        planets[to].m_name = to;
        planets[to].neighbours.insert(from);
        return *this;
    }
 
    CUniverse & optimize() {
        return *this;
    }
 
    map<string, string> colonise(const vector<Cbase> & bases) const {
        
        for(auto itr = planets.begin(); itr != planets.end(); itr++){
            itr->second.closed_planet = false;
            itr->second.home_planet = false;
            itr->second.m_species = "";
            itr->second.reach_year = INT32_MAX;
        }

        int lowest_start = INT32_MAX;
        int last_start = -1;

        vector<pair<int, string>> planet_starts;

        for(const Cbase & i : bases){
            planets[i.m_Sector].m_species = i.m_Species;
            planets[i.m_Sector].reach_year = i.m_When;
            planets[i.m_Sector].home_planet = true;
            if(lowest_start > i.m_When){
                lowest_start = i.m_When;
            }
            if(last_start < i.m_When){
                last_start = i.m_When;
            }
            planet_starts.push_back(make_pair(i.m_When, i.m_Sector));
        }

        //prepare que
        queue<CNode *> que;
        for(auto itr = planet_starts.begin(); itr != planet_starts.end();){
            if(itr->first == lowest_start){
                que.push(&planets[itr->second]);
                itr = planet_starts.erase(itr);
            }
            else{
                itr++;
            }
        }

        int cycle = lowest_start;

        while(!que.empty()){
            CNode & cur = *que.front();
            que.pop();
            if(!cur.closed_planet){
            for(const string & i : cur.neighbours){
                if(planets[i].home_planet || planets[i].closed_planet){
                    continue;
                }
                if((planets[i].reach_year == cur.reach_year + 1) && (planets[i].m_species != cur.m_species)){
                    planets[i].closed_planet = true;
                    planets[i].m_species = "";
                    if(cycle < cur.reach_year + 1){
                        cycle = cur.reach_year + 1;
                    }
                }
                if(planets[i].reach_year > cur.reach_year + 1){
                    planets[i].m_species = cur.m_species;
                    planets[i].reach_year = cur.reach_year + 1;
                    que.push(&planets[i]);
                    if(cycle < cur.reach_year + 1){
                        cycle = cur.reach_year + 1;
                    }
                }
                }
            }
            for(auto itr = planet_starts.begin(); itr != planet_starts.end();){
                if(itr->first == cycle){
                    que.push(&planets[itr->second]);
                    itr = planet_starts.erase(itr);
                }
                else{
                    itr++;
                }
            }
            while(que.empty()){
                while(cycle <= last_start){
                    bool end_flag = false;
                    for(auto itr = planet_starts.begin(); itr != planet_starts.end();){
                        if(itr->first == cycle){
                            end_flag = true;
                            que.push(&planets[itr->second]);
                            itr = planet_starts.erase(itr);
                        }
                        else{
                            itr++;
                        }
                    }
                    if(end_flag){
                        break;
                    }
                    cycle++;
                }
                if(cycle > last_start){
                    break;
                }
            }
        }

        map<string, string> res;

        for(auto itr = planets.begin(); itr != planets.end(); itr++){
            if(itr->second.m_species != ""){
                res.insert(make_pair(itr->first, itr->second.m_species));
            }
        }

        return res;
    }
private:
    mutable map<string, CNode> planets;
};
 
 
#ifndef __PROGTEST__
 
int main ( void ) {
    CUniverse u1;
    
    u1 .addConnection("Earth", "X1")
       .addConnection("Earth", "Y1")
       .addConnection("Vulcan", "X1")
       .addConnection("Vulcan", "Y2")
       .addConnection("Y2", "Y1")
       .addConnection("Kronos", "X1")
       .addConnection("X1", "X2")
       .addConnection("X2", "X3")
       .optimize();
    auto r1 = u1.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 0}});
    assert(r1 == (map<string, string> ({{"Earth", "Humans"}, {"Y1", "Humans"}, {"Vulcan", "Vulcans"}, {"Y2", "Vulcans"}, {"Kronos", "Clingons"}})));
    auto r2 = u1.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Humans", 0}});
    assert(r2 == (map<string, string> ({{"Earth", "Humans"}, {"Y1", "Humans"}, {"Vulcan", "Vulcans"}, {"Y2", "Vulcans"}, {"Kronos", "Humans"}})));
    auto r3 = u1.colonise({{"Unknown", "Unknown", 0}});
    assert(r3 == (map<string, string> ({{"Unknown", "Unknown"}})));
    auto r4 = u1.colonise({});
    assert(r4 == (map<string, string>({})));
 
 
    CUniverse u2;
 
    u2 .addConnection("Earth", "Z1")
       .addConnection("Earth", "Y1")
       .addConnection("Earth", "Kronos")
       .addConnection("Earth", "Vulcan")
       .addConnection("Vulcan", "Y3")
       .addConnection("Vulcan", "X1")
       .addConnection("Kronos", "Z2")
       .addConnection("Kronos", "X4")
       .addConnection("Kronos", "Vulcan")
       .addConnection("Y1", "Y2")
       .addConnection("Y2", "Y3")
       .addConnection("Z1", "Z2")
       .addConnection("X1", "X2")
       .addConnection("X2", "X3")
       .addConnection("X1", "X3")
       .addConnection("X3", "X4")
       .addConnection("Bajor", "Cassiopea Prime")
       .optimize();
 
    auto r5 = u2.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 0}, {"Cassiopea Prime", "Cassiopeans", 0}});
    assert(r5 == (map<string, string> ({{"Earth", "Humans"}, {"Kronos", "Clingons"}, {"Vulcan", "Vulcans"}, {"Cassiopea Prime", "Cassiopeans"}, {"Bajor", "Cassiopeans"}, {"Z1", "Humans"}, {"Z2", "Clingons"}, {"Y1", "Humans"}, {"Y3", "Vulcans"}, {"X1", "Vulcans"}, {"X2", "Vulcans"}, {"X4", "Clingons"}})));
 
    //Harder tests when != 0 for all colonies
    CUniverse u3;
 
    u3 .addConnection("Earth", "Z1")
            .addConnection("Earth", "Y1")
            .addConnection("Earth", "Kronos")
            .addConnection("Earth", "Vulcan")
            .addConnection("Vulcan", "Y3")
            .addConnection("Vulcan", "X1")
            .addConnection("Kronos", "Z2")
            .addConnection("Kronos", "X4")
            .addConnection("Y1", "Y2")
            .addConnection("Y2", "Y3")
            .addConnection("Z1", "Z2")
            .addConnection("X1", "X2")
            .addConnection("X1", "X3")
            .addConnection("X2", "X3")
            .addConnection("X3", "X4")
            .addConnection("Bajor", "Cassiopea Prime")
            .optimize();
 
    auto r8 = u3.colonise({{"Earth", "Humans", 1}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 2}, {"Cassiopea Prime", "Cassiopeans", 10000}});
    assert(r8 == (map<string, string> ({{"Earth", "Humans"}, {"Kronos", "Clingons"}, {"Vulcan", "Vulcans"}, {"Y1", "Humans"}, {"Z1", "Humans"}, {"Y3", "Vulcans"}, {"Y2", "Vulcans"}, {"X1", "Vulcans"}, {"X2", "Vulcans"}, {"X3", "Vulcans"}, {"Cassiopea Prime", "Cassiopeans"}, {"Bajor", "Cassiopeans"}})));
    
    CUniverse u4;
    u4 .addConnection("Earth", "Z1")
            .addConnection("Earth", "Y1")
            .addConnection("Y1", "X1")
            .addConnection("Z1", "X1");

    auto res = u4.colonise({{"Earth", "Humans", 1}});
    assert(res == (map<string, string> ({{"Earth", "Humans"}, {"Z1", "Humans"}, {"Y1", "Humans"}, {"X1", "Humans"}})));
    
    
    /*
    */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */