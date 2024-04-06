#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate 
{
  public:
                             CDate                         ( int               y,
                                                             int               m,
                                                             int               d )
      : m_Y ( y ),
        m_M ( m ),
        m_D ( d )
    {
    }
    
    std::strong_ordering     operator <=>                  ( const CDate     & other ) const = default;
    
    friend std::ostream    & operator <<                   ( std::ostream    & os,
                                                             const CDate     & d )
    {
      return os << d . m_Y << '-' << d . m_M << '-' << d . m_D;
    }
    
  private:
    int                      m_Y;
    int                      m_M;
    int                      m_D;
};

enum class ESortKey
{
  NAME,
  BIRTH_DATE,
  ENROLL_YEAR
};

#endif /* __PROGTEST__ */

using namespace std;

string toLowercase(const string& str) {
    string result = str;

    transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return result;
}

vector<string> parseAndStoreNames(const std::string& namesString) {
        std::string lowercaseNames = toLowercase(namesString);
        vector<string> ret;
        
        size_t pos = 0;
        while (pos < lowercaseNames.length()) {
            while (pos < lowercaseNames.length() && std::isspace(lowercaseNames[pos])) {
                ++pos;
            }
            if (pos == lowercaseNames.length()) {
                break;
            }

            size_t endPos = pos;
            while (endPos < lowercaseNames.length() && !std::isspace(lowercaseNames[endPos])) {
                ++endPos;
            }

            std::string name = lowercaseNames.substr(pos, endPos - pos);
            ret.push_back(name);

            pos = endPos;
        }
      sort(ret.begin(), ret.end());
      return ret;
  }

bool equalDate(const CDate & first, const CDate & second){
  return first == second;
}

bool smallerDate(const CDate & first, const CDate & second){
  return first < second;
}

class CStudent
{
  public:

  CStudent(){
    original_name = "";
    enroll_year = 0;
    all_names.clear();
    index = -1;
  } 

  CStudent(const std::string & name, const CDate & born, int enrolled){
    original_name = name;
    m_birth_date = born;
    enroll_year = enrolled;
    parseAndStoreNames(name);
    index = -1;
  }

  void parseAndStoreNames(const std::string& namesString) {
        std::string lowercaseNames = toLowercase(namesString);

        size_t pos = 0;
        while (pos < lowercaseNames.length()) {
            while (pos < lowercaseNames.length() && std::isspace(lowercaseNames[pos])) {
                ++pos;
            }
            if (pos == lowercaseNames.length()) {
                break;
            }

            size_t endPos = pos;
            while (endPos < lowercaseNames.length() && !std::isspace(lowercaseNames[endPos])) {
                ++endPos;
            }

            std::string name = lowercaseNames.substr(pos, endPos - pos);
            all_names.push_back(name);

            pos = endPos;
        }
      sort(all_names.begin(), all_names.end());
  }

  bool operator ==(const CStudent & other) const{
    if((this->original_name == other.original_name)
     &&(equalDate(this->m_birth_date, other.m_birth_date))
     &&(this->enroll_year == other.enroll_year)){
      return true;
     }
    return false;
  }
  
  bool operator !=(const CStudent & other) const{
    return !((*this) == other);
  }
  
  bool  operator < (const CStudent & other) const{
    if(original_name != other.original_name){
      return original_name < other.original_name;
    }
    if(!equalDate(m_birth_date, other.m_birth_date)){
      return smallerDate(m_birth_date, other.m_birth_date);
    }
    return enroll_year < other.enroll_year;
  }


  string original_name;
  CDate m_birth_date{0,0,0};
  int enroll_year;

  vector<string> all_names;
  long long int index;
  
  private:
    // todo
};

struct cmpNames{
  bool operator() (const CStudent & s1, const CStudent & s2) const{
    if(s1.all_names == s2.all_names){
      return s1 < s2;
    }
    return s1.all_names < s2.all_names;
  }
};

struct cmpBirthDates{
  bool operator() (const CStudent & s1, const CStudent & s2) const{
    if(equalDate(s1.m_birth_date,s2.m_birth_date)){
      return s1 < s2;
    }
    return smallerDate(s1.m_birth_date,s2.m_birth_date);
  }
};

struct cmpEnrollDates{
  bool operator() (const CStudent & s1, const CStudent & s2) const{
    if(s1.enroll_year == s2.enroll_year){
      return s1 < s2;
    }
    return s1.enroll_year < s2.enroll_year;
  }
};

class CFilter 
{
  public:
                             CFilter                       ()
    {
      enrolled_before = INT_MAX;
      enrolled_after = INT_MIN;
      names.clear();
    }
    CFilter                & name                          ( const std::string & name ){
      vector<string> tmp = parseAndStoreNames(name);
      names.push_back(tmp);
      return *this;
    }
    CFilter                & bornBefore                    ( const CDate     & date ){
      if(smallerDate(m_born_before, date)){
        return *this;
      }
      m_born_before = date;
      return *this;
    }
    CFilter                & bornAfter                     ( const CDate     & date ){
      if(smallerDate(date, m_born_after)){
        return *this;
      }
      m_born_after = date;
      return *this;
    }
    CFilter                & enrolledBefore                ( int               year ){
      if(enrolled_before < year){
        return *this;
      }
      enrolled_before = year;
      return *this;
    }
    CFilter                & enrolledAfter                 ( int               year ){
      if(enrolled_after > year){
        return *this;
      }
      enrolled_after = year;
      return *this;
    }
  
    CDate m_born_before{INT_MAX,INT_MAX,INT_MAX};
    CDate m_born_after{INT_MIN,INT_MIN,INT_MIN};
    int enrolled_before;
    int enrolled_after;
    vector<vector<string>> names;
  
  private:
    // todo
};

class CSort
{
  public:
                             CSort                         ();
    CSort                  & addKey                        ( ESortKey          key,
                                                             bool              ascending );
    
  private:
    // todo
};

class CStudyDept 
{
  public:
    CStudyDept()
    {
      indexing_sequence = 0;
    }
                        
    bool                     addStudent                    ( const CStudent  & x ){
      if(name_set.find(x) != name_set.end()){
        return false;
      }

      CStudent in = x;
      in.index = indexing_sequence;
      indexing_sequence++;

      name_set.insert(in);
      birth_set.insert(in);
      enroll_set.insert(in);

      return true;
    }

    bool                     delStudent                    ( const CStudent  & x ){
      if(name_set.find(x) == name_set.end()){
        return false;
      }

      name_set.erase(x);
      birth_set.erase(x);
      enroll_set.erase(x);

      return true;
    }
    
    std::list<CStudent>      search                        ( const CFilter   & flt,
                                                             const CSort     & sortOpt ) const;
    std::set<std::string>    suggest                       ( const std::string & name ) const;

    long long int indexing_sequence;

    std::set<CStudent,cmpNames> name_set;
    std::set<CStudent,cmpBirthDates> birth_set;
    std::set<CStudent,cmpEnrollDates> enroll_set;
};
#ifndef __PROGTEST__
int main ( void )
{
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) ) );
  assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) );
  assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) ) );
  
  CStudent a("CCC     AAA       bbBbbB aAAA Ac", CDate{2000,20,20}, 2000);
  CStudent b("CCC     AAA       bbBbbB aAAA Acc", CDate{1900,20,20}, 2001);
  CStudent c("CCC     AAA       bbBbbB aAAA Ac", CDate{1900,20,19}, 2001);

  CStudyDept x;
  assert ( x . addStudent ( CStudent ( "AAA BB CC", CDate ( 2000, 2, 2), 2000 ) ) );
  assert ( x . addStudent ( CStudent ( "AAA AA CC", CDate ( 1900, 1, 1), 2002 ) ) );
  assert ( x . addStudent ( CStudent ( "BB BB CC", CDate ( 2000, 1, 2), 1999 ) ) );
  assert ( x . addStudent ( CStudent ( "BB BB CC", CDate ( 2000, 1, 2), 1999 ) ) == false);
  assert ( x . addStudent ( CStudent ( "AAA AA CC", CDate ( 1900, 1, 1), 2002 ) ) == false);
  assert ( x . delStudent ( CStudent ( "AAA AA CC", CDate ( 1900, 1, 1), 2002 ) ));
  assert ( x . delStudent ( CStudent ( "BB BB CC", CDate ( 2000, 1, 2), 1999 ) ) );
  assert ( x . delStudent ( CStudent ( "BB BB CC", CDate ( 2000, 1, 2), 1999 ) ) == false);

  CFilter y;
  y.bornAfter(CDate{2000,1,1});
  y.bornAfter(CDate{1900,1,1});
  y.bornAfter(CDate{2000,1,2});
  y.bornBefore(CDate{2000,1,2});
  y.bornBefore(CDate{2000,1,3});
  y.bornBefore(CDate{1999,1,2});
  y.enrolledAfter(2000);
  y.enrolledAfter(1999);
  y.enrolledAfter(1999).enrolledAfter(2001);
  y.enrolledBefore(2000);
  y.enrolledBefore(2005);
  y.enrolledBefore(1900);

  y.name("James     BoND");
   y.name("James     BoND JamMEEES BOUaAND");

  /*
  CStudyDept x0;
  assert ( x0 . addStudent ( CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ) ) );
  assert ( x0 . addStudent ( CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ) ) );
  assert ( x0 . addStudent ( CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ) ) );
  assert ( x0 . addStudent ( CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ) ) );
  assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ) ) );
  assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ) ) );
  assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ) ) );
  assert ( x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ) ) );
  assert ( x0 . addStudent ( CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ) ) );
  assert ( x0 . search ( CFilter (), CSort () ) == (std::list<CStudent>
  {
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
  }) );
  assert ( x0 . search ( CFilter (), CSort () . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
  {
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
  }) );
  assert ( x0 . search ( CFilter (), CSort () . addKey ( ESortKey::NAME, false ) ) == (std::list<CStudent>
  {
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
  }) );
  assert ( x0 . search ( CFilter (), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
  {
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
  }) );
  assert ( x0 . search ( CFilter () . name ( "james bond" ), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
  {
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 )
  }) );
  assert ( x0 . search ( CFilter () . bornAfter ( CDate ( 1980, 4, 11) ) . bornBefore ( CDate ( 1983, 7, 13) ) . name ( "John Taylor" ) . name ( "james BOND" ), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
  {
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
  }) );
  assert ( x0 . search ( CFilter () . name ( "james" ), CSort () . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
  {
  }) );
  assert ( x0 . suggest ( "peter" ) == (std::set<std::string>
  {
    "John Peter Taylor",
    "Peter John Taylor",
    "Peter Taylor"
  }) );
  assert ( x0 . suggest ( "bond" ) == (std::set<std::string>
  {
    "Bond James",
    "James Bond"
  }) );
  assert ( x0 . suggest ( "peter joHn" ) == (std::set<std::string>
  {
    "John Peter Taylor",
    "Peter John Taylor"
  }) );
  assert ( x0 . suggest ( "peter joHn bond" ) == (std::set<std::string>
  {
  }) );
  assert ( x0 . suggest ( "pete" ) == (std::set<std::string>
  {
  }) );
  assert ( x0 . suggest ( "peter joHn PETER" ) == (std::set<std::string>
  {
    "John Peter Taylor",
    "Peter John Taylor"
  }) );
  assert ( ! x0 . addStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  assert ( x0 . delStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  assert ( x0 . search ( CFilter () . bornAfter ( CDate ( 1980, 4, 11) ) . bornBefore ( CDate ( 1983, 7, 13) ) . name ( "John Taylor" ) . name ( "james BOND" ), CSort () . addKey ( ESortKey::ENROLL_YEAR, false ) . addKey ( ESortKey::BIRTH_DATE, false ) . addKey ( ESortKey::NAME, true ) ) == (std::list<CStudent>
  {
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
  }) );
  assert ( ! x0 . delStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  */
  

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
