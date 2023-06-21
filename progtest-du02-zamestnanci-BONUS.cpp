#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

struct vyplaty{
    unsigned int salary;
    int mnozstvi;
};

struct duo{
    string name;
    string surname;
    string email;
};

struct employee{
    string name;
    string surname;
    string email;
    int salary;
};

bool compare_names(const employee & first,const employee & second){
    int srovnani = first.surname.compare(second.surname); 
    if(srovnani == 0){
      srovnani = first.name.compare(second.name);
      if(srovnani < 0){
        return true;
      }
      return false;
    }
    if(srovnani < 0){
      return true;
    }
    if(srovnani > 0){
      return false;
    }
    cout << "prusvih se stal" << endl;
    return true;
}

bool compare_mails(const duo & first, const duo & second){
    int srovnani = first.email.compare(second.email);
    if(srovnani < 0){
      return true;
    }
    return false;
}

bool compare_vyplaty(const vyplaty & first, const vyplaty & second){
    if(first.salary < second.salary){
      return true;
    }
    return false;
}

class CPersonalAgenda
{
  private:
  
    vector<employee> all_emp;
    vector<duo> match_table;
    vector<vyplaty> salary_table;
  
  public:
                  CPersonalAgenda  ( void ){

                  };
                  ~CPersonalAgenda ( void ){

                  };

    bool checkbeforeadd(employee adding) const
    {
        if(all_emp.size()==0){
            return true;
        }
        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),adding,compare_names);
        
        if((itr_emp != all_emp.end())){

          if(((*itr_emp).name == adding.name)&&((*itr_emp).surname == adding.surname)){
              return false;
          }

        }

        duo tmpduo;
        tmpduo.email = adding.email;

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);

        if(itr_duo == match_table.end()){
            return true;
        }

        if((*itr_duo).email == adding.email){            
            return false;
        }

        return true;
    }

    bool nameexists(const employee & checking){
        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),checking,compare_names);

        if(itr_emp == all_emp.end()){
            return false;
        }
        
        if(((*itr_emp).name == checking.name)&&((*itr_emp).surname == checking.surname)){
            return true;
        }

        return false;
    }

    bool          add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary )
    {
        employee tmp_emp;
        tmp_emp.name = name;
        tmp_emp.surname = surname;
        tmp_emp.email = email;
        tmp_emp.salary = salary;

        duo tmpduo;
        tmpduo.email = email;
        tmpduo.name = name;
        tmpduo.surname = surname;
        
        vyplaty tmpvyplata;
        tmpvyplata.salary = salary;

        if(checkbeforeadd(tmp_emp) == false){
          return false;
        }

        if(all_emp.size() == 0){
            all_emp.push_back(tmp_emp);
            match_table.push_back(tmpduo);
            
            if(salary_table.size() == 0){
                tmpvyplata.salary = salary;
                tmpvyplata.mnozstvi = 1;
                salary_table.push_back(tmpvyplata);
            }
            return true;
        }

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);
        all_emp.insert(itr_emp,tmp_emp);

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);
        match_table.insert(itr_duo,tmpduo);


        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

        if((itr_vypl != salary_table.end())){
            if((*itr_vypl).salary == salary){
                (*itr_vypl).mnozstvi++;
            }
            else{
                tmpvyplata.mnozstvi = 1;
                salary_table.insert(itr_vypl,tmpvyplata);
            }
        }
        else{
            tmpvyplata.mnozstvi = 1;
            salary_table.insert(itr_vypl,tmpvyplata);
        }

      return true;
    }
    bool          del              ( const string    & name,
                                     const string    & surname )
    {
        employee tmp_emp;
        tmp_emp.name = name;
        tmp_emp.surname = surname;

        duo tmpduo;

        vyplaty tmpvyplata;

        if(nameexists(tmp_emp) == false){
            return false;
        }

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);
        tmpduo.email = (*itr_emp).email;
        
        tmpvyplata.salary = (*itr_emp).salary;

        all_emp.erase(itr_emp);
        
        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);
        match_table.erase(itr_duo);

        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);
        
        if((*itr_vypl).mnozstvi > 1){
            (*itr_vypl).mnozstvi--;
            return true;
        }

        if((*itr_vypl).mnozstvi == 1){
            salary_table.erase(itr_vypl);
        }
        

        return true;

    }
    bool          del              ( const string    & email )
    {
        employee tmp_emp;

        duo tmpduo;
        tmpduo.email = email;

        vyplaty tmpvyplata;

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);
        
        if(itr_duo == match_table.end()){
            return false;
        }

        if((*itr_duo).email == email){
            tmp_emp.name = (*itr_duo).name;
            tmp_emp.surname = (*itr_duo).surname;
            match_table.erase(itr_duo);

            auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);
            
            tmpvyplata.salary = (*itr_emp).salary;

            all_emp.erase(itr_emp);

            auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);
        
            if((*itr_vypl).mnozstvi > 1){
                (*itr_vypl).mnozstvi--;
                return true;
            }

            if((*itr_vypl).mnozstvi == 1){
                salary_table.erase(itr_vypl);
            }

            return true;
        }
        return false;

    }
    bool          changeName       ( const string    & email,
                                     const string    & newName,            //!!stejne jmeno jako change by nefungovalo
                                     const string    & newSurname )
    {
        employee tmp_emp;
        tmp_emp.name = newName;
        tmp_emp.surname = newSurname;

        duo tmpduo;
        tmpduo.email = email;

        if(nameexists(tmp_emp) == true){
            return false;
        }

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);
        if(itr_duo == match_table.end()){
            return false;
        }

        if((*itr_duo).email != email){
            return false;
        }
        tmp_emp.name = (*itr_duo).name;
        tmp_emp.surname = (*itr_duo).surname;

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        tmp_emp.salary = (*itr_emp).salary;

        all_emp.erase(itr_emp);
        match_table.erase(itr_duo);

        tmp_emp.name = newName;
        tmp_emp.surname = newSurname;
        tmp_emp.email = email;

        vyplaty tmpvyplata;
        tmpvyplata.salary = tmp_emp.salary;

        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

        if((*itr_vypl).mnozstvi > 1){
                (*itr_vypl).mnozstvi--;
            }

        else if((*itr_vypl).mnozstvi == 1){
                salary_table.erase(itr_vypl);
            }
        
        add(tmp_emp.name,tmp_emp.surname,tmp_emp.email,tmp_emp.salary);

        return true;

    }
    bool          changeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail )
    {
        employee tmp_emp;
        tmp_emp.name = name;
        tmp_emp.surname = surname;

        duo tmpduo;
        tmpduo.email = newEmail;
        tmpduo.name = name;
        tmpduo.surname = surname;

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);

        if((itr_duo != match_table.end())&&((*itr_duo).email == newEmail)){
            return false;
        }

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        if(itr_emp== all_emp.end()){
            return false;
        }
        if(((*itr_emp).name != name)||((*itr_emp).surname) != surname){
            return false;
        }

        duo tmpduo_del;
        tmpduo_del.email = (*itr_emp).email;

        (*itr_emp).email = newEmail;

        auto itr_duo_del = lower_bound(match_table.begin(),match_table.end(),tmpduo_del,compare_mails);
        match_table.erase(itr_duo_del);

        itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);
        match_table.insert(itr_duo,tmpduo);

        return true;

    }
    bool          setSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary )
    {
        employee tmp_emp;
        tmp_emp.name = name;
        tmp_emp.surname = surname;

        vyplaty tmpvyplata;

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        if(itr_emp == all_emp.end()){
            return false;
        }
        if(((*itr_emp).name != name)||((*itr_emp).surname)!= surname){
            return false;
        }
        tmpvyplata.salary = (*itr_emp).salary;
        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

        if((*itr_vypl).mnozstvi > 1){
                (*itr_vypl).mnozstvi--;
                (*itr_emp).salary = salary;
                tmpvyplata.salary = salary;

                itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

                if((itr_vypl != salary_table.end())){
                    if((*itr_vypl).salary == salary){
                        (*itr_vypl).mnozstvi++;
                    }
                    else{
                        tmpvyplata.mnozstvi = 1;
                        salary_table.insert(itr_vypl,tmpvyplata);
                    }
                }
                else{
                    tmpvyplata.mnozstvi = 1;
                    salary_table.insert(itr_vypl,tmpvyplata);
                }
                return true;
            }

            if((*itr_vypl).mnozstvi == 1){
                salary_table.erase(itr_vypl);
            }



        //smazani

        (*itr_emp).salary = salary;
        //vlozeni nove
        tmpvyplata.salary = salary;
        itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

        if((itr_vypl != salary_table.end())){
            if((*itr_vypl).salary == salary){
                (*itr_vypl).mnozstvi++;
            }
            else{
                tmpvyplata.mnozstvi = 1;
                salary_table.insert(itr_vypl,tmpvyplata);
            }
        }
        else{
            tmpvyplata.mnozstvi = 1;
            salary_table.insert(itr_vypl,tmpvyplata);
        }

        return true;

    }
    bool          setSalary        ( const string    & email,
                                     unsigned int      salary )
    {
        duo tmpduo;
        tmpduo.email = email;

        employee tmp_emp;
        
        vyplaty tmpvyplata;

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);

        if(itr_duo == match_table.end()){
            return false;
        }
        if((*itr_duo).email != email){
            return false;
        }

        tmp_emp.name = (*itr_duo).name;
        tmp_emp.surname = (*itr_duo).surname;

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        tmpvyplata.salary = (*itr_emp).salary;
        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

        if((*itr_vypl).mnozstvi > 1){
                (*itr_vypl).mnozstvi--;
                (*itr_emp).salary = salary;
                tmpvyplata.salary = salary;
                
                auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

                if((itr_vypl != salary_table.end())){
                    if((*itr_vypl).salary == salary){
                        (*itr_vypl).mnozstvi++;
                    }
                    else{
                        tmpvyplata.mnozstvi = 1;
                        salary_table.insert(itr_vypl,tmpvyplata);
                    }
                }
                else{
                    tmpvyplata.mnozstvi = 1;
                    salary_table.insert(itr_vypl,tmpvyplata);
                }
                return true;
            }

            if((*itr_vypl).mnozstvi == 1){
                salary_table.erase(itr_vypl);
            }

        tmpvyplata.salary = salary;
        itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);

        if((itr_vypl != salary_table.end())){
            if((*itr_vypl).salary == salary){
                (*itr_vypl).mnozstvi++;
            }
            else{
                tmpvyplata.mnozstvi = 1;
                salary_table.insert(itr_vypl,tmpvyplata);
            }
        }
        else{
            tmpvyplata.mnozstvi = 1;
            salary_table.insert(itr_vypl,tmpvyplata);
        }


        (*itr_emp).salary = salary;

        return true;

    }
    unsigned int  getSalary        ( const string    & name,
                                     const string    & surname ) const
    {
        employee tmp_emp;
        tmp_emp.name = name;
        tmp_emp.surname = surname;

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        if(itr_emp == all_emp.end()){
            return 0;
        }
        if(((*itr_emp).name != name)||((*itr_emp).surname)!= surname){
            return 0;
        }

        return ((*itr_emp).salary);

    }
    unsigned int  getSalary        ( const string    & email ) const
    {
        employee tmp_emp;

        duo tmpduo;
        tmpduo.email = email;

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);

        if(itr_duo == match_table.end()){
            return 0;
        }
        if((*itr_duo).email != email){
            return 0;
        }

        tmp_emp.name = (*itr_duo).name;
        tmp_emp.surname = (*itr_duo).surname;

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        return (*itr_emp).salary;

    }
    bool          getRank          ( const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax ) const
    {
        employee tmp_emp;

        tmp_emp.name = name;
        tmp_emp.surname = surname;

        vyplaty tmpvyplata;


        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        if(itr_emp == all_emp.end()){
            return false;
        }
        if(((*itr_emp).name != name)||((*itr_emp).surname)!= surname){
            return false;
        }

        tmpvyplata.salary = (*itr_emp).salary;
        
        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);
                
        int pocet_stejne = (*itr_vypl).mnozstvi;

        int pocet_mene = 0;
        itr_vypl--;
        for(;itr_vypl >= salary_table.begin();itr_vypl--){
            pocet_mene = pocet_mene + (*itr_vypl).mnozstvi;
        }

        rankMin = pocet_mene;
        rankMax = pocet_mene + pocet_stejne -1;
        return true;

    }

    bool          getRank          ( const string    & email,
                                     int             & rankMin,
                                     int             & rankMax ) const
    {
        duo tmpduo;
        employee tmp_emp;

        tmpduo.email = email;

        vyplaty tmpvyplata;

        auto itr_duo = lower_bound(match_table.begin(),match_table.end(),tmpduo,compare_mails);

        if(itr_duo == match_table.end()){
            return false;
        }
        if((*itr_duo).email != email){
            return false;
        }

        tmp_emp.name = (*itr_duo).name;
        tmp_emp.surname = (*itr_duo).surname;

        auto itr_emp = lower_bound(all_emp.begin(),all_emp.end(),tmp_emp,compare_names);

        tmpvyplata.salary = (*itr_emp).salary;
        
        auto itr_vypl = lower_bound(salary_table.begin(),salary_table.end(),tmpvyplata,compare_vyplaty);
                
        int pocet_stejne = (*itr_vypl).mnozstvi;

        int pocet_mene = 0;
        itr_vypl--;
        for(;itr_vypl >= salary_table.begin();itr_vypl--){
            pocet_mene = pocet_mene + (*itr_vypl).mnozstvi;
        }
        
        rankMin = pocet_mene;
        rankMax = pocet_mene + pocet_stejne -1;
        return true;

    }                                 
    bool          getFirst         ( string          & outName,
                                     string          & outSurname ) const
    {
        if(all_emp.size() == 0){
          return false;
        }
        outName = all_emp[0].name;
        outSurname = all_emp[0].surname;
        return true;
    }
    bool          getNext          ( const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname ) const
    {
        employee tmp;
        tmp.name = name;
        tmp.surname = surname;

          auto iterator = lower_bound(all_emp.begin(),all_emp.end(),tmp,compare_names);
          if(iterator != all_emp.end()){
            if(((*iterator).name == name)&&((*iterator).surname == surname)){
              iterator++;
              if(iterator != all_emp.end()){     //!! jak to funguje kdyz se zmeni kapacita celyho vektoru ? doufam ze se to nerozbije xd
                outName = (*iterator).name;
                outSurname = (*iterator).surname;
                return true;
              }
            }
          }
          return false;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );

  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
