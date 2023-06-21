#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX

class CRangeList;

class CRange
{
  public:
    CRange(long long int lo, long long int hi){
        if(lo > hi){
            throw logic_error("");
        }
        begin = lo;
        end = hi;
    };

    friend class CRangeList;
    friend bool cmp_intervals(const CRange & first, const CRange & second);
    friend bool CMPRanges(const CRange & first, const CRange & second);
    friend ostream & operator << (ostream & out,const CRangeList & x);
    friend ostream & operator << (ostream & out,const CRange & x);

  
  private:
    long long int begin;
    long long int end;
};

bool cmp_intervals(const CRange & first, const CRange & second){
    if(first.begin < second.begin){
      return true;
    }
    return false;
}

bool CMPRanges(const CRange & first, const CRange & second){
    if((first.begin == second.begin)&&(first.end == second.end)){
        return true;
    }
    return false;
}

class CRangeList
{
  public:
    // constructor
    CRangeList(){

    };
    //!! BONUS
    CRangeList(const initializer_list<CRange> & args){
        if(args.size() == 0){
            return;
        }

        for(auto itr = args.begin();itr < args.end();itr++){
            *this += (*itr);
        }
    };
    //!!BONUS
    auto begin(){
        return all_ranges.begin();
    }
    //!!BONUS
    auto end(){
        return all_ranges.end();
    }

    // includes long long / range BUDE CONST
    bool includes(long long int input) const {
        CRange tmp(input,input);
        return includes(tmp);
    }

    bool includes(const CRange & input) const {
        
        if(all_ranges.size() == 0){
            return false;
        }

        auto itr = lower_bound(all_ranges.begin(),all_ranges.end(),input,cmp_intervals);
        //muzu dostat na zacatek a na konec
        if(itr == all_ranges.end()){
            itr--;
            if(((*itr).begin <= input.begin)&&((*itr).end >= input.end)){
                return true;
            }
            else{
                return false;
            }
        }


        //muzu dostat stejne
        if((*itr).begin == input.begin){
            if((*itr).end >= input.end){
                return true;
            }
            else{
                return false;
            }
        }

        if((itr == all_ranges.begin())&&((*itr).begin > input.begin)){
            return false;
        }


        //muzu dostat vetsi
        itr--;
        if(((*itr).begin <= input.begin)&&((*itr).end >= input.end)){
            return true;
        }
        return false;

    }

    // += range / range list

    CRangeList & operator +=(const CRange & add){ // u whilu bude moznost optimalizace

        if(includes(add) == true){
            return *this;
        }

        if(all_ranges.size()==0){
          all_ranges.push_back(add);
          return *this;
        }
        
        auto itr = lower_bound(all_ranges.begin(),all_ranges.end(),add,cmp_intervals);
        
        //pokud jsem na konci
        if(itr == all_ranges.end()){
            auto prev_itr = (itr -1);
            //pokud se prekryvaji
            if((*prev_itr).end >= (add.begin -1)){
                (*prev_itr).end = add.end;
            }

            else{
                all_ranges.push_back(add);
            }
            return *this;
        }

        //pokud jsem na zacatku
        if(itr == all_ranges.begin()){
            auto next_itr = itr +1;
            //pokud dostanu pouze min
            if(((*itr).begin == LONG_LONG_MIN)&&(add.end == LONG_LONG_MIN)){
                return *this;
            }
            //pokud mam min a prekryvaji se
            if(((*itr).begin == LONG_LONG_MIN)&&(add.begin == LONG_LONG_MIN)){
                int iterace = 0;

                while((next_itr != all_ranges.end())&&((*next_itr).begin -1 <= add.end)){
                    next_itr++;
                    iterace++;
                }
                next_itr--;
                iterace--;

                (*itr).end = (*next_itr).end;
                //kontrola zda jsem nedostal jeste vetsi hodnotu nez doted
                if((*itr).end < add.end){
                    (*itr).end = add.end;
                }
                
                while(iterace >= 0){
                  all_ranges.erase(next_itr);
                  next_itr--;
                  iterace--;
                }
                return *this;
            }      
            //pokud se prekryvaji
            if((add).end >= ((*itr).begin -1)){
                
                int iterace = 0;

                //zkontrolovat zacat a pripradne upravit
                if(add.begin < (*itr).begin){
                    (*itr).begin = add.begin;
                }
                while((next_itr != all_ranges.end())&&((*next_itr).begin -1 <= add.end)){
                    next_itr++;
                    iterace++;
                }
                next_itr--;
                iterace--;

                (*itr).end = (*next_itr).end;
                //kontrola zda jsem nedostal jeste vetsi hodnotu nez doted
                if((*itr).end < add.end){
                    (*itr).end = add.end;
                }
                
                while(iterace >= 0){
                  all_ranges.erase(next_itr);
                  next_itr--;
                  iterace--;
                }
                return *this;
            }

            else{
                all_ranges.insert(all_ranges.begin(),add);
            }
            return *this;

        }

        //pokud jsem nekde uprostred
        //muzu dostat zacatek stejne cislo ktere uz mam
        auto next_itr = itr +1;
        if((*itr).begin == add.begin){
            //pokud se prekryvaji s nasledujicim
            if((*next_itr).begin -1 <= add.end){
                int iterace = 0;
                while((next_itr != all_ranges.end())&&((*next_itr).begin -1 <= add.end)){
                    next_itr++;
                    iterace++;
                }
                next_itr--;
                iterace--;

                (*itr).end = (*next_itr).end;

                if((*itr).end < add.end){
                    (*itr).end = add.end;
                }

                while(iterace >= 0){
                  all_ranges.erase(next_itr);
                  next_itr--;
                  iterace--;
                }
                return *this;
            }
            else{
              (*itr).end = add.end;
              return *this;
            }
        }

        //pokud dostanu zacatek jinde nez cislo ktere uz mam -- vratim se o 1 (lowerbound) a aktualizuji iteratory
        itr--;
        next_itr = itr +1;

        //muze stacit prodlouzit uz muj interval pokud obsahuje muj zacatek
        if((*itr).end >= add.begin -1){

            if((*next_itr).begin -1 <= add.end){
                int iterace = 0;
                while((next_itr != all_ranges.end())&&((*next_itr).begin -1 <= add.end)){
                    next_itr++;
                    iterace++;
                }
                next_itr--;
                iterace--;

                (*itr).end = (*next_itr).end;
                if((*itr).end < add.end){
                    (*itr).end = add.end;
                }
                while(iterace >= 0){
                  all_ranges.erase(next_itr);
                  next_itr--;
                  iterace--;
                }
                return *this;
            }
            else{
              (*itr).end = add.end;
              return *this;
            }
        }
        //pokud prodluzuji interval ktery konci long long max a napojuji se na konec nejakeho
        else if(((*next_itr).begin <= add.end)&&(add.end == LONG_LONG_MAX)){
            (*next_itr).begin = add.begin;
            if((*next_itr).end >= add.end){
                return *this;
            }
            int iterace = 0;
                next_itr++;
                while((next_itr != all_ranges.end())&&((*next_itr).begin -1 <= add.end)){
                    next_itr++;
                    iterace++;
                }
                next_itr--;
                
                if((*next_itr).end < add.end){
                    (*next_itr).end = add.end;
                }
                (*next_itr).begin = add.begin;
                
                next_itr--;
                while(iterace > 0){
                  all_ranges.erase(next_itr);
                  next_itr--;
                  iterace--;
                }
                return *this;
        }

        //jeste se muzu zkusit napojit zepredu na jiny interval
        else if((*next_itr).begin <= add.end +1){
            (*next_itr).begin = add.begin;
            if((*next_itr).end >= add.end){
                return *this;
            }
            int iterace = 0;
                next_itr++;
                while((next_itr != all_ranges.end())&&((*next_itr).begin -1 <= add.end)){
                    next_itr++;
                    iterace++;
                }
                next_itr--;
                

                

                if((*next_itr).end < add.end){
                    (*next_itr).end = add.end;
                }
                (*next_itr).begin = add.begin;
                
                next_itr--;
                while(iterace > 0){
                  all_ranges.erase(next_itr);
                  next_itr--;
                  iterace--;
                }
                return *this;
        }

        //ted bych se nemel mit moznost pridat do jineho intervalu
        itr++;
        all_ranges.insert(itr,add);
        
        return *this;
    
    }

    CRangeList & operator +=(const CRangeList & add){
        for(auto itr = add.all_ranges.begin();itr < add.all_ranges.end();itr++){
            (*this) += (*itr);
        }
        return *this;
    }
    
    // -= range / range list

    CRangeList & operator -=(const CRange & subtract){

        if(all_ranges.size() == 0){
            return *this;
        }

        auto itr = lower_bound(all_ranges.begin(),all_ranges.end(),subtract,cmp_intervals);

        //muzu odstranovat ze zacatku
        if(itr == all_ranges.begin()){
            int iterace = 0;
            auto next_itr = itr;
            
            while((next_itr != all_ranges.end())&&((*next_itr).end <= subtract.end)){
                iterace++;
                next_itr++;
            }
            itr = next_itr;
            if(itr == all_ranges.end()){
                itr--;
            }
            next_itr--;
            if((*itr).begin <= subtract.end){
              (*itr).begin = subtract.end +1;
            }
            while(iterace > 0){
                auto tmp = next_itr;
                next_itr--;
                all_ranges.erase(tmp);
                iterace--;
              } 
            return *this;

        }

        //muzu odstranovat z konce
        if(itr == all_ranges.end()){
            itr--;
            if(subtract.begin > (*itr).end){
                return *this;
            }

            if(((*itr).end <= subtract.end)&&((*itr).end >= subtract.begin)){
                (*itr).end = subtract.begin -1;
                return *this; 
            }

            CRange tmp(0,0);
            tmp.begin = subtract.end +1;
            tmp.end = (*itr).end;             
            
            (*itr).end = subtract.begin -1;

            all_ranges.push_back(tmp);
            return *this;

        }

        //z prostredka
        //tedy zde pokud jsem se presne trefil na zacatek a zaroven do jednoho intervalu only
        if(((*itr).begin == subtract.begin)&&((*itr).end >= subtract.end)){
            if((*itr).end == subtract.end){
                all_ranges.erase(itr);
                return *this;
            }

            (*itr).begin = subtract.end +1;
        }

        //tedy zde pokud jsem se trefil presne na zacatek a menim vice intervalu
        if((*itr).begin == subtract.begin){
            int iterace = 0;
            auto next_itr = itr;
            
            while((next_itr != all_ranges.end())&&((*next_itr).end <= subtract.end)){
                iterace++;
                next_itr++;
            }
            itr = next_itr;
            next_itr--;
            if((*itr).begin <= subtract.end){
              (*itr).begin = subtract.end +1;
            }
            while(iterace > 0){
                auto tmp = next_itr;
                next_itr--;
                all_ranges.erase(tmp);
                iterace--;
              } 
            return *this;
        }

        //zbytek tedy netrefil jsem se na zacatek zadneho intervalu
        itr--;
        auto next_itr = itr;
        int iterace = 0;

        //pokud zustanu uvnitr jednoho intervalu
        if(((*itr).begin < subtract.begin)&&((*itr).end >= subtract.end)){
            
            if((*itr).end == subtract.end){
                (*itr).end = subtract.begin -1;
                return *this;
            }

            CRange tmp(0,0);
            tmp.begin = subtract.end +1;
            tmp.end = (*itr).end;

            (*itr).end = subtract.begin -1;
            itr++;
            all_ranges.insert(itr,tmp);
            return *this;
        }


        //pokud se budu pohybovat mezi vice intervaly
        if(((*itr).end >= subtract.begin)){
            (*itr).end = subtract.begin -1;
        }

        while((next_itr != all_ranges.end())&&((*next_itr).end <= subtract.end)){
            iterace++;
            next_itr++;
        }

        itr = next_itr;
        next_itr--;
        if(itr == all_ranges.end()){
            itr--;
        }
        if((*itr).begin <= subtract.end){
            (*itr).begin = subtract.end +1;
        }

        while(iterace > 1){
            auto tmp = next_itr;
            next_itr--;
            all_ranges.erase(tmp);
            iterace--;
        } 
        return *this;
    }

    CRangeList & operator -=(const CRangeList & subtract){
        for(auto itr = subtract.all_ranges.begin();itr < subtract.all_ranges.end();itr++){
            (*this) -= (*itr);
        }
        return *this;
        
    }

    
    // = range / range list
   CRangeList & operator  = (const CRange & in){
      CRange tmp(0,0);
      tmp.begin = in.begin;
      tmp.end = in.end;
      all_ranges.clear();
      all_ranges.push_back(tmp);
      return *this;
   } 

    // operator == BUDE CONST
    bool operator == (const CRangeList & compare) const {
        auto itr1 = all_ranges.begin();
        auto itr2 = compare.all_ranges.begin();
        if(all_ranges.size() != compare.all_ranges.size()){
          return false;
        }

        while((itr1 != all_ranges.end()) && (itr2 != compare.all_ranges.end())){
            if(CMPRanges((*itr1),(*itr2)) == false){
              return false;
            }
            itr1++;
            itr2++;
        }
        return true;
    }
    // operator != BUDE CONST
    bool operator != (const CRangeList & compare) const{
        if((*this) == compare){
            return false;
        }
        return true;
    }

    // operator << BUDE CONST

  private:
    vector<CRange> all_ranges;
    friend ostream & operator << (ostream & out,const CRangeList & x);
    friend class CRange;
    
};

ostream & operator << (ostream & out,const CRangeList & x){
    bool first = true;
    auto flag = out.flags();

    out << "{";
    out << dec;
    auto itr = x.all_ranges.begin();
    while(itr < x.all_ranges.end()){
        if(first == true){
            first = false;
        }
        else{
            out << ",";
        }
        if((*itr).begin == (*itr).end){
          out << (*itr).begin;
          itr++;
        }
        else{
          out << "<" << (*itr).begin << ".." << (*itr).end << ">";
          itr++;
        }
    }
    
    out << "}";
    out.flags(flag);
    return out;
}
//!!BONUS
ostream & operator << (ostream & out,const CRange & x){
    auto flag = out.flags();
    out << dec;
    if(x.begin == x.end){
        out << x.begin;
    }
    else{
        out << "<" << x.begin << ".." << x.end << ">";
    }
    out.flags(flag);
    return out;
}

CRangeList operator +(const CRangeList & first,const CRangeList & second){
        CRangeList res;
        res += first;
        res += second;
        return res;
    }

CRangeList operator +(const CRange  & first, const CRange & second){
    CRangeList tmp;
    tmp += first;
    tmp += second;
    return tmp;
}

CRangeList operator +(const CRangeList  & first, const CRange & second){
    CRangeList tmp;
    tmp += first;
    tmp += second;
    return tmp;
}

CRangeList operator +(const CRange  & first, const CRangeList & second){
    CRangeList tmp;
    tmp += second;
    tmp += first;
    return tmp;
}

CRangeList operator -(const CRange & first, const CRange & second){
    CRangeList tmp;
    tmp += first;
    tmp -= second;
    return tmp;
}

CRangeList operator -(const CRangeList  & first, const CRange & second){
    CRangeList tmp;
    tmp += first;
    tmp -= second;
    return tmp;
}

CRangeList operator -(const CRange  & first, const CRangeList & second){
    CRangeList tmp;
    tmp += first;
    tmp -= second;
    return tmp;
}

CRangeList operator -(const CRangeList & first,const CRangeList & second){
        CRangeList tmp;
        tmp += first;
        tmp -= second;
        return tmp;
}

#ifndef __PROGTEST__
string             toString                                ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int                main                                    ( void )
{
    long long int lmin = LONG_LONG_MIN;
    long long int lmax = LONG_LONG_MAX;
    CRangeList b;
    
    b += CRange(0,0 );
    b += CRange(1000,lmax);
    b += CRange(900,lmax);



  

#ifdef EXTENDED_SYNTAX
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  
oss << setw ( 10 ) << 1024;
assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );

#endif /* EXTENDED_SYNTAX */

  return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
