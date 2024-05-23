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
    string name;
    int reach_val = INT32_MAX;
    set<string> neigbours;
};

struct Cnt{
    int counter = 0;
};


class CDelivery
{
public:
	// ctor
	// dtor
	CDelivery & addConn ( const string & a, const string & b ){
        graf[a].name = a;
        graf[a].neigbours.insert(b);
        graf[b].name = b;
        graf[b].neigbours.insert(a);
        return *this;
    }
	vector<int> findCoverage ( const set<string> & depots ) const{

        map<int, Cnt> result;
        //prep graf
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.reach_val = INT32_MAX;
        }


        queue<CNode *> que;

        int max_round = 0;

        for(const string & i : depots){
            if(graf.find(i) == graf.end()){
                throw invalid_argument("tohle mesto nemame");
            }
            graf[i].reach_val = 0;
            result[0].counter++;
            que.push(&graf[i]);
        }

        while(!que.empty()){
            CNode & cur = *que.front();
            for(const string & i : cur.neigbours){
                if(graf[i].reach_val > cur.reach_val + 1){
                    graf[i].reach_val = cur.reach_val + 1;
                    if(max_round < cur.reach_val + 1){
                        max_round = cur.reach_val + 1;
                    }
                    que.push(&graf[i]);
                    result[cur.reach_val + 1].counter++;
                }
            }
            que.pop();
        }

        for(int i = 0; i <= max_round;i++){
            result[i].counter = result[i-1].counter + result[i].counter;
        }

        vector<int> res;

        for(int i = 0; i <= max_round;i++){
            res.push_back(result[i].counter);
        }
        return res;
    }
 
private:
	mutable unordered_map<string, CNode> graf;
};
 
/*
CDelivery & CDelivery::addConn ( const string & a, const string & b )
{
	// todo
}
 
vector<int> CDelivery::findCoverage ( const set<string> & depots ) const
{
	// todo
}
*/
 
int main ()
{
	CDelivery t0;
	vector<int> r;
	t0
	.addConn("Austin", "Berlin")
	.addConn("Chicago", "Berlin")
	.addConn("Chicago", "Dallas")
	.addConn("Dallas", "Essen")
	.addConn("Essen", "Austin")
	.addConn("Frankfurt", "Essen")
	.addConn("Gyor", "Frankfurt")
	.addConn("Helsinki", "Istanbul")
	.addConn("Istanbul", "Jakarta");
 
	r = t0.findCoverage(set<string>{"Berlin"});
	assert ( r == (vector<int>{1, 3, 5, 6, 7}) );
	/* result:
	 * [0]: Berlin = 1
	 * [1]: Austin, Berlin, Chicago = 3
	 * [2]: Austin, Berlin, Chicago, Dallas, Essen = 5
	 * [3]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt = 6
	 * [4]: Austin, Berlin, Chicago, Dallas, Essen, Frankfurt, Gyor = 7
	 */
 
	r = t0.findCoverage(set<string>{"Berlin", "Essen"});
	assert ( r == (vector<int>{2, 6, 7}) );
 
	r = t0.findCoverage(set<string>{"Helsinki"});
	assert ( r == (vector<int>{1, 2, 3}) );
 
	r = t0.findCoverage(set<string>{"Istanbul"});
	assert ( r == (vector<int>{1, 3}) );
 
	r = t0.findCoverage(set<string>{"Austin", "Jakarta"});
	assert ( r == (vector<int>{2, 5, 9, 10}) );
 
	r = t0.findCoverage(set<string>{"Chicago", "Gyor", "Helsinki", "Jakarta"});
	assert ( r == (vector<int>{4, 8, 10}) );
 
	try
	{
		r = t0.findCoverage(set<string>{"Incorrect city"});
		assert ( "No invalid_argument exception caught!" == nullptr );
	}
	catch ( const invalid_argument & e ) {}
 
	return EXIT_SUCCESS;
}