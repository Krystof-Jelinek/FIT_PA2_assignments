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

string parseAndStoreNames(const std::string& namesString) {
        std::string lowercaseNames = toLowercase(namesString);
        vector<string> ret;
        size_t length = lowercaseNames.length();
        
        size_t pos = 0;
        while (pos < length) {
            while (pos < length && std::isspace(lowercaseNames[pos])) {
                ++pos;
            }
            if (pos == length) {
                break;
            }

            size_t endPos = pos;
            while (endPos < length && !std::isspace(lowercaseNames[endPos])) {
                ++endPos;
            }

            std::string name = lowercaseNames.substr(pos, endPos - pos);
            ret.push_back(name);

            pos = endPos;
        }
      sort(ret.begin(), ret.end());
      string normalized_name;
      for( string & name : ret){
        normalized_name += name + " ";
      }
      return normalized_name;
  }

vector<string> parseAndStoreNames_intoVector(const std::string& namesString) {
        std::string lowercaseNames = toLowercase(namesString);
        vector<string> ret;
        size_t length = lowercaseNames.length();
        
        size_t pos = 0;
        while (pos < length) {
            while (pos < length && std::isspace(lowercaseNames[pos])) {
                ++pos;
            }
            if (pos == length) {
                break;
            }

            size_t endPos = pos;
            while (endPos < length && !std::isspace(lowercaseNames[endPos])) {
                ++endPos;
            }

            std::string name = lowercaseNames.substr(pos, endPos - pos);
            ret.push_back(name + " ");

            pos = endPos;
        }
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
        size_t lenght = lowercaseNames.length();
        vector<string> all_names;

        size_t pos = 0;
        while (pos < lenght) {
            while (pos < lenght && std::isspace(lowercaseNames[pos])) {
                ++pos;
            }
            if (pos == lenght) {
                break;
            }

            size_t endPos = pos;
            while (endPos < lenght && !std::isspace(lowercaseNames[endPos])) {
                ++endPos;
            }

            std::string name = lowercaseNames.substr(pos, endPos - pos);
            all_names.push_back(name);

            pos = endPos;
        }
      sort(all_names.begin(), all_names.end());
      for( string & name : all_names){
        normalized_name += name + " ";
      }
  }

    bool                     operator ==                   ( const CStudent  & other ) const{
      if((this->original_name == other.original_name)
     &&(equalDate(this->m_birth_date, other.m_birth_date))
     &&(this->enroll_year == other.enroll_year)){
      return true;
     }
    return false;
    }
    bool                     operator !=                   ( const CStudent  & other ) const{
      return !((*this) == other);
    }

    bool  operator < (const CStudent & other) const{
    if(enroll_year != other.enroll_year){
      return enroll_year < other.enroll_year;
    }
    if(!equalDate(m_birth_date, other.m_birth_date)){
      return smallerDate(m_birth_date, other.m_birth_date);
    }
    return original_name < other.original_name;
  }


  string original_name;
  string normalized_name;
  CDate m_birth_date{0,0,0};
  int enroll_year;

  long long int index;
  private:
    // todo
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
      string tmp = parseAndStoreNames(name);
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
    vector<string> names;
  private:
    // todo
};

struct sorting_key{
  int sort_index = -1;
  bool asceding = true;
};

class CSort
{
  public:
                             CSort                         ()
    {
      current = 0;
    }
    CSort                  & addKey                        ( ESortKey          key,
                                                             bool              ascending )
    {
      if(keys.size() >= 3){
        return *this;
      }
      
      sorting_key tmp;
      tmp.asceding = ascending;
      tmp.sort_index = static_cast<int>(key);

      keys.push_back(tmp);
      return *this;
    }
    
    vector<sorting_key> keys;
    int current;

  private:
    
};

int compare_by_index(const CStudent & first, const CStudent & second, bool ascending){
  if(ascending){
    if(first.index > second.index){return -1;}
    if(first.index < second.index){return 1;}
    return 0;
  }
  else{
    if(first.index > second.index){return 1;}
    if(first.index < second.index){return -1;}
    return 0;
  }
}

int compare_by_enroll(const CStudent & first, const CStudent & second, bool ascending){
  if(ascending){
    if(first.enroll_year > second.enroll_year){return -1;}
    if(first.enroll_year < second.enroll_year){return 1;}
    return 0;
  }
  else{
    if(first.enroll_year > second.enroll_year){return 1;}
    if(first.enroll_year < second.enroll_year){return -1;}
    return 0;
  }
}

int compare_by_birth(const CStudent & first, const CStudent & second, bool ascending){
  if(ascending){
    if(first.m_birth_date > second.m_birth_date){return -1;}
    if(first.m_birth_date < second.m_birth_date){return 1;}
    return 0;
  }
  else{
    if(first.m_birth_date > second.m_birth_date){return 1;}
    if(first.m_birth_date < second.m_birth_date){return -1;}
    return 0;
  }
}

int compare_by_name(const CStudent & first, const CStudent & second, bool ascending){
  if(ascending){
    if(first.original_name > second.original_name){return -1;}
    if(first.original_name < second.original_name){return 1;}
    return 0;
  }
  else{
    if(first.original_name > second.original_name){return 1;}
    if(first.original_name < second.original_name){return -1;}
    return 0;
  }
}

bool ultra_compare_func(const CStudent & first, const CStudent & second, CSort parametrs){
  int cur = parametrs.current;
  if(int(parametrs.keys.size()) <= parametrs.current){
    return compare_by_index(first,second, true) > 0;
  }
  if(parametrs.keys[cur].sort_index == 0){
    int res = compare_by_name(first,second,parametrs.keys[cur].asceding);
    if(res == 0){
      CSort tmp;
      tmp = parametrs;
      tmp.current++;
      return ultra_compare_func(first,second,tmp);
    }
    return res > 0;
  }
  if(parametrs.keys[cur].sort_index == 1){
    int res = compare_by_birth(first,second,parametrs.keys[cur].asceding);
    if(res == 0){
      CSort tmp;
      tmp = parametrs;
      tmp.current++;
      return ultra_compare_func(first,second,tmp);
    }
    return res > 0;
  }
  if(parametrs.keys[cur].sort_index == 2){
    int res = compare_by_enroll(first,second,parametrs.keys[cur].asceding);
    if(res == 0){
      CSort tmp;
      tmp = parametrs;
      tmp.current++;
      return ultra_compare_func(first,second,tmp);
    }
    return res > 0;
  }
  throw std::runtime_error("Error in ultracompare function");
}

class ultra_compare{
  public:
  CSort m_csort;
  ultra_compare(const CSort & in){
    m_csort = in;
  }
  bool operator() (const CStudent & first, const CStudent & second){
    return ultra_compare_func(first,second,m_csort);
  }
};

class CStudyDept 
{
  public:
    CStudyDept()
    {
      indexing_sequence = 0;
    }
    bool                     addStudent                    ( const CStudent  & x ){
      if(all_students.find(x) != all_students.end()){
        return false;
      }

      CStudent in = x;
      in.index = indexing_sequence;
      indexing_sequence++;

      all_students.insert(in);
      return true;
    }
    bool                     delStudent                    ( const CStudent  & x ){
      if(all_students.find(x) == all_students.end()){
        return false;
      }

      all_students.erase(x);
      return true;
    }
    
    std::list<CStudent>      search                        ( const CFilter   & flt,
                                                             const CSort     & sortOpt ) const
    {
      list<CStudent> res;
      if((flt.enrolled_after >= flt.enrolled_before)||
         (flt.m_born_after >= flt.m_born_before))
      {
        return res;
      }
      CStudent enrollStudent("", CDate{0,0,0}, flt.enrolled_before);
      auto end_itr = lower_bound(all_students.begin(), all_students.end(), enrollStudent);
      enrollStudent.enroll_year = flt.enrolled_after;
      auto begin_itr = upper_bound(all_students.begin(), all_students.end(), enrollStudent);
      while((begin_itr != all_students.end()) &&(begin_itr->enroll_year == flt.enrolled_after)){
        begin_itr++;
      }

      if(flt.names.empty() == false){
        while(begin_itr != end_itr){
          if((begin_itr->m_birth_date >= flt.m_born_before)||(begin_itr->m_birth_date <= flt.m_born_after)){
            begin_itr++;
            continue;
          }
          for(const string & name : flt.names){
            if(begin_itr->normalized_name == name){
              res.push_back(*begin_itr);
              break;
            }
          }
          begin_itr++;
        }
      }
      else{
        while(begin_itr != end_itr){
          if((begin_itr->m_birth_date >= flt.m_born_before)||(begin_itr->m_birth_date <= flt.m_born_after)){
            begin_itr++;
            continue;
          }
          res.push_back(*begin_itr);
          begin_itr++;
        }
      }
      res.sort(ultra_compare(sortOpt));
      return res;
    }

    std::set<std::string>    suggest                       ( const std::string & name ) const{
      //this should remove duplicates from the string
      vector<string> searching_names = parseAndStoreNames_intoVector(name);
      set<string> uniqueElements(searching_names.begin(), searching_names.end());
      searching_names = vector<string>(uniqueElements.begin(), uniqueElements.end());

      set<string> ret;
      bool flag = false;

      for(const CStudent & student : all_students){
        flag = true;
        for(string & name : searching_names){
          if(student.normalized_name.find(name) == string::npos){
            flag = false;
            break;
          }
        }
        if(flag){
          ret.insert(student.original_name);
        }
      }
      
      return ret;
  }
  
  long long int indexing_sequence;

  std::set<CStudent> all_students;
  
  private:
    // todo
};
#ifndef __PROGTEST__

int main ( void )
{

  CStudyDept x0;
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
  x0 . search ( CFilter (), CSort ());
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
  x0.suggest("peter");
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

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
