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

/*
* helping functions for the normalizie name function
*/
bool bothspaces(char lhs, char rhs){ 
  return (lhs == rhs) && (lhs == ' '); 
}
// cuts all the whitespaces before the string
string from_left(const string &s){
    size_t start = s.find_first_not_of(" ");
    return (start == string::npos) ? "" : s.substr(start);
}
// cuts all the whitespaces after the string
string from_right(const string &s){
    size_t end = s.find_last_not_of(" ");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
//uses both functions to cut whitespaces from both sides of the string 
string trim(const string &s) {
    return from_right(from_left(s));
}  

/*
* This function takes company name as parametr and returns its normilized form 
* its trimms spaces from boths sides, than it transforms all characters into lowercase
* lastly it reduces all inner whitespaces to maximally one whitespace
*/
string normalazie_name(const string & source){
    string nor_name = trim(source);

    transform(nor_name.begin(), nor_name.end(), nor_name.begin(),[](unsigned char c){ return tolower(c); });
        
    auto new_end = unique(nor_name.begin(), nor_name.end(), bothspaces);
    nor_name.erase(new_end, nor_name.end());
    return nor_name;
}

/*
* class CInvoice represents one Invoice
* other than the atributes that were mandatory I added normilized names of both seller and buyer
* and i also added m_state which represents whether this specific instance of object was added like an issued or accepted invoice
* m_state is by deafult set to 0, then set to 1 if it was added by addIssued method, to num. 2 when added only by addAccepted
* when both invoices are paired i switch m_state to 3
* lastly m_add_num stores the number of CInvoice relative to when it was added into the vector of CCompany
* now i can store them ordered and search by lowerbound but i also keep track of the order that i got them in
*/
class CInvoice
{  
  // I think there is no need to have these values as private, it would only led to huge amount of friend functions or loads of getters and setterss 
  public:
    
  CDate m_date = CDate(-1,-1,-1);
  string m_seller;
  string m_buyer;
  string m_norm_seller;
  string m_norm_buyer;
  unsigned int m_amount;
  double m_vat;
  int m_add_num;
  int m_state;
  
  
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
    //i guess these getters needs to stay there so the code compiles on PT
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
    private:
};

/*
* custom functions that compare whether two instances of CInvoice are the same or not,
* it only compares the mandatory atributes so it works correcly with Progtest functions 
*/
bool operator == (const CInvoice & first, const CInvoice & second){
    if((first.m_date.compare(second.m_date) == 0)&&
       (first.m_amount == second.m_amount)&&
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

/*
* This class represents one Company
* each company has its official name and normalized name which has to be unique
* also each company stores all its Invoices into ordered bills vector
* last variable counter keeps tract of how many bills were added and this value is stored inside the Invoice as m_add_num
* this was needed because its the last conditions for sorting Invoices 
*/
class CCompany{
    public:
    int counter = 0;
    string m_normal_name;
    string m_offic_name;

    vector<CInvoice> bills;
};

//this is just a helping structure for sorting first atribute represents by which atribute should the algorithm sort and atribute direc is either ascending or descending
struct razeni{
    int by_what = -5;
    bool direc = true;
};

/*
* this class sorts the invoices by 5 different criteria
* it stores the parametrs of sorting inside all_criteria field
* criteria_num stores the number of criteria that were given by user
* current_parametr surprisingly stores the value of current parametr 
*/
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
    /*
    * this method modifies CSortOpt class and it adds one criteria how invoices should be sorted
    * it also returs the same CSortOpt object so it can be catenated together
    */
    CSortOpt               & addKey                        ( int               key,
                                                             bool              ascending = true )
    {

        all_criteria[criteria_num].by_what = key;
        all_criteria[criteria_num].direc = ascending;
        criteria_num++;
        return (*this);
    }
};

/*
 * funtions that helps searching in the CCompany bills vector
 * i needed to keep them sorted so i can search the vector in logaritmic time
 * also they could have been sorted in any other order but i choose this one 
*/
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
    // i picked this last because i would guess this is from all the above the most demanding comparison so it should be done as litle as possible
    if(first.m_date.compare(second.m_date) < 0){
        return true;
    }
    if(first.m_date.compare(second.m_date) > 0){
        return false;
    }

    return false;
}
// function that converts string into lowercase
string to_lower(const string & source ){
    string data = source;
    transform(data.begin(), data.end(), data.begin(),
    [](unsigned char c){ return std::tolower(c); });
    return data;
}

/*
* now there are 5 functions that all work almost the same way
* they are all just functions that are used in the main sorting function
* all of them compare CInvoices based on 1 atribute only and return 1 if the first one smaller then the second one
* returns 0 if they are equal based on that atribute and -1 if the second one is bigger 
*/

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
    // this should most definitely never happen but i wanned to have some alaraming value in case something horrible was happening
    return -69;
}

int compare_by_buyer(const CInvoice & first, const CInvoice & second,bool dir){
    string first_buyer = to_lower(first.m_buyer);
    string second_buyer = to_lower(second.m_buyer);
    
    if(dir == true){
        if(first_buyer > second_buyer){
            return -1;
        }
        if(first_buyer < second_buyer){
            return 1;
        }
        return 0;
    }
    if(dir == false){
        if(first_buyer > second_buyer){
            return 1;
        }
        if(first_buyer < second_buyer){
            return -1;
        }
        return 0;
    }
    return -69;
}

int compare_by_seller(const CInvoice & first, const CInvoice & second,bool dir){
    string first_seller = to_lower(first.m_seller);
    string second_seller = to_lower(second.m_seller);

    if(dir == true){
        if(first_seller > second_seller){
            return -1;
        }
        if(first_seller < second_seller){
            return 1;
        }
        return 0;
    }
    if(dir == false){
        if(first_seller > second_seller){
            return 1;
        }
        if(first_seller < second_seller){
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


/*
 * this is the main function for sorting invoices
 * it uses recursion to decide whether the first invoice should be sorted before or after the second one
 * the third parametr is an instance of CSortOpt which determines which branch of the funtion is executed
 * basicaly when two invoices are the same by one parametr it adds 1 to the the current sorting parametr and calls recusion on itself
 * so then it compares based on second parametr and so on
 * when there are no more parametrs invoice should be defaultly sorted in the same order they were added
 * thats when the recursion stops and orders them by m_add_num which represents the adding order 
*/
bool ultra_compare_func(const CInvoice & first, const CInvoice & second,CSortOpt parametrs){
    
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
/*
 * this class represents the sorting function but i basically needed to save "data" inside my funtion but without passing them as parametrs
 * i wasnt able to come up with anything better so i just made a functor from this object and stored the data inside the CSortOpt class
 * the overloaded operator for 2 CInvoice works for the std::sort funtion 
*/
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
/*
 * this class represents the whole register with all companies stored in map 
*/
class CVATRegister
{
  private:
    map<string,CCompany> companies;
  public:
                             CVATRegister                  ( void )
    {
        
    }
    /*
     * this function takes string as parametr and basically creates new company that is then added into the vector
     * first it also checks whether there already is and company with the same name and if so it returns false
     * otherwise it just adds the newly created company 
    */
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

    /*
    * this method adds new Invoice into companies and also pairs them if it was already added by method addAcepted
    */
    bool                     addIssued                     ( const CInvoice  & x )
    {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_seller);
        auto itr2 = companies.find(name_buyer);

        //conditions for adding
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

        //Now i try to find if there already is the same Invoice
        auto itr_in = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),add,compare_invoice);
        if(itr_in!= (*itr1).second.bills.end()){
            //this means that the invoice was already added by method addIssued therefore it should return false
            if(((*itr_in) == add)&&((*itr_in).m_state != 2)){
                return false;
            }
            //this would mean that the Invoice was already added by addAccepted therefore i only pair it and there is no need to add anything
            if(((*itr_in) == add)&&((*itr_in).m_state == 2)){
                (*itr_in).m_state = 3;
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),add,compare_invoice);
                (*itr_check).m_state = 3;
                return true;
            }
        }

        // this means that the invoice wasnt found before so i simply add it and set its values 
        // state = 1 means that this invoice was added by this method
        // m_add_num is the number from the company 
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

    /*
     * this method works completly the same as the method addIssued 
     * only difference being that this method sets the value of m_state to 2 which means added by this method
    */
    bool                     addAccepted                   ( const CInvoice  & x )
    {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_buyer);
        auto itr2 = companies.find(name_seller);

        //conditions
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
        
        //Now i try to find if there already is the same Invoice
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

    /*
    * this functions deletes Invoice given in parametr from the register
    */
    bool                     delIssued                     ( const CInvoice  & x )
    {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_seller);
        auto itr2 = companies.find(name_buyer);
        
        //if there is no such company
        if((itr1 == companies.end())||(itr2 == companies.end())){
            return false;
        }

        CInvoice unwanted = x;
        unwanted.m_norm_seller = name_seller;
        unwanted.m_norm_buyer = name_buyer;
        unwanted.m_seller = (*itr1).second.m_offic_name;
        unwanted.m_buyer = (*itr2).second.m_offic_name;

        //Here i try to find if there is such invoice
        auto itr_del = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),unwanted,compare_invoice);
        if(itr_del!= (*itr1).second.bills.end()){
            
            // this would mean that the invoice was added only by method addIsued so i can completly erase it
            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 1)){
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr1).second.bills.erase(itr_del);
                (*itr2).second.bills.erase(itr_check);
                return true;
            }
            /*
            *this would mean that they are already paired so i cant delete them completly i just change the state
            * if it was paired and i deleted issued "half" of the invoice there is still the accepted one left so i
            * set it to 2 which means added by addAccepted
            */
            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 3)){
                
                (*itr_del).m_state = 2;
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr_check).m_state = 2;
                return true;
            }
        }
        //this means that i didnt find such invoice so there is nothing to delete and return false
        return false;

    }
    /*
    * this method functions completly the same as delIssued with the only difference being
    * that if the invoices are already paired it changes m_state to 1 instead of 2
    * logic behind it remains unchanged
    */
    bool                     delAccepted                   ( const CInvoice  & x )
    {
        {
        string name_seller = normalazie_name(x.m_seller);
        string name_buyer = normalazie_name(x.m_buyer);
        auto itr1 = companies.find(name_buyer);
        auto itr2 = companies.find(name_seller);
        
        //If there is no such company
        if((itr1 == companies.end())||(itr2 == companies.end())){
            return false;
        }

        CInvoice unwanted = x;
        unwanted.m_norm_seller = name_seller;
        unwanted.m_norm_buyer = name_buyer;
        unwanted.m_buyer = (*itr1).second.m_offic_name;
        unwanted.m_seller = (*itr2).second.m_offic_name;

        auto itr_del = lower_bound((*itr1).second.bills.begin(),(*itr1).second.bills.end(),unwanted,compare_invoice);
        // this would mean that the invoice was added only by method addAccepted so i can completly erase it
        if(itr_del!= (*itr1).second.bills.end()){
            
            if(((*itr_del) == unwanted)&&((*itr_del).m_state == 2)){
                auto itr_check = lower_bound((*itr2).second.bills.begin(),(*itr2).second.bills.end(),unwanted,compare_invoice);
                (*itr1).second.bills.erase(itr_del);
                (*itr2).second.bills.erase(itr_check);
                return true;
            }
            
            /*
            *this would mean that they are already paired so i cant delete them completly i just change the state
            * if it was paired and i deleted accepted "half" of the invoice there is still the issued one left so i
            * set it to 1 which means added by addAccepted
            */
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
    /*
    * this method returns (in form of list) all the unpaired invoice of the company passed in parametr
    * and they are sorted by the sortby options provided by the user 
    */
    list<CInvoice>           unmatched                     ( const string    & company,
                                                             const CSortOpt  & sortBy ) const
    {
        string name_company = normalazie_name(company);
        auto itr = companies.find(name_company);
        //if there is no such company
        if(itr == companies.end()){
            list<CInvoice> empty;
            return empty;
        }
        //vector is created and all the unpaired invoices(these that dont have num 3 as their state) are pushed inside it
        vector<CInvoice> tmp;
        auto itr2 = (*itr).second.bills.begin();
        while(itr2 != (*itr).second.bills.end()){
            if((*itr2).m_state != 3){
                tmp.push_back(*itr2);
            }
            itr2++;
        }
        // now the vector is sorted by std::sort and it utilizes the recusive function for sorting
        stable_sort(tmp.begin(),tmp.end(),ultra_compare(sortBy));
        
        //now the vector is transformed into list and returned
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

//help function for debugging purposes
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
