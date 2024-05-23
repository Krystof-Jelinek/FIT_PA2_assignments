#include <iostream>
#include <cassert>
#include <functional>
#include <sstream>
using namespace std;
 
 
class CHash
{
  private:
    struct TItem
    {
                             TItem                         ( const string & key,
                                                             const string & val )
        : m_Key ( key ),
          m_Val ( val ),
          m_NextHash ( NULL ),
          m_NextOrder ( NULL ),
          m_PrevOrder ( NULL )
      {
      }

        void killmeandmyfriends(){
            if(m_NextHash){
                m_NextHash->killmeandmyfriends();
            }
            delete this;
        }

        TItem * copymeandmyfriends(){
            TItem * to_be = nullptr;
            if(m_NextHash){
                to_be = m_NextHash->copymeandmyfriends();
            }
            TItem * ret = new TItem(this->m_Key, this->m_Val);
            ret->m_NextHash = to_be;
            return ret;
        }

      TItem * insert(const string & key, const string & val){
        if(m_NextHash){
            return m_NextHash->insert(key, val);
        }
        m_NextHash = new TItem(key,val);
        return m_NextHash;
      }
 
      string                 m_Key;
      string                 m_Val;
      TItem                * m_NextHash;
      TItem                * m_NextOrder;
      TItem                * m_PrevOrder;
    };
  public:
                             CHash                         ( size_t         size )
    {
      m_Size = size;
      m_Table = new TItem *[m_Size];
      for(size_t i = 0; i < m_Size; i++){
        m_Table[i] = nullptr;
      }
      m_FirstOrder = nullptr;
      m_LastOrder = nullptr;
    }
 
                             CHash                         ( const CHash  & src )
    {
      m_Size = src.m_Size;
      m_Table = new TItem *[m_Size];
      for(size_t i = 0; i < m_Size; i++){
        m_Table[i] = nullptr;
      }
      m_FirstOrder = nullptr;
      m_LastOrder = nullptr;
      
        auto ptr = src.m_FirstOrder;
        while(ptr){
            Ins(ptr->m_Key, ptr->m_Val);
            ptr = ptr->m_NextOrder;
        }
    }
 
                            ~CHash                         ( void )
    {
      for(size_t i  = 0; i < m_Size;i++){
        if(m_Table[i]){
            m_Table[i]->killmeandmyfriends();
        }
      }
      delete[] m_Table;
    }
 
    CHash                  & operator =                    ( const CHash  & src )
    {

        if(this == &src){
            return *this;
        }

      if(m_Table){
        for(size_t i  = 0; i < m_Size;i++){
        if(m_Table[i]){
            m_Table[i]->killmeandmyfriends();
        }
        }
        delete[] m_Table;
      }

      m_Size = src.m_Size;
      
      m_Table = new TItem *[m_Size];
      for(size_t i = 0; i < m_Size; i++){
        m_Table[i] = nullptr;
      }
      m_FirstOrder = nullptr;
      m_LastOrder = nullptr;
      
        auto ptr = src.m_FirstOrder;
        while(ptr){
            Ins(ptr->m_Key, ptr->m_Val);
            ptr = ptr->m_NextOrder;
        }
        return *this;
    }
 
    bool                     Ins                           ( const string & key,
                                                             const string & val )
    {
      if(IsSet(key)){
        return false;
      }
      TItem * cur = m_Table[hashFn(key,m_Size)];
        cur = new TItem(key,val);
        cur->m_NextHash = m_Table[hashFn(key,m_Size)];
        m_Table[hashFn(key,m_Size)] = cur;

      if(m_FirstOrder == nullptr){
        m_FirstOrder = cur;
        cur->m_NextOrder = nullptr;
        cur->m_PrevOrder = nullptr;
        m_LastOrder = cur;
      }
      else{
        m_LastOrder->m_NextOrder = cur;
        cur->m_PrevOrder = m_LastOrder;
        cur->m_NextOrder = nullptr;
        m_LastOrder = cur;
      }
      return true;
    }
 
    void del_from_list(TItem * ptr){
        if(ptr == m_FirstOrder && ptr == m_LastOrder){
            m_LastOrder = nullptr;
            m_FirstOrder = nullptr;
        }
        else if(ptr == m_FirstOrder){
            m_FirstOrder = ptr->m_NextOrder;
            ptr->m_NextOrder->m_PrevOrder = nullptr;
        }
        else if(ptr == m_LastOrder){
            m_LastOrder = ptr->m_PrevOrder;
            ptr->m_PrevOrder->m_NextOrder = nullptr;
        }
        else{
            ptr->m_PrevOrder->m_NextOrder = ptr->m_NextOrder;
            ptr->m_NextOrder->m_PrevOrder = ptr->m_PrevOrder;
        }
    }

    bool                     Del                           ( const string & key )
    {

        if(!IsSet(key)){
            return false;
        }

      TItem * cur = m_Table[hashFn(key,m_Size)];
      //if hned zacatek;
      if(cur->m_Key == key){
        del_from_list(cur);
        m_Table[hashFn(key,m_Size)] = cur->m_NextHash;
        delete cur;
        return true;
      }
      
      while(cur->m_NextHash){
        if(cur->m_NextHash->m_Key == key){
            TItem * next = cur->m_NextHash;
            cur->m_NextHash = next->m_NextHash;
            del_from_list(next);
            delete next;
            return true;
        }
        cur = cur->m_NextHash;
      }

      return false;
    }
 
    bool                     IsSet                         ( const string & key ) const
    {
      TItem * cur = m_Table[hashFn(key,m_Size)];
        
        while(cur){
            if(cur->m_Key == key){
                return true;
            }
            cur = cur->m_NextHash;
        }
        return false;
    }
 
    void                     ForEach                       (const std::function<void(const string & val)>& func) const
    {
      auto itr = m_FirstOrder;
      while(itr){
        func(itr->m_Val);
        itr = itr->m_NextOrder;
      }
    }
 
 
    TItem                 ** m_Table;
    size_t                   m_Size;
    TItem                  * m_FirstOrder;
    TItem                  * m_LastOrder;
 
    friend int main ( void );
 
    size_t                   hashFn                        ( const string & x,
                                                             size_t         modulus ) const
    {
      size_t h = 5381;
      for ( string::size_type c : x )
        h += ( h << 5 ) + c;
      return h % modulus;
    }
};
 
 
int main ( void )
{
  CHash x ( 100 );
  assert ( sizeof ( CHash ) == sizeof ( CHash::TItem * ) * 3 + sizeof ( size_t ) );
  assert ( sizeof ( CHash::TItem ) == sizeof ( CHash::TItem * ) * 3 + sizeof ( string ) * 2 );
  assert ( x . Ins ( "peg", "foo" ));
  cout << x.hashFn("peg", 100) << endl;
  assert ( x . m_Table[1] -> m_Key == "peg" );
  assert ( x . m_Table[1] -> m_Val == "foo" );
  assert ( x . m_FirstOrder == x . m_Table[1] );
  assert( x . m_FirstOrder -> m_NextOrder == NULL);
  assert( x . m_LastOrder == x . m_Table[1]);
  assert( x . m_LastOrder -> m_PrevOrder == NULL);

  assert ( x . Ins ( "bwc", "bar" )
           && x . m_Table[45] -> m_Key == "bwc"
           && x . m_Table[45] -> m_Val == "bar"
           && x . m_FirstOrder == x . m_Table[1] 
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . Ins ( "jdr", "test" )
           && x . m_Table[45] -> m_Key == "jdr"
           && x . m_Table[45] -> m_Val == "test"
           && x . m_Table[45] -> m_NextHash -> m_Key == "bwc"
           && x . m_Table[45] -> m_NextHash -> m_Val == "bar"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . Ins ( "peg", "something" ) == false );
  assert ( x . Ins ( "peg", "foo" ) == false );
  assert ( x . Ins ( "bwc", "test" ) == false );
  assert ( x . Ins ( "jdr", "mudr" ) == false );
  assert ( x . Ins ( "asa", "butterfield" )
           && x . m_Table[22] -> m_Key == "asa"             
           && x . m_Table[22] -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . IsSet ( "peg" ) );
  assert ( x . IsSet ( "asa" ) );
  assert ( x . IsSet ( "bwc" ) );
  assert ( x . IsSet ( "jdr" ) );
  assert ( x . IsSet ( "whf" ) == false );
  assert ( x . IsSet ( "xns" ) == false );
  assert ( x . IsSet ( "abc" ) == false );
  assert ( x . IsSet ( "ooj" ) == false );
  ostringstream oss;
  x . ForEach ( [&oss] ( const string & val )
    {
      oss << val << endl;
    } );
  assert ( oss . str () == "foo\nbar\ntest\nbutterfield\n" );
  CHash n ( x );
  assert ( n . m_Table[1] -> m_Key == "peg"
           && n . m_Table[1] -> m_Val == "foo"
           && n . m_Table[45] -> m_Key == "jdr"
           && n . m_Table[45] -> m_Val == "test"
           && n . m_Table[45] -> m_NextHash -> m_Key == "bwc"
           && n . m_Table[45] -> m_NextHash -> m_Val == "bar"
           && n . m_Table[22] -> m_Key == "asa"             
           && n . m_Table[22] -> m_Val == "butterfield"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45] -> m_NextHash
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45] -> m_NextHash
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( n . Del ( "jdr" )
           && n . m_Table[45] -> m_Key == "bwc"
           && n . m_Table[45] -> m_Val == "bar"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( n . IsSet ( "jdr" ) == false );
  assert ( x . m_Table[1] -> m_Key == "peg"
           && x . m_Table[1] -> m_Val == "foo"
           && x . m_Table[45] -> m_Key == "jdr"
           && x . m_Table[45] -> m_Val == "test"
           && x . m_Table[45] -> m_NextHash -> m_Key == "bwc"
           && x . m_Table[45] -> m_NextHash -> m_Val == "bar"
           && x . m_Table[22] -> m_Key == "asa"             
           && x . m_Table[22] -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( n . Del ( "404" ) == false );
  assert ( n . Del ( "system32" ) == false );
  assert ( n . Del ( "root" ) == false );
  n = n;
  assert ( n . Ins ( "xns", "test" )
           && n . m_Table[22] -> m_Key == "xns"
           && n . m_Table[22] -> m_Val == "test"
           && n . m_Table[22] -> m_NextHash -> m_Key == "asa"             
           && n . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22] -> m_NextHash
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL 
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[22] -> m_NextHash
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  x = n;
  assert ( x . m_Table[22] -> m_Key == "xns"
           && x . m_Table[22] -> m_Val == "test"
           && x . m_Table[22] -> m_NextHash -> m_Key == "asa"             
           && x . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL 
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . Ins ( "whf", "FIT" )
           && x . m_Table[22] -> m_Key == "whf"
           && x . m_Table[22] -> m_Val == "FIT"
           && x . m_Table[22] -> m_NextHash -> m_Key == "xns"
           && x . m_Table[22] -> m_NextHash -> m_Val == "test"
           && x . m_Table[22] -> m_NextHash -> m_NextHash -> m_Key == "asa"             
           && x . m_Table[22] -> m_NextHash -> m_NextHash -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( n . m_Table[22] -> m_Key == "xns"
           && n . m_Table[22] -> m_Val == "test"
           && n . m_Table[22] -> m_NextHash -> m_Key == "asa"             
           && n . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && n . m_FirstOrder == n . m_Table[1]
           && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22] -> m_NextHash
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
           && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL 
           && n . m_LastOrder == n . m_Table[22]
           && n . m_LastOrder -> m_PrevOrder == n . m_Table[22] -> m_NextHash
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
           && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . Del ( "xns" )
           && x . m_Table[22] -> m_Key == "whf"
           && x . m_Table[22] -> m_Val == "FIT"
           && x . m_Table[22] -> m_NextHash -> m_Key == "asa"             
           && x . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . Del ( "whf" )
           && x . m_Table[22] -> m_Key == "asa"             
           && x . m_Table[22] -> m_Val == "butterfield"
           && x . m_FirstOrder == x . m_Table[1]
           && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
           && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
           && x . m_LastOrder == x . m_Table[22]
           && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
           && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
  assert ( x . IsSet ( "asa" ) );
  assert ( x . IsSet ( "xns" ) == false );
  assert ( x . IsSet ( "whf" ) == false );
  assert ( x . Del ( "asa" )
           && x . m_Table[22] == NULL );
  assert ( x . Del ( "peg" )
           && x . m_Table[1] == NULL );
  assert ( x . Del ( "bwc" )
           && x . m_Table[45] == NULL );
  assert ( x . m_FirstOrder == NULL
           && x . m_LastOrder == NULL );

 /*
           */
  return 0;
}