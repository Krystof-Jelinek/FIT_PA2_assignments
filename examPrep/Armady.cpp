//Jedná se o přepis zkouškového testu dle paměti. Asserty jsem vytvářel sám, podle vzpomínek a možných záludností.
//Pokud v assertech objevíte chybu, dejte prosím vědět do komentáře.
//S hlavičkovými soubory si nejsem úplně jistý, nicméně vím, že zadání umožňovalo použít vector, mapu, množinu, spoják, a pair.
 
#ifndef __PROGTEST__
 
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <algorithm>
#include <utility>
using namespace std;
#endif /* __PROGTEST__ */
 
class CArmyNotSpecified {};
class CSamePact {};
 
struct CNode{
    int wariors = -1;
    string name;
    set<string> allyes;
    bool opened = false;

};

class CArmy
{
public:
  //default constructor
    CArmy(){}
 
  void Update(const string &name, int cnt)
  {
    graf[name].name = name;
    graf[name].wariors = cnt;
  }
 
    int returnOnesArmy(const string & name) const{
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.opened = false;
        }
        return returnOnesArmyRec(name);
    }      

    int returnOnesArmyRec(const string & name) const{
        if(graf.find(name) == graf.end()){
            throw CArmyNotSpecified();
        }
        if(graf[name].wariors == -1){
            throw CArmyNotSpecified();
        }
        int counter = graf[name].wariors;
        graf[name].opened = true;
        for(const string & i : graf[name].allyes){
            if(graf[i].opened == false){
                graf[i].opened = true;
                counter += returnOnesArmyRec(i);
            }
        }
        return counter;
    }

    void bfs(const string & name) const{
        for(auto itr = graf.begin(); itr != graf.end(); itr++){
            itr->second.opened = false;
        }
        deque<CNode *> que;
        graf[name].opened = true;
        que.push_back(&graf[name]);

        while(!que.empty()){
            CNode & cur = *que.front();
            for(const string & i : cur.allyes){
                if(!graf[i].opened){
                    graf[i].opened = true;
                    que.push_back(&graf[i]);
                }
            }
            que.pop_back();
        }
    }

    bool areFriends(const string &name1, const string &name2) const{
        bfs(name1);
        if(graf[name2].opened){
            throw CSamePact();
        }
        return false;
    }

  void Treat(const string &name1, const string &name2, bool status)
  {
    if(status){
        graf[name1].allyes.insert(name2);
        graf[name2].allyes.insert(name1);
    }
    else{
        graf[name1].allyes.erase(name2);
        graf[name2].allyes.erase(name1);
    }
  }
 
  int Compare(const string &name1, const string &name2) const
  {
    areFriends(name1, name2);
    return returnOnesArmy(name1) - returnOnesArmy(name2);
  }
 
private:
  mutable map<string, CNode> graf; 
};
 
#ifndef __PROGTEST__
int main()
{
  CArmy x;
  x.Update("A", 1500);
  x.Update("B", 1500);
  x.Update("C", 2000);
  x.Update("D", 1000);
 
  cout << x.Compare("A", "B") << endl; //0
  cout << x.Compare("C", "D") << endl; //1000
 
  x.Treat("A", "B", true);
  x.Treat("C", "D", true);
 
  cout << x.Compare("A", "C") << endl; //0
 
  try { x.Compare("A", "E"); }
  catch(const CArmyNotSpecified &err) { cout << "CArmyNotSpecified" << endl; }
 
  x.Update("E", 100);
  x.Treat("E", "B", true);
 
  try { x.Compare("A", "E"); }
  catch(const CSamePact &err) { cout << "CSamePact" << endl; }
 
  x.Update("F", 2500);
  x.Treat("A", "F", true);
 
  cout << x.Compare("A", "C") << endl; //2600
 
  x.Treat("A", "B", false);
  cout << x.Compare("A", "B") << endl; //2400
 
  x.Treat("A", "B", true);
  x.Treat("A", "B", true);
 
  try { x.Compare("A", "B"); }
  catch(const CSamePact &err) { cout << "CSamePact" << endl; }
 
  x.Treat("B", "F", true);
  x.Treat("D", "G", true);
  try { x.Compare("F", "C"); cout << "jebe" << endl;}
  catch(const CArmyNotSpecified &err) { cout << "CArmyNotSpecified" << endl; }
 
  x.Update("G", 3300);
  cout << x.Compare("F", "C") << endl; //-700
 
  x.Treat("A", "B", false);
  x.Treat("F", "A", false);
  x.Update("A", 340);
  cout << x.Compare("A", "B") << endl; // -3760
}
#endif /* __PROGTEST__ */