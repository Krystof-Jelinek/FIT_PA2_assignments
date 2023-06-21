#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CContestant{
    public:
    string m_name;
    vector<string> m_won;

    CContestant(){

    }

    CContestant(const string & name){
        m_name = name;
    }

    void beated(const string & name){
        auto itr = lower_bound(m_won.begin(),m_won.end(),name);
        m_won.insert(itr,name);
    }

    bool find_name(const string & name) const {
        if(m_won.empty()){
            return false;
        }
        auto itr = lower_bound(m_won.begin(),m_won.end(),name);
        if(itr == m_won.end()){
            return false;
        }
        if((*itr) == name){
          return true;
        }
        return false;
    }
};

bool compare_contestants(const CContestant & first, const CContestant & second){
    if(first.m_name < second.m_name){
        return true;
    }
    return false;
}


template<typename A>
class CFight{
    public:
      string m_first;
      string m_second;
      A m_match;

    CFight(const string & first, const string & second, const A & match):
    m_first(first), m_second(second), m_match(match) {}

    bool operator ()(const CFight & first, const CFight & second){
    if(first.m_first == second.m_first){
        if(first.m_second < second.m_second){
            return true;
        }
        return false;
    }
    if(first.m_first < second.m_first){
        return true;
    }
    return false;
}
};

class CSortclass{
  public:
    vector<CContestant> m_involved;

  CSortclass(const vector<CContestant> & src){
      m_involved = src;
  }

  bool has_beaten(const CContestant & first, const CContestant & second,set<string> & already_visited){

    if(first.find_name(second.m_name) == true){
        return true;
    }

    if(first.m_won.empty()){
        return false;
    }

    for(auto itr = first.m_won.begin();itr != first.m_won.end();itr++){
        CContestant tmp((*itr));
        auto itr2 = lower_bound(m_involved.begin(),m_involved.end(),tmp,compare_contestants);
        if(itr2 == m_involved.end()){
            return false;
        }

        if(already_visited.count((*itr2).m_name)){ // tedy smycka ?
            return false;
        }
        already_visited.insert((*itr2).m_name);
        if(has_beaten((*itr2),second,already_visited)){
            return true;
        }        
    }
    return false;    
}

  bool operator() (const CContestant & first, const CContestant & second){
        bool firstwon = false;
        bool secondwon = false;
        set<string> already_visited1;
        already_visited1.insert(first.m_name);
        
        set<string> already_visited2;
        already_visited2.insert(second.m_name);
        
        firstwon = has_beaten(first,second,already_visited1);
        secondwon = has_beaten(second,first,already_visited2);
        if(firstwon == secondwon){  //! BUDEME HAZET VYJIMKOSY
            throw std::logic_error("Imposible to sort");
        }
        if(firstwon == true){
            return true;
        }
        if(secondwon == true){
            return false;
        }
        cout << "VEEElky prusvih" << endl;
        return false;
    }

};

template <typename M_>
class CContest
{
  public:
  private:
    vector<CFight<M_>> m_matches;
  public:
    // default constructor
    CContest(){

    }
    // destructor
    ~CContest(){

    }

    // addMatch ( contestant1, contestant2, result )
    CContest & addMatch(const string & first_guy, const string & second_guy, const M_ & zapas){
        
        CFight<M_> tmp(first_guy, second_guy, zapas);
        CFight<M_> tmp_mirror(second_guy, first_guy, zapas);

        auto itr = lower_bound(m_matches.begin(), m_matches.end(),tmp,tmp);
        auto itr2 = lower_bound(m_matches.begin(), m_matches.end(),tmp_mirror,tmp);

        if((itr2 != m_matches.end()) && ((*itr2).m_first == tmp_mirror.m_first) && ((*itr2).m_second == tmp_mirror.m_second)){
            throw std::logic_error("match already exists");
            return *this;
        }

        if((itr == m_matches.end())){
            m_matches.push_back(tmp);
            return *this;
        }

        if(((*itr).m_first == tmp.m_first) && ((*itr).m_second == tmp.m_second)){
            throw std::logic_error("match already exists");
            return *this;
        }

        m_matches.insert(itr,tmp);
        return *this;
    }

    bool findPerson(const string & name, const vector<CContestant> & src) const {
        CContestant tmp(name);
        if(src.empty()){
            return false;
        }
        auto itr = lower_bound(src.begin(),src.end(),tmp,compare_contestants);
        if(itr == src.end()){
            return false;
        }
        if((*itr).m_name == name){
            return true;
        }
        return false;
    }

    vector<CContestant> are_included(function<int(const M_ &)> funkce){
      vector<CContestant> tmp;
        
        for(auto itr = m_matches.begin();itr!= m_matches.end();itr++){
            //pripadne si pridam nove osoby
            if(findPerson((*itr).m_first,tmp) != true){
                CContestant add((*itr).m_first);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),add,compare_contestants);
                tmp.insert(itr2,add);
            }
            if(findPerson((*itr).m_second,tmp) != true){
                CContestant add((*itr).m_second);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),add,compare_contestants);
                tmp.insert(itr2,add);
            }

            //zapisu vysledek zapasu
            if(funkce((*itr).m_match) > 0){
                CContestant find((*itr).m_first);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),find,compare_contestants);
                (*itr2).beated((*itr).m_second);
            }
            if(funkce((*itr).m_match) < 0){
                CContestant find((*itr).m_second);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),find,compare_contestants);
                (*itr2).beated((*itr).m_first);
            }
        }
        return tmp;
    }

    bool isOrdered (function<int(const M_ &)> funkce) const{
        //nejdriv nahazet vsechny vyhrany zapasy ke spravnym hracum
        vector<CContestant> tmp;
        
        for(auto itr = m_matches.begin();itr!= m_matches.end();itr++){
            //pripadne si pridam nove osoby
            if(findPerson((*itr).m_first,tmp) != true){
                CContestant add((*itr).m_first);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),add,compare_contestants);
                tmp.insert(itr2,add);
            }
            if(findPerson((*itr).m_second,tmp) != true){
                CContestant add((*itr).m_second);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),add,compare_contestants);
                tmp.insert(itr2,add);
            }

            //zapisu vysledek zapasu
            if(funkce((*itr).m_match) > 0){
                CContestant find((*itr).m_first);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),find,compare_contestants);
                (*itr2).beated((*itr).m_second);
            }
            if(funkce((*itr).m_match) < 0){
                CContestant find((*itr).m_second);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),find,compare_contestants);
                (*itr2).beated((*itr).m_first);
            }
        }


        //zkusit seradit vektor
        try{
            sort(tmp.begin(),tmp.end(),CSortclass(tmp));
        }
        catch(std::logic_error & error){
            return false;
        }
        return true;
    }
    // results ( comparator )
    list<string> results(function<int(const M_ &)> funkce) const {

        vector<CContestant> tmp;
        
        for(auto itr = m_matches.begin();itr!= m_matches.end();itr++){
            //pripadne si pridam nove osoby
            if(findPerson((*itr).m_first,tmp) != true){
                CContestant add((*itr).m_first);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),add,compare_contestants);
                tmp.insert(itr2,add);
            }
            if(findPerson((*itr).m_second,tmp) != true){
                CContestant add((*itr).m_second);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),add,compare_contestants);
                tmp.insert(itr2,add);
            }

            //zapisu vysledek zapasu
            if(funkce((*itr).m_match) > 0){
                CContestant find((*itr).m_first);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),find,compare_contestants);
                (*itr2).beated((*itr).m_second);
            }
            if(funkce((*itr).m_match) < 0){
                CContestant find((*itr).m_second);
                auto itr2 = lower_bound(tmp.begin(),tmp.end(),find,compare_contestants);
                (*itr2).beated((*itr).m_first);
            }
        }

        sort(tmp.begin(),tmp.end(),CSortclass(tmp));

        list<string> ret;
        for(auto itr = tmp.begin();itr != tmp.end();itr++){
            ret.push_back((*itr).m_name);
        }
        return ret;
    }
};


#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
      : m_A ( a ), 
        m_B ( b )
    {
    }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int                main                                    ( void )
{
  //prazdna instance z ?? ma by serazeno nebo ne ?

  CContest<CMatch>  z;
  //assert ( z . isOrdered ( HigherScore ) );
  
  /*z . addMatch ( "A", "B", CMatch ( 10, 3 ) );
  z . addMatch ( "B", "C", CMatch ( 10, 3 ) );
  z . addMatch ( "C", "A", CMatch ( 10, 3 ) );
  vector<CContestant> tmp = z.are_included(HigherScore);
  CSortclass trida(tmp);*/



    /*. addMatch ( "B", "C", CMatch ( 40, 0 ) );
    . addMatch ( "B", "C", CMatch ( 40, 0 ) );
    . addMatch ( "B", "C", CMatch ( 40, 0 ) );*/

  list<string> res = z . results ( HigherScore );

  /*try
  {
    list<string> res = z . results ( HigherScore );
    
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }*/

  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
