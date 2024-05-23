#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;
 
struct CNode{
    string name = "";
    string predecesor = "";
    int reach_len = INT32_MAX;
    set<string> neighbours;
};


class CDelivery
{
public:
	// ctor
	// dtor
 
	// add connection from -> to, fluent interface
	CDelivery & addConn ( const string & from, const string & to ){
        graf[from].name = from;
        graf[to].name = to;

        graf[from].neighbours.insert(to);
        return *this;
    }
 
	// returns a map containing shortest path to each customer from any of the depots
	// map key is the customer name, value is a list of nodes on the shortest path (depot -> customer)
	// if there is no path, the value is an empty list
	
    list<string> createPath(const string & in) const{
        list<string> res;
        CNode * cur = &graf[in];

        if(graf[in].reach_len == INT32_MAX){
            return {};
        }

        if(graf[in].reach_len == 0){
            return {cur->name};
        }

        while(cur->predecesor != ""){
            res.push_front(cur->name);
            cur = &graf[cur->predecesor];
        }
        //tohle je sklad
        res.push_front(cur->name);
        return res;
    }
    
    map<string, list<string>> serveCustomers ( const set<string> & customers, const set<string> & depots ) const{
        
        //prepare graf
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.predecesor = "";
            itr->second.reach_len = INT32_MAX;
        }
        
        queue<CNode *> que;

        for(const string & i : depots){
            graf[i].reach_len = 0;
            graf[i].predecesor = "";
            que.push(&graf[i]);
        }

        while(!que.empty()){
            CNode & cur = *que.front();
            for(const string & i : cur.neighbours){
                if(graf[i].reach_len > cur.reach_len + 1){
                    graf[i].reach_len = cur.reach_len + 1;
                    graf[i].predecesor = cur.name;
                    que.push(&graf[i]);
                }
            }
            que.pop();
        }

        map<string, list<string>> result;

        for(const string & i : customers){
            result.insert(make_pair(i, createPath(i)));
        }
        return result;
    }
 
private:
	mutable map<string, CNode> graf;
};
 
/*
CDelivery & CDelivery::addConn ( const string & from, const string & to )
{
	// todo
}
 
map<string, list<string>> CDelivery::serveCustomers ( const set<string> & customers, const set<string> & depots ) const
{
	// todo
}
*/

int main ()
{
	CDelivery t0;
	map<string, list<string>> r;
	t0.addConn("Austin", "Berlin");
	t0.addConn("Chicago", "Berlin");
	t0.addConn("Berlin", "Dallas");
	t0.addConn("Dallas", "Essen");
	t0.addConn("Essen", "Austin");
	t0.addConn("Frankfurt", "Gyor");
	t0.addConn("Gyor", "Helsinki");
	t0.addConn("Helsinki", "Frankfurt");
	r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});
	assert ( r == (map<string, list<string>>{ {"Berlin", {"Essen", "Austin", "Berlin"}}, {"Gyor", {"Helsinki", "Frankfurt", "Gyor"}} }) );
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
	t0.addConn("Chicago", "Helsinki");
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
	t0.addConn("Berlin", "Chicago");
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}} }) );
	t0.addConn("Essen", "Frankfurt");
 
	r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
	assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Dallas", "Essen", "Frankfurt", "Gyor"}} }) );
    /*
    */
	return EXIT_SUCCESS;
}