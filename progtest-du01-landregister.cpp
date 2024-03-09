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
#endif /* __PROGTEST__ */

using namespace std;

std::string toLowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

class CParcel{
    public:
    string m_city;
    string m_adress;
    string m_region;
    unsigned m_id;
    string m_owner;

  CParcel(){

  };

  CParcel(const string & city,const string & adres,const string & region, unsigned id, const string & owner){
    m_city = city;
    m_adress = adres;
    m_region = region;
    m_id = id;
    m_owner = owner;
  };

  bool operator == (const CParcel & other) const{
    return m_city == other.m_city && m_adress == other.m_adress && m_region == other.m_region && m_id == other.m_id;
  }

};

bool compare_parcels_id(const CParcel & first, const CParcel & second){
  if(first.m_id == second.m_id){
    return first.m_region < second.m_region;
  }
  return first.m_id < second.m_id;
}

bool compare_parcels_city(const CParcel & first, const CParcel & second){
  if(first.m_city == second.m_city){
    return first.m_adress < second.m_adress;
  }
  return first.m_city < second.m_city;
}

class COwner{
    public:
    string m_compare_name;
    string m_real_name;
    vector<CParcel> m_parcel_list;

    COwner(const string & name){
      m_real_name = name;
      m_compare_name = toLowercase(name);
      m_parcel_list.clear();
    }

    void remove_parcel(CParcel & to_be_del){
      for(auto itr = m_parcel_list.begin(); itr != m_parcel_list.end(); itr++){
        if((*itr) == to_be_del){
          m_parcel_list.erase(itr);
          return;
        }
      }
    }
};

bool compare_owners(const COwner & first, const COwner & second){
  return first.m_compare_name < second.m_compare_name;
}


class CIterator
{
  public:
    bool                     atEnd                         () const{
      if(itr == vec_pointer->end()){
        return true;
      }
      return false;
    }
    void                     next                          (){
      itr++;
    }
    std::string              city                          () const{
      return itr->m_city;
    }
    std::string              addr                          () const{
      return itr->m_adress;
    }
    std::string              region                        () const{
      return itr->m_region;
    }
    unsigned                 id                            () const{
      return itr->m_id;
    }
    std::string              owner                         () const{
      return itr->m_owner;
    }
  
    CIterator(const vector<CParcel> * in_vec_pointer){
      itr = in_vec_pointer->begin();
      vec_pointer = in_vec_pointer;
    }

  private:
    vector<CParcel>::const_iterator itr;
    const vector<CParcel> * vec_pointer;

};

class CLandRegister
{

  vector<CParcel> m_city_ordered;
  vector<CParcel> m_id_ordered;
  vector<COwner> m_owners;
  vector<CParcel> m_empty_vector;

  public:
    CLandRegister(){
      COwner state("");

      m_owners.push_back(state);
    }

    bool find(const string & city, const string & address, CParcel & founded_parcel) {
      if(m_city_ordered.empty()){
        return false;
      }
      CParcel tmp(city, address, "", 1, "tmp");
      auto itr = get_cityorder_itr(tmp);
      if(itr != m_city_ordered.end() && itr->m_city == city && itr->m_adress == address){
        founded_parcel = (*itr);
        return true;
      }
      return false;
    }

    bool find(unsigned id, const string & region, CParcel & founded_parcel) {
      if(m_city_ordered.empty()){
        return false;
      }
      CParcel tmp("", "", region, id, "tmp");
      auto itr = get_idorder_itr(tmp);
      if(itr != m_id_ordered.end() && itr->m_id == id && itr->m_region == region){
        founded_parcel = (*itr);
        return true;
      }
      return false;
    }

    bool find_with_stupid_const(const string & city, const string & address, CParcel & founded_parcel) const {
      if(m_city_ordered.empty()){
        return false;
      }
      CParcel tmp(city, address, "", 1, "tmp");
      auto itr = get_cityorder_itr_with_stupid_const(tmp);
      if(itr != m_city_ordered.end() && itr->m_city == city && itr->m_adress == address){
        founded_parcel = (*itr);
        return true;
      }
      return false;
    }

    bool find_with_stupid_const(unsigned id, const string & region, CParcel & founded_parcel) const{
      if(m_city_ordered.empty()){
        return false;
      }
      CParcel tmp("", "", region, id, "tmp");
      auto itr = get_idorder_itr_with_stupid_const(tmp);
      if(itr != m_id_ordered.end() && itr->m_id == id && itr->m_region == region){
        founded_parcel = (*itr);
        return true;
      }
      return false;
    }

    bool validate_add_input(const string & city,const string & addr,const string & region, unsigned id){

      if(m_city_ordered.empty()){
        return true;
      }

      string owner = "tmp";
      string tmp_region = "tmp";
      unsigned tmp_id = 1;

      CParcel tmp(city, addr, tmp_region, tmp_id, owner);

      auto itr = lower_bound(m_city_ordered.begin(), m_city_ordered.end(), tmp, compare_parcels_city);
      if(itr != m_city_ordered.end() && itr->m_city == city && itr->m_adress == addr){
        return false;
      }

      tmp.m_region = region;
      tmp.m_id = id;

      auto itr2 = lower_bound(m_id_ordered.begin(), m_id_ordered.end(), tmp, compare_parcels_id);
      if(itr2 != m_id_ordered.end() && itr2->m_region == region && itr2->m_id == id){
        return false;
      }
      return true;
    }

    vector<CParcel>::iterator get_cityorder_itr(const CParcel & input) {
      auto itr = lower_bound(m_city_ordered.begin(), m_city_ordered.end(), input, compare_parcels_city);
      return itr;
    }

    vector<CParcel>::iterator get_idorder_itr(const CParcel & input) {
      auto itr = lower_bound(m_id_ordered.begin(), m_id_ordered.end(), input, compare_parcels_id);
      return itr;
    }

    vector<CParcel>::const_iterator get_cityorder_itr_with_stupid_const(const CParcel & input) const{
      auto itr = lower_bound(m_city_ordered.begin(), m_city_ordered.end(), input, compare_parcels_city);
      return itr;
    }

    vector<CParcel>::const_iterator get_idorder_itr_with_stupid_const(const CParcel & input) const{
      auto itr = lower_bound(m_id_ordered.begin(), m_id_ordered.end(), input, compare_parcels_id);
      return itr;
    }

    vector<COwner>::iterator get_owner_itr(const string & input){
      COwner tmp(input);
      auto itr = lower_bound(m_owners.begin(), m_owners.end(), tmp, compare_owners);
      return itr;
    }

    vector<COwner>::const_iterator get_owner_itr_with_stupid_const(const string & input) const{
      COwner tmp(input);
      auto itr = lower_bound(m_owners.begin(), m_owners.end(), tmp, compare_owners);
      return itr;
    }

    bool                     add                           ( const std::string    & city,
                                                             const std::string    & addr,
                                                             const std::string    & region,
                                                             unsigned int           id )
    {
      if(!validate_add_input(city,addr, region, id)){
        return false;
      }

      CParcel input(city, addr, region, id, "");

      auto itr = get_cityorder_itr(input);
      m_city_ordered.insert(itr, input);

      auto itr2 = get_idorder_itr(input);
      m_id_ordered.insert(itr2, input);

      auto itr3 = get_owner_itr("");
      itr3->m_parcel_list.push_back(input);

      return true;
    }

    bool                     del                           ( const std::string    & city,
                                                             const std::string    & addr )
    {
        CParcel tmp(city, addr, "", 1, "");
        
        if(find(city, addr, tmp) == false){
          return false;
        }

        delete_parcel(tmp);
        return true;
    }

    bool                     del                           ( const std::string    & region,
                                                             unsigned int           id )
    {
      CParcel tmp("", "", region, id, "");
        
        if(find(id, region, tmp) == false){
          return false;
        }

        delete_parcel(tmp);
        return true;
    }

    void delete_parcel(CParcel & input){
      auto itr = get_cityorder_itr(input);
      m_city_ordered.erase(itr);

      auto itr2 = get_idorder_itr(input);
      m_id_ordered.erase(itr2);

      auto itr3 = get_owner_itr(input.m_owner);
      itr3->remove_parcel(input);
    
    }

    bool                     getOwner                      ( const std::string    & city,
                                                             const std::string    & addr,
                                                             std::string          & owner ) const
    {
      CParcel tmp("", "", "", 1, "");
      if(find_with_stupid_const(city, addr, tmp) == false){
        return false;
      }

      owner = tmp.m_owner;
      return true;
    }

    bool                     getOwner                      ( const std::string    & region,
                                                             unsigned int           id,
                                                             std::string          & owner ) const
    {
      CParcel tmp("", "", "", 1, "");
      if(find_with_stupid_const(id, region ,tmp) == false){
        return false;
      }

      owner = tmp.m_owner;
      return true;
    }

    bool                     newOwner                      ( const std::string    & city,
                                                             const std::string    & addr,
                                                             const std::string    & owner )
    {
      CParcel parcel(city, addr, "", 1, "");
      if (find(city, addr, parcel) == false){
        return false;
      }
      if (toLowercase(parcel.m_owner) == toLowercase(owner)){
        return false;
      }

      auto itr = get_owner_itr(parcel.m_owner);
      itr->remove_parcel(parcel);

      parcel.m_owner = owner;
      auto itr2 = get_owner_itr(owner);

      //if this owner doesnt yet exists
      if(itr2 == m_owners.end() || itr2->m_compare_name != toLowercase(owner)){
        COwner new_owner(owner);
        m_owners.insert(itr2, new_owner);
        itr2 = get_owner_itr(owner);
      }

      itr2->m_parcel_list.push_back(parcel);

      auto city_itr = get_cityorder_itr(parcel);
      city_itr->m_owner = owner;

      auto id_itr = get_idorder_itr(parcel);
      id_itr->m_owner = owner;
      return true;

    }

    bool                     newOwner                      ( const std::string    & region,
                                                             unsigned int           id,
                                                             const std::string    & owner )
    {
      CParcel parcel("", "", region, id, "");
      if (find(id, region, parcel) == false){
        return false;
      }
      if (toLowercase(parcel.m_owner) == toLowercase(owner)){
        return false;
      }

      auto itr = get_owner_itr(parcel.m_owner);
      itr->remove_parcel(parcel);

      parcel.m_owner = owner;
      auto itr2 = get_owner_itr(owner);

      //if this owner doesnt yet exists
      if(itr2 == m_owners.end() || itr2->m_compare_name != toLowercase(owner)){
        COwner new_owner(owner);
        m_owners.insert(itr2, new_owner);
        itr2 = get_owner_itr(owner);
      }

      itr2->m_parcel_list.push_back(parcel);

      auto city_itr = get_cityorder_itr(parcel);
      city_itr->m_owner = owner;

      auto id_itr = get_idorder_itr(parcel);
      id_itr->m_owner = owner;
      return true;
    }

    size_t                   count                         ( const std::string    & owner ) const
    {
      auto itr = get_owner_itr_with_stupid_const(owner);
      if(itr == m_owners.end()||itr->m_compare_name != toLowercase(owner)){
        return 0;
      }
      return itr->m_parcel_list.size();
    }

    CIterator                listByAddr                    () const{
      CIterator res(&m_city_ordered);
      return res;
    }

    CIterator                listByOwner                   ( const std::string    & owner ) const{
      auto itr = get_owner_itr_with_stupid_const(owner);

      if(itr == m_owners.end() || itr->m_compare_name != toLowercase(owner)){
        CIterator res(&(m_empty_vector));
        return res;
      }

      CIterator res(&(itr->m_parcel_list));
      return res;
    }
  private:
    // todo
};
#ifndef __PROGTEST__

static void test0 ()
{
  CLandRegister x;
  std::string owner;

  assert ( x . add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
  assert ( x . add ( "Prague", "Evropska", "Vokovice", 12345 ) );
  assert ( x . add ( "Prague", "Technicka", "Dejvice", 9873 ) );
  assert ( x . add ( "Plzen", "Evropska", "Plzen mesto", 78901 ) );
  assert ( x . add ( "Liberec", "Evropska", "Librec", 4552 ) );
  CIterator i0 = x . listByAddr ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Liberec"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Librec"
           && i0 . id () == 4552
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Plzen"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Plzen mesto"
           && i0 . id () == 78901
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Vokovice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Technicka"
           && i0 . region () == "Dejvice"
           && i0 . id () == 9873
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Thakurova"
           && i0 . region () == "Dejvice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( i0 . atEnd () );

  assert ( x . count ( "" ) == 5 );
  CIterator i1 = x . listByOwner ( "" );
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Thakurova"
           && i1 . region () == "Dejvice"
           && i1 . id () == 12345
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Evropska"
           && i1 . region () == "Vokovice"
           && i1 . id () == 12345
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Technicka"
           && i1 . region () == "Dejvice"
           && i1 . id () == 9873
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Plzen"
           && i1 . addr () == "Evropska"
           && i1 . region () == "Plzen mesto"
           && i1 . id () == 78901
           && i1 . owner () == "" );
  i1 . next ();
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Liberec"
           && i1 . addr () == "Evropska"
           && i1 . region () == "Librec"
           && i1 . id () == 4552
           && i1 . owner () == "" );
  i1 . next ();
  assert ( i1 . atEnd () );

  assert ( x . count ( "CVUT" ) == 0 );
  CIterator i2 = x . listByOwner ( "CVUT" );
  assert ( i2 . atEnd () );

  assert ( x . newOwner ( "Prague", "Thakurova", "CVUT" ) );
  assert ( x . newOwner ( "Dejvice", 9873, "CVUT" ) );
  assert ( x . newOwner ( "Plzen", "Evropska", "Anton Hrabis" ) );
  assert ( x . newOwner ( "Librec", 4552, "Cvut" ) );
  assert ( x . getOwner ( "Prague", "Thakurova", owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Dejvice", 12345, owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Prague", "Evropska", owner ) && owner == "" );
  assert ( x . getOwner ( "Vokovice", 12345, owner ) && owner == "" );
  assert ( x . getOwner ( "Prague", "Technicka", owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Dejvice", 9873, owner ) && owner == "CVUT" );
  assert ( x . getOwner ( "Plzen", "Evropska", owner ) && owner == "Anton Hrabis" );
  assert ( x . getOwner ( "Plzen mesto", 78901, owner ) && owner == "Anton Hrabis" );
  assert ( x . getOwner ( "Liberec", "Evropska", owner ) && owner == "Cvut" );
  assert ( x . getOwner ( "Librec", 4552, owner ) && owner == "Cvut" );
  CIterator i3 = x . listByAddr ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Liberec"
           && i3 . addr () == "Evropska"
           && i3 . region () == "Librec"
           && i3 . id () == 4552
           && i3 . owner () == "Cvut" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Plzen"
           && i3 . addr () == "Evropska"
           && i3 . region () == "Plzen mesto"
           && i3 . id () == 78901
           && i3 . owner () == "Anton Hrabis" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Prague"
           && i3 . addr () == "Evropska"
           && i3 . region () == "Vokovice"
           && i3 . id () == 12345
           && i3 . owner () == "" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Prague"
           && i3 . addr () == "Technicka"
           && i3 . region () == "Dejvice"
           && i3 . id () == 9873
           && i3 . owner () == "CVUT" );
  i3 . next ();
  assert ( ! i3 . atEnd ()
           && i3 . city () == "Prague"
           && i3 . addr () == "Thakurova"
           && i3 . region () == "Dejvice"
           && i3 . id () == 12345
           && i3 . owner () == "CVUT" );
  i3 . next ();
  assert ( i3 . atEnd () );

  assert ( x . count ( "cvut" ) == 3 );
  CIterator i4 = x . listByOwner ( "cVuT" );
  assert ( ! i4 . atEnd ()
           && i4 . city () == "Prague"
           && i4 . addr () == "Thakurova"
           && i4 . region () == "Dejvice"
           && i4 . id () == 12345
           && i4 . owner () == "CVUT" );
  i4 . next ();
  assert ( ! i4 . atEnd ()
           && i4 . city () == "Prague"
           && i4 . addr () == "Technicka"
           && i4 . region () == "Dejvice"
           && i4 . id () == 9873
           && i4 . owner () == "CVUT" );
  i4 . next ();
  assert ( ! i4 . atEnd ()
           && i4 . city () == "Liberec"
           && i4 . addr () == "Evropska"
           && i4 . region () == "Librec"
           && i4 . id () == 4552
           && i4 . owner () == "Cvut" );
  i4 . next ();
  assert ( i4 . atEnd () );

  assert ( x . newOwner ( "Plzen mesto", 78901, "CVut" ) );
  assert ( x . count ( "CVUT" ) == 4 );
  CIterator i5 = x . listByOwner ( "CVUT" );
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Prague"
           && i5 . addr () == "Thakurova"
           && i5 . region () == "Dejvice"
           && i5 . id () == 12345
           && i5 . owner () == "CVUT" );
  i5 . next ();
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Prague"
           && i5 . addr () == "Technicka"
           && i5 . region () == "Dejvice"
           && i5 . id () == 9873
           && i5 . owner () == "CVUT" );
  i5 . next ();
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Liberec"
           && i5 . addr () == "Evropska"
           && i5 . region () == "Librec"
           && i5 . id () == 4552
           && i5 . owner () == "Cvut" );
  i5 . next ();
  assert ( ! i5 . atEnd ()
           && i5 . city () == "Plzen"
           && i5 . addr () == "Evropska"
           && i5 . region () == "Plzen mesto"
           && i5 . id () == 78901
           && i5 . owner () == "CVut" );
  i5 . next ();
  assert ( i5 . atEnd () );

  assert ( x . del ( "Liberec", "Evropska" ) );
  assert ( x . del ( "Plzen mesto", 78901 ) );
  assert ( x . count ( "cvut" ) == 2 );
  CIterator i6 = x . listByOwner ( "cVuT" );
  assert ( ! i6 . atEnd ()
           && i6 . city () == "Prague"
           && i6 . addr () == "Thakurova"
           && i6 . region () == "Dejvice"
           && i6 . id () == 12345
           && i6 . owner () == "CVUT" );
  i6 . next ();
  assert ( ! i6 . atEnd ()
           && i6 . city () == "Prague"
           && i6 . addr () == "Technicka"
           && i6 . region () == "Dejvice"
           && i6 . id () == 9873
           && i6 . owner () == "CVUT" );
  i6 . next ();
  assert ( i6 . atEnd () );

  assert ( x . add ( "Liberec", "Evropska", "Librec", 4552 ) );
}

static void test1 ()
{
  CLandRegister x;
  std::string owner;

  assert ( x . add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
  assert ( x . add ( "Prague", "Evropska", "Vokovice", 12345 ) );
  assert ( x . add ( "Prague", "Technicka", "Dejvice", 9873 ) );
  assert ( ! x . add ( "Prague", "Technicka", "Hradcany", 7344 ) );
  assert ( ! x . add ( "Brno", "Bozetechova", "Dejvice", 9873 ) );
  assert ( !x . getOwner ( "Prague", "THAKUROVA", owner ) );
  assert ( !x . getOwner ( "Hradcany", 7343, owner ) );
  CIterator i0 = x . listByAddr ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Evropska"
           && i0 . region () == "Vokovice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Technicka"
           && i0 . region () == "Dejvice"
           && i0 . id () == 9873
           && i0 . owner () == "" );
  i0 . next ();
  assert ( ! i0 . atEnd ()
           && i0 . city () == "Prague"
           && i0 . addr () == "Thakurova"
           && i0 . region () == "Dejvice"
           && i0 . id () == 12345
           && i0 . owner () == "" );
  i0 . next ();
  assert ( i0 . atEnd () );

  assert ( x . newOwner ( "Prague", "Thakurova", "CVUT" ) );
  assert ( ! x . newOwner ( "Prague", "technicka", "CVUT" ) );
  assert ( ! x . newOwner ( "prague", "Technicka", "CVUT" ) );
  assert ( ! x . newOwner ( "dejvice", 9873, "CVUT" ) );
  assert ( ! x . newOwner ( "Dejvice", 9973, "CVUT" ) );
  assert ( ! x . newOwner ( "Dejvice", 12345, "CVUT" ) );
  assert ( x . count ( "CVUT" ) == 1 );
  CIterator i1 = x . listByOwner ( "CVUT" );
  assert ( ! i1 . atEnd ()
           && i1 . city () == "Prague"
           && i1 . addr () == "Thakurova"
           && i1 . region () == "Dejvice"
           && i1 . id () == 12345
           && i1 . owner () == "CVUT" );
  i1 . next ();
  assert ( i1 . atEnd () );

  assert ( ! x . del ( "Brno", "Technicka" ) );
  assert ( ! x . del ( "Karlin", 9873 ) );
  assert ( x . del ( "Prague", "Technicka" ) );
  assert ( ! x . del ( "Prague", "Technicka" ) );
  assert ( ! x . del ( "Dejvice", 9873 ) );
}

int main ( void )
{
  test0 ();
  test1 ();
  
  CLandRegister x;
  assert (x.add("city", "addres", "region", 4) == true);
  
  assert (x.add("city", "addres", "region", 4) == false);
  assert (x.add("city", "AAaddres", "region", 4) == false);
  assert (x.add("city", "addres", "XXregion", 4) == false);

  assert (x.add("Acity", "addres", "region", 1) == true);
  assert (x.add("city", "Aaddres", "Aregion", 4) == true);

  assert(x.del("XX", "XX") == false);
  assert(x.del("XX", 88) == false);
  assert(x.del("city", "XX") == false);
  assert(x.del("XX", "addres") == false);
  assert(x.del("region", 88) == false);
  assert(x.del("XX", "4") == false);

  assert(x.del("city" , "addres") == true);
  assert(x.del("region" , 1) == true);
  assert(x.del("city" , "Aaddres") == true);

  assert(x.del("XX", "4") == false);

  assert (x.add("city", "addres", "region", 4) == true);
  assert (x.add("AAAA", "addres", "region", 5) == true);

  assert(x.newOwner("city", "addres", "") == false);
  assert(x.newOwner("XX", "XX", "") == false);
  //assert(x.newOwner("XX", 88, "") == false);
  assert(x.newOwner("city", "XX", "") == false);
  assert(x.newOwner("XX", "addres", "") == false);
  //assert(x.newOwner("region", 88, "") == false);
  assert(x.newOwner("XX", "4", "") == false);
  assert(x.newOwner("region", 4, "BOREC") == true);
  assert(x.newOwner("region", 4, "BOREC") == false);
  assert(x.newOwner("region", 5, "") == false);
  assert(x.newOwner("region", 5, "CCC") == true);
  assert(x.newOwner("region", 5, "alkoholec") == true);
  assert(x.newOwner("region", 5, "ARneco") == true);
  
  string owner;

  assert(x.del("region", 5) == true);
  assert(x.getOwner("region", 4, owner) == true && owner == "BOREC");
  assert(x.getOwner("region", 5, owner) == false && owner == "BOREC");
  assert(x.count("BOREC") == 1);

  assert(x.del("region", 4) == true);

  assert (x.add("city", "addres", "region", 4) == true);
  assert (x.add("AAAA", "addres", "region", 5) == true);

  CIterator tmp = x.listByAddr();
  tmp.next();
  tmp.next();

  CIterator second = x.listByOwner("");
  second.next();
  second.next();

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
