#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
  public:
    //---------------------------------------------------------------------------------------------
                             CDate                         ( int               y,
                                                             int               m,
                                                             int               d )
      : m_Year ( y ),
        m_Month ( m ),
        m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      compare                       ( const CDate     & x ) const
    {
      if ( m_Year != x . m_Year )
        return m_Year - x . m_Year;
      if ( m_Month != x . m_Month )
        return m_Month - x . m_Month;
      return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      year                          ( void ) const 
    {
      return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      month                         ( void ) const 
    {
      return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      day                           ( void ) const 
    {
      return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
      char oldFill = os . fill ();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
  private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */

bool bothspaces(char lhs, char rhs){ 
  return (lhs == rhs) && (lhs == ' '); 
}

string zleva(const string &s){
    size_t start = s.find_first_not_of(" ");
    return (start == string::npos) ? "" : s.substr(start);
}
 
string zprava(const string &s){
    size_t end = s.find_last_not_of(" ");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
 
string trim(const string &s) {
    return zprava(zleva(s));
}  

string normalazie_name(const string & source){
    string nor_name = trim(source);

    //pokradeno ze stackowerflow to fr neni metoda co by proste vzala string a dala ho na lowercase wtf ?
    transform(nor_name.begin(), nor_name.end(), nor_name.begin(),[](unsigned char c){ return tolower(c); });
        
    auto new_end = unique(nor_name.begin(), nor_name.end(), bothspaces);
    nor_name.erase(new_end, nor_name.end());
    return nor_name;
}

class CSortOpt;

class CInvoice
{
  private:
    
  CDate m_date = CDate(-1,-1,-1);
  string m_seller;
  string m_buyer;
  string m_norm_seller;
  string m_norm_buyer;
  unsigned int m_amount;
  double m_vat;
  int m_add_num;
  int m_state;

  friend bool operator == (const CInvoice & first, const CInvoice & second);
  friend bool operator != (const CInvoice & first, const CInvoice & second);
  friend class CVATRegister;
  friend bool compare_invoice(const CInvoice & first,const CInvoice & second);
  friend int compare_by_date(const CInvoice & first, const CInvoice & second,bool dir);
  friend bool ultra_compare_func(const CInvoice & first, const CInvoice & second,CSortOpt parametrs);
  friend int compare_by_buyer(const CInvoice & first, const CInvoice & second,bool dir);
  friend int compare_by_seller(const CInvoice & first, const CInvoice & second,bool dir);
  friend int compare_by_amount(const CInvoice & first, const CInvoice & second,bool dir);
  friend int compare_by_vat(const CInvoice & first, const CInvoice & second,bool dir);
  friend void printlist(const list<CInvoice> & a);
  
  public:
                             CInvoice                      ( const CDate     & date,
                                                             const string    & seller,
                                                             const string    & buyer,
                                                             unsigned int      amount,
                                                             double            vat )
    {
        m_date = CDate(date.year(),date.month(),date.day());
        m_seller = seller;
        m_buyer = buyer;
        m_amount = amount;
        m_vat = vat;
        m_add_num = 0;
        m_state = 0;
    };
    CDate                    date                          ( void ) const
    {
        return m_date;
    }
    string                   buyer                         ( void ) const
    {
        return m_buyer;
    }
    string                   seller                        ( void ) const
    {
        return m_seller;
    }
    unsigned int             amount                        ( void ) const
    {
        return m_amount;
    }
    double                   vat                           ( void ) const
    {
        return m_vat;
    }
};

//porovnani CInvoice
bool operator == (const CInvoice & first, const CInvoice & second){
    if((first.m_date.compare(second.m_date) == 0)&&
       (first.m_amount == second.m_amount)&&
       //(first.m_norm_seller == second.m_norm_seller)&&
       //(first.m_norm_buyer == second.m_norm_buyer)&&
       (first.m_seller == second.m_seller)&&
       (first.m_buyer == second.m_buyer)&&
       (first.m_vat == second.m_vat)){
       return true; 
    }
    return false;
}

bool operator != (const CInvoice & first, const CInvoice & second){
    if(first == second){
      return false;
    }
    return true;
}

class CCompany{
    public:
    int counter = 0;
    string m_normal_name;
    string m_offic_name;

    vector<CInvoice> bills;
};

struct razeni{
    int by_what = -5;
    bool direc = true;
};

class CSortOpt
{
  
  private:
    razeni all_criteria[10];
    int criteria_num;
    int current_parametr;
    friend class ultra_compare;
    friend bool ultra_compare_func(const CInvoice & first, const CInvoice & second,CSortOpt parametrs);

  public:
    static const int         BY_DATE                       = 0;
    static const int         BY_BUYER                      = 1;
    static const int         BY_SELLER                     = 2;
    static const int         BY_AMOUNT                     = 3;
    static const int         BY_VAT                        = 4;
                             CSortOpt                      ( void )
    {
        criteria_num = 0;
        current_parametr = 0;
    }
    CSortOpt               & addKey                        ( int               key,
                                                             bool              ascending = true )
    {

        all_criteria[criteria_num].by_what = key;
        all_criteria[criteria_num].direc = ascending;
        criteria_num++;
        return (*this);
    }
};

bool compare_invoice(const CInvoice & first,const CInvoice & second){
    if(first.m_norm_seller < second.m_norm_seller){
        return true;
    }
    if(first.m_norm_seller > second.m_norm_seller){
        return false;
    }

    if(first.m_norm_buyer < second.m_norm_buyer){
        return true;
    }
    if(first.m_norm_buyer > second.m_norm_buyer){
        return false;
    }

    if(first.m_amount < second.m_amount){
        return true;
    }
    if(first.m_amount > second.m_amount){
        return false;
    }

    if(first.m_vat < second.m_vat){
        return true;
    }
    if(first.m_vat > second.m_vat){
        return false;
    }

    if(first.m_date.compare(second.m_date) < 0){
        return true;
    }
    if(first.m_date.compare(second.m_date) > 0){
        return false;
    }

    return false;
}

string to_lower(const string & source ){
    string data = source;
    transform(data.begin(), data.end(), data.begin(),
    [](unsigned char c){ return std::tolower(c); });
    return data;
}

int compare_by_date(const CInvoice & first, const CInvoice & second,bool dir){
    //pokud vzestupne
    if(dir == true){
        if(first.m_date.compare(second.m_date) > 0){ //tedy prvni date je vetsi
            return -1;
        }
        if(first.m_date.compare(second.m_date) < 0){
            return 1;
        }
        return 0;
    }

    if(dir == false){
        if(first.m_date.compare(second.m_date) > 0){
            return 1;
        }
        if(first.m_date.compare(second.m_date) < 0){
            return -1;
        }
        return 0;
    }
    return -69;
}

int compare_by_buyer(const CInvoice & first, const CInvoice & second,bool dir){
    if(dir == true){
        if(to_lower(first.m_buyer) > to_lower(second.m_buyer)){
            return -1;
        }
        if(to_lower(first.m_buyer) < to_lower(second.m_buyer)){
            return 1;
        }
        return 0;
    }
    if(dir == false){
        if(to_lower(first.m_buyer) > to_lower(second.m_buyer)){
            return 1;
        }
        if(to_lower(first.m_buyer) < to_lower(second.m_buyer)){
            return -1;
        }
        return 0;
    }
    return -69;
}

int compare_by_seller(const CInvoice & first, const CInvoice & second,bool dir){
    if(dir == true){
        if(to_lower(first.m_seller) > to_lower(second.m_seller)){
            return -1;
        }
        if(to_lower(first.m_seller) < to_lower(second.m_seller)){
            return 1;
        }
        return 0;
    }
    if(dir == false){
        if(to_lower(first.m_seller) > to_lower(second.m_seller)){
            return 1;
        }
        if(to_lower(first.m_seller) < to_lower(second.m_seller)){
            return -1;
        }
        return 0;
    }
    return -69;
}

int compare_by_amount(const CInvoice & first, const CInvoice & second,bool dir){
    if(dir == true){
        if(first.m_amount > second.m_amount){
            return -1;
        }
        if(first.m_amount < second.m_amount){
            return 1;
        }
        return 0;
    }
    if(dir == false){
        if(first.m_amount > second.m_amount){
            return 1;
        }
        if(first.m_amount < second.m_amount){
            return -1;
        }
        return 0;
    }
    return -69;
}

int compare_by_vat(const CInvoice & first, const CInvoice & second,bool dir){
    if(dir == true){
        if(first.m_vat > second.m_vat){
            return -1;
        }
        if(first.m_vat < second.m_vat){
            return 1;
        }
        return 0;
    }
    if(dir == false){
        if(first.m_vat > second.m_vat){
            return 1;
        }
        if(first.m_vat < second.m_vat){
            return -1;
        }
        return 0;
    }
    return -69;
}


bool ultra_compare_func(const CInvoice & first, const CInvoice & second,CSortOpt parametrs){
    //pokud uz jsem mimo meze pole tak radim defaultne
    //if(parametrs.current_parametr)
    if(parametrs.all_criteria[parametrs.current_parametr].by_what == -5){
        if(first.m_add_num < second.m_add_num){
            return true;
        }
        return false;
    }
    if(parametrs.all_criteria[parametrs.current_parametr].by_what == 0){
            int res = compare_by_date(first,second,parametrs.all_criteria[parametrs.current_parametr].direc);
            if(res == 0){
                CSortOpt tmp;
                tmp = parametrs;
                tmp.current_parametr = tmp.current_parametr +1; 
                return ultra_compare_func(first,second,tmp);
            }
            if(res < 0){
                return false;
            }
            if(res > 0){
                return true;
            }
        }
    if(parametrs.all_criteria[parametrs.current_parametr].by_what == 1){
            int res = compare_by_buyer(first,second,parametrs.all_criteria[parametrs.current_parametr].direc);
            if(res == 0){
                CSortOpt tmp;
                tmp = parametrs;
                tmp.current_parametr = tmp.current_parametr +1; 
                return ultra_compare_func(first,second,tmp);
            }
            if(res < 0){
                return false;
            }
            if(res > 0){
                return true;
            }
        }
    if(parametrs.all_criteria[parametrs.current_parametr].by_what == 2){
            int res = compare_by_seller(first,second,parametrs.all_criteria[parametrs.current_parametr].direc);
            if(res == 0){
                CSortOpt tmp;
                tmp = parametrs;
                tmp.current_parametr = tmp.current_parametr +1; 
                return ultra_compare_func(first,second,tmp);
            }
            if(res < 0){
                return false;
            }
            if(res > 0){
                return true;
            }
        }
    if(parametrs.all_criteria[parametrs.current_parametr].by_what == 3){
            int res = compare_by_amount(first,second,parametrs.all_criteria[parametrs.current_parametr].direc);
            if(res == 0){
                CSortOpt tmp;
                tmp = parametrs;
                tmp.current_parametr = tmp.current_parametr +1; 
                return ultra_compare_func(first,second,tmp);
            }
            if(res < 0){
                return false;
            }
            if(res > 0){
                return true;
            }
        }
    if(parametrs.all_criteria[parametrs.current_parametr].by_what == 4){
            int res = compare_by_vat(first,second,parametrs.all_criteria[parametrs.current_parametr].direc);
            if(res == 0){
                CSortOpt tmp;
                tmp = parametrs;
                tmp.current_parametr = tmp.current_parametr +1; 
                return ultra_compare_func(first,second,tmp);
            }
            if(res < 0){
                return false;
            }
            if(res > 0){
                return true;
            }
        }       
        
    return false;        
}

class ultra_compare{
    public:
    CSortOpt my_csort;

    ultra_compare(const CSortOpt & src){
        my_csort = src;
    }

    
    bool operator() (const CInvoice & first, const CInvoice & second){
        return ultra_compare_func(first,second,my_csort);
    }

};

class CVATRegister
{
  private:
    map<string,CCompany> companies;
  public:
                             CVATRegister                  ( void )
    {
        //mooc hezkej konstruktor
    }
    bool                     registerCompany               ( const string    & name )
    {
        string offc_name = name;
        string nor_name = normalazie_name(name);

        CCompany add;
        add.m_offic_name = offc_name;
        add.m_normal_name = nor_name;

        if(companies.empty() == true){
            companies.insert(make_pair(nor_name,add));
            return true;
        }

        if(companies.find(nor_name) != companies.end()){
            return false;
        }

        companies.insert(make_pair(nor_name,add));
        

        return true;

        
    }
    bool                     addIssued                     ( const CInvoice  & x )
    {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_seller);
        auto itr2 = companies.find(name_buyer);

        //podminky pro pridani
        if(name_seller == name_buyer){
            return false;
        }

        if((itr1 == companies.end())||(itr2 == companies.end())){
            return false;
        }

        CInvoice add = x;
        add.m_norm_seller = name_seller;
        add.m_norm_buyer = name_buyer;
        add.m_seller = (*itr1).second.m_offic_name;
        add.m_buyer = (*itr2).second.m_offic_name;

        //zkusit najit parovou
        auto itr_in = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),add,compare_invoice);
        if(itr_in!= (*itr1).second.bills.end()){
            if(((*itr_in) == add)&&((*itr_in).m_state != 2)){
                return false;
            }
            if(((*itr_in) == add)&&((*itr_in).m_state == 2)){
                (*itr_in).m_state = 3;
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),add,compare_invoice);
                (*itr_check).m_state = 3;
                return true;
            }
        }

        add.m_add_num = (*itr1).second.counter;
        (*itr1).second.counter = (*itr1).second.counter +1;
        add.m_state = 1; 
        (*itr1).second.bills.insert(itr_in,add);

        auto in_sec = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),add,compare_invoice);
        add.m_add_num = (*itr2).second.counter;
        (*itr2).second.counter = (*itr2).second.counter +1; 

        (*itr2).second.bills.insert(in_sec,add);
        return true;
    }
    bool                     addAccepted                   ( const CInvoice  & x )
    {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_buyer);
        auto itr2 = companies.find(name_seller);

        //podminky pro pridani
        if(name_seller == name_buyer){
            return false;
        }

        if((itr1 == companies.end())||(itr2 == companies.end())){
            return false;
        }

        CInvoice add = x;
        add.m_norm_seller = name_seller;
        add.m_norm_buyer = name_buyer;
        add.m_buyer = (*itr1).second.m_offic_name;
        add.m_seller = (*itr2).second.m_offic_name;

        auto itr_in = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),add,compare_invoice);
        if(itr_in!= (*itr1).second.bills.end()){
            if(((*itr_in) == add)&&((*itr_in).m_state != 1)){
                return false;
            }
            if(((*itr_in) == add)&&((*itr_in).m_state == 1)){
                (*itr_in).m_state = 3;
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),add,compare_invoice);
                (*itr_check).m_state = 3;
                return true;
            }
        }
        add.m_add_num = (*itr1).second.counter;
        (*itr1).second.counter = (*itr1).second.counter +1;
        add.m_state = 2; 
        (*itr1).second.bills.insert(itr_in,add);

        auto in_sec = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),add,compare_invoice);
        add.m_add_num = (*itr2).second.counter;
        (*itr2).second.counter = (*itr2).second.counter +1; 

        (*itr2).second.bills.insert(in_sec,add);
        return true;

    }
    bool                     delIssued                     ( const CInvoice  & x )
    {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_seller);
        auto itr2 = companies.find(name_buyer);
        
        //pokud tam takova spolecnost neni
        if((itr1 == companies.end())||(itr2 == companies.end())){
            return false;
        }

        CInvoice unwanted = x;
        unwanted.m_norm_seller = name_seller;
        unwanted.m_norm_buyer = name_buyer;
        unwanted.m_seller = (*itr1).second.m_offic_name;
        unwanted.m_buyer = (*itr2).second.m_offic_name;

        //zkusim najit parovou
        auto itr_del = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),unwanted,compare_invoice);
        if(itr_del!= (*itr1).second.bills.end()){
            
            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 1)){
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr1).second.bills.erase(itr_del);
                (*itr2).second.bills.erase(itr_check);
                return true;
            }

            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 3)){
                
                (*itr_del).m_state = 2;
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr_check).m_state = 2;
                return true;
            }
        }
        return false;

    }
    bool                     delAccepted                   ( const CInvoice  & x )
    {
        {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_buyer);
        auto itr2 = companies.find(name_seller);
        
        //pokud tam takova spolecnost neni
        if((itr1 == companies.end())||(itr2 == companies.end())){
            return false;
        }

        CInvoice unwanted = x;
        unwanted.m_norm_seller = name_seller;
        unwanted.m_norm_buyer = name_buyer;
        unwanted.m_buyer = (*itr1).second.m_offic_name;
        unwanted.m_seller = (*itr2).second.m_offic_name;

        //zkusim najit parovou
        auto itr_del = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),unwanted,compare_invoice);
        if(itr_del!= (*itr1).second.bills.end()){
            
            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 2)){
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr1).second.bills.erase(itr_del);
                (*itr2).second.bills.erase(itr_check);
                return true;
            }

            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 3)){
                
                (*itr_del).m_state = 1;
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr_check).m_state = 1;
                return true;
            }
        }
        return false;

    }
    }
    list<CInvoice>           unmatched                     ( const string    & company,
                                                             const CSortOpt  & sortBy ) const
    {
        string name_company = normalazie_name(company);
        auto itr = companies.find(name_company);
        if(itr == companies.end()){
            list<CInvoice> empty;
            return empty;
        }

        vector<CInvoice> tmp;
        auto itr2 = (*itr).second.bills.begin();
        while(itr2 != (*itr).second.bills.end()){
            if((*itr2).m_state != 3){
                tmp.push_back(*itr2);
            }
            itr2++;
        }

        stable_sort(tmp.begin(),tmp.end(),ultra_compare(sortBy));
        
        list<CInvoice> ret(tmp.begin(), tmp.end());
        return ret;
        
    }
};

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
  auto itr1 = a.begin();
  auto itr2 = b.begin();
  for(;itr1 != a.end() && itr2 != b.end();itr1++, itr2++){
      if((*itr1) != (*itr2)){
          return false;
      }
  }
  if(itr1 == a.end() && itr2 == b.end()){
      return true;
  }
  return false;
}

void printlist(const list<CInvoice> & a){
    auto itr = a.begin();
    while(itr != a.end()){
        cout << "rok: "<<(*itr).m_date.year() << " mesic: " << (*itr).m_date.month() << " den: "<<(*itr).m_date.day();
        cout << " od: " << (*itr).m_seller << " do: " << (*itr).m_buyer << " castka: " << (*itr).m_amount << " vat: " << (*itr).m_vat << endl;
        itr++;
    }
}

int main ( void )
{
  CVATRegister NEDAVAM_PT;
  assert ( NEDAVAM_PT . registerCompany ( "afirst Company" ) );
  assert ( NEDAVAM_PT . registerCompany ( "bsecond Company" ) );
  assert ( NEDAVAM_PT . registerCompany ( "cfirst Company" ) );
  assert ( NEDAVAM_PT . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Afirst Company", "BSecond Company ", 100, 19 ) ) );
  assert ( NEDAVAM_PT . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "Bsecond Company", "Afirst Company ", 300, 20 ) ) );
  assert ( NEDAVAM_PT . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Cfirst company", "afirst Company ", 600, 21 ) ) );

  /*list<CInvoice> result = NEDAVAM_PT . unmatched ( "afirst company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ));
  printlist(result);*/

  /*assert ( equalLists ( NEDAVAM_PT . unmatched ( "afirst company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "Afirst Company", "Cfirst Company", 600, 21 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Bsecond Company", "Afirst Company", 100, 19 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "Bsecond Company", "Cfirst Company", 300, 20 ),
           } ) );*/


  CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );

  /*list<CInvoice> zabijuse = r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ));
  printlist(zabijuse);*/
  /*
    cout << endl;
    cout << endl;
    cout << endl;
  list<CInvoice> neco = r . unmatched ( "First Company", CSortOpt () );
  printlist(neco);  */

  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  /*
  */
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  /*
  */
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  
  list<CInvoice> neco = r . unmatched ( "First Company", CSortOpt () );

  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
 /*         
    */           
  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  /*         
    */

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
