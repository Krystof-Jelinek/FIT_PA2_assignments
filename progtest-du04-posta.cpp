#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class CMail
{
  private:
    char * sender = NULL;
    char * recv = NULL;
    char * obsah = NULL;
    friend class CMailIterator;
    friend class CMailServer;
    friend class Sender;
  
  public:
                             CMail                         ( const char      * from,
                                                             const char      * to,
                                                             const char      * body )
    {
        sender = new char [strlen(from) +1 ];
        strcpy(sender,from);
        recv = new char[strlen(to) +1 ];
        strcpy(recv,to);
        obsah = new char[strlen(body) +1 ];
        strcpy(obsah,body);
    };

    CMail(){
      sender = NULL;
      recv = NULL;
      obsah = NULL;
      };

    ~CMail(){
        if(sender!=NULL){
          delete[]sender;
        }
        if(recv!=NULL){
          delete[]recv;
        }
        if(sender!=NULL){
          delete[]obsah;
        }
        sender = NULL;
        recv = NULL;
        obsah = NULL;
    };

    CMail & operator = ( const CMail & src )
     {  
        if(this->sender != NULL){
            delete[]this->sender;
        }
        this->sender = new char [strlen(src.sender) +1 ];
        strcpy(this->sender,src.sender);

        if(this->recv != NULL){
            delete[]this->recv;
        }
        this->recv = new char [strlen(src.recv) +1 ];
        strcpy(this->recv,src.recv);

        if(this->obsah != NULL){
            delete[]this->obsah;
        }
        this->obsah = new char [strlen(src.obsah) +1 ];
        strcpy(this->obsah,src.obsah);
        
        return *this;
     }

    bool                     operator ==                   ( const CMail     & x ) const
    {
        if((strcmp(sender,x.sender) == 0)&&(strcmp(recv,x.recv) == 0)&&(strcmp(obsah,x.obsah) == 0)){
            return true;
        }
        return false;
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CMail     & m )
    {
        os << "From: " << m.sender << ", To: " << m.recv << ", Body: " << m.obsah;
        return os;
    }
};

class Person{
    public:

    char * name = NULL;
    int *  e_indexes = NULL;
    int capacity = 0;
    int email_num = 0;

    Person(){
        name = NULL;
        e_indexes = NULL;
        capacity = 0;
        email_num = 0;
    }

    Person(const Person & source){
        name = new char[strlen(source.name) +1];
        strcpy(name,source.name);
        capacity = source.capacity;
        email_num = source.email_num;
        
        e_indexes = new int [source.capacity];
        for(int i = 0; i < email_num;i++){
            e_indexes[i] = source.e_indexes[i];
        }
    }

    ~Person(){
        delete[]name;
        delete[]e_indexes;
    }

    Person & operator = (const Person & source){
        Person tmp(source);
        auto swap_name = this->name;
        auto swap_indexes = this->e_indexes;
        auto swap_capacity = this->capacity;
        auto swap_email_num = this->email_num;

        this->name = tmp.name;
        this->e_indexes = tmp.e_indexes;
        this->capacity = tmp.capacity;
        this->email_num = tmp.email_num;

        tmp.name = swap_name;
        tmp.e_indexes = swap_indexes;
        tmp.capacity = swap_capacity;
        tmp.email_num = swap_email_num;

        return *this;
    }


    void print_person(){
        for(int i =0;i < email_num;i++){
            cout << name << "  index zpravy: "<< e_indexes[i] << endl;
        }
    }

    void add_email(int index){
        if(email_num >= capacity){
            capacity = int((capacity*1.5) +1);
            int * tmpemails = new int[capacity];
            for(int i = 0;i < email_num;i++){
              tmpemails[i] = e_indexes[i];
            }
            delete[]e_indexes;
            
            e_indexes = tmpemails;
        }

        e_indexes[email_num] = index;
        email_num++;
    }

};

class Senders{
    public:
    Person * all_senders = nullptr;
    int capacity;
    int num_senders;

    Senders(){
        all_senders = new Person[1];
        capacity = 1;
        num_senders = 0;
    }
    ~Senders(){
        delete[]all_senders;
    }

    Senders(const Senders & source){
        all_senders = new Person[source.capacity];
        capacity = source.capacity;
        num_senders = source.num_senders;
        for(int i = 0;i < num_senders;i++){
            all_senders[i] = source.all_senders[i];
        }
        
    }

    Senders & operator =( const Senders & source){
        Senders tmp(source);
        auto swap_senders = this->all_senders;
        auto swap_num_senders = this->num_senders;
        auto swap_capacity = this->capacity;

        this->all_senders = tmp.all_senders;
        this->num_senders = tmp.num_senders;
        this->capacity = tmp.capacity;

        tmp.all_senders = swap_senders;
        tmp.num_senders = swap_num_senders;
        tmp.capacity = swap_capacity;

        return *this;
    }

    void add_sender(const char * name, int index){
          //rozsireni pameti
         if(capacity <= num_senders){
            capacity = int((capacity*1.5) +1);
            Person * tmp = new Person[capacity];
            for(int i = 0;i < num_senders; i++){
                tmp[i] = all_senders[i];
            }
            delete[]all_senders;
            all_senders = tmp;
        }

        for(int i = 0; i< num_senders;i++){
            if(strcmp(all_senders[i].name,name)==0){
                all_senders[i].add_email(index);
                return;
            }
        }

        all_senders[num_senders].name = new char [strlen(name) +1];
        strcpy(all_senders[num_senders].name,name);
        all_senders[num_senders].add_email(index);
        num_senders++;
    }

};

class Recievers{
    public:
    Person * all_recievers = nullptr;
    int capacity;
    int num_recievers;

    Recievers(){
        all_recievers = new Person[1];
        capacity = 1;
        num_recievers = 0;
    }
    ~Recievers(){
        delete[]all_recievers;
    }

    Recievers(const Recievers & source){
        all_recievers = new Person[source.capacity];
        capacity = source.capacity;
        num_recievers = source.num_recievers;
        for(int i = 0;i < num_recievers;i++){
            all_recievers[i] = source.all_recievers[i];
        }
        
    }

    Recievers & operator =( const Recievers & source){
        Recievers tmp(source);
        auto swap_recievers = this->all_recievers;
        auto swap_num_recievers = this->num_recievers;
        auto swap_capacity = this->capacity;

        this->all_recievers = tmp.all_recievers;
        this->num_recievers = tmp.num_recievers;
        this->capacity = tmp.capacity;

        tmp.all_recievers = swap_recievers;
        tmp.num_recievers = swap_num_recievers;
        tmp.capacity = swap_capacity;

        return *this;
    }

    void add_reciever(const char * name, int index){
          //rozsireni pameti
         if(capacity <= num_recievers){
            capacity = int((capacity*1.5) +1);
            Person * tmp = new Person[capacity];
            for(int i = 0;i < num_recievers; i++){
                tmp[i] = all_recievers[i];
            }
            delete[]all_recievers;
            all_recievers = tmp;
        }

        for(int i = 0; i< num_recievers;i++){
            if(strcmp(all_recievers[i].name,name)==0){
                all_recievers[i].add_email(index);
                return;
            }
        }

        all_recievers[num_recievers].name = new char [strlen(name) +1];
        strcpy(all_recievers[num_recievers].name,name);
        all_recievers[num_recievers].add_email(index);
        num_recievers++;
    }

};

class Schranka{
  public:
    CMail *all_mails;
    int field_capacity;
    int mail_count;

    Schranka(){
        all_mails = NULL;
        field_capacity = 0;
        mail_count = 0;
    }

    ~Schranka(){
        delete[]all_mails;
    }

    Schranka(const Schranka & source){
        field_capacity = source.field_capacity;
        mail_count = source.mail_count;
        all_mails = new CMail[field_capacity];
        for(int i = 0;i< mail_count;i++){
            all_mails[i] = source.all_mails[i];
        }
    }

    Schranka & operator =( const Schranka & source){
        Schranka tmp(source);

        auto swap_all_mails = this->all_mails;
        auto swap_mail_count = this->mail_count;
        auto swap_field_capacity = this->field_capacity;

        this->all_mails = tmp.all_mails;
        this->mail_count = tmp.mail_count;
        this->field_capacity = tmp.field_capacity;

        tmp.all_mails = swap_all_mails;
        tmp.mail_count = swap_mail_count;
        tmp.field_capacity = swap_field_capacity;

        return *this;
    }

    const CMail & operator[](int index){
        return all_mails[index];
    }

    int add_mail(const CMail & add ){
        //rozsireni pameti
        if(mail_count  >= field_capacity ){
            field_capacity = int((field_capacity*1.5) +1);
            CMail * tmp = new CMail[field_capacity];
            for(int i = 0;i < mail_count; i++){
                tmp[i] = all_mails[i];
            }
            delete[]all_mails;
            all_mails = tmp;
        }
        //pridani emailu
        all_mails[mail_count] = add;
        int index = mail_count;
        mail_count++;
        return index;
    }
};

class CMailIterator 
{
  private:
    int flag;
    int person_index;
    Person person;
    Schranka iter_schranka;
    friend class CMailServer;
  public:
    CMailIterator(){
        flag = -1;
        person_index = -69;

    }

    CMailIterator(const Schranka & in_schranka,const Person & in_sender){
        person = in_sender;
        flag = 1;
        person_index = 0;

        for(int i = 0; i < person.email_num;i++){
            iter_schranka.add_mail(in_schranka.all_mails[in_sender.e_indexes[i]]);
        }
    }

    CMailIterator(const CMailIterator & source){

        if(source.flag == -1){
            flag = -1;
        }
        else{
            person = source.person;
            flag = source.flag;
            person_index = source.person_index;

            for(int i = 0; i < source.person.email_num;i++){
                iter_schranka.add_mail(source.iter_schranka.all_mails[i]);
            }
        }
    }

    explicit                 operator bool                 ( void ) const
    {
        if(flag == -1){
            return false;
        }

        if(person.email_num <= person_index){
            return false;
        }
        return true;
    }
    bool                     operator !                    ( void ) const
    {
        if(*this){
            return false;
        }
        return true;
    }
    const CMail            & operator *                    ( void ) const
    {
        return iter_schranka.all_mails[person_index];
    }
    
    CMailIterator          & operator ++                   ( void )
    {
        this->person_index++;
        return *this;
    }
    
};



class CMailServer 
{
  private:
    Senders senders;
    Recievers recievers;
    friend class CMailIterator;
    Schranka schranka;
  public:

                             CMailServer                   ( void )
    {

    }
                             CMailServer                   ( const CMailServer & src )
    {
        senders = src.senders;
        recievers = src.recievers;
        schranka = src.schranka;
    }
    CMailServer            & operator =                    ( const CMailServer & src )
    {
        CMailServer tmp(src);

        auto swap_schranka = this->schranka;
        auto swap_sedners = this->senders;
        auto swap_recievers = this->recievers;

        this->schranka = tmp.schranka;
        this->senders = tmp.senders;
        this->recievers = tmp.recievers;

        tmp.schranka = swap_schranka;
        tmp.senders = swap_sedners;
        tmp.recievers = swap_recievers;

        return *this;
    }
                             ~CMailServer                  ( void )
    {
        
    }

    void                     sendMail                      ( const CMail     & m )
    {
        int index = schranka.add_mail(m);

        senders.add_sender(m.sender,index);
        recievers.add_reciever(m.recv,index);


    }
    CMailIterator            outbox                        ( const char      * email ) const
    {
        for(int i = 0;i< senders.num_senders;i++){
            if(strcmp(senders.all_senders[i].name,email)== 0){
                CMailIterator itr(schranka,senders.all_senders[i]);
                return itr;
            }
        }
        CMailIterator itr;
        return itr;
    }
    CMailIterator            inbox                         ( const char      * email ) const
    {
        for(int i = 0;i< recievers.num_recievers;i++){
            if(strcmp(recievers.all_recievers[i].name,email)== 0){
                CMailIterator itr(schranka,recievers.all_recievers[i]);
                return itr;
            }
        }
        CMailIterator itr;
        return itr;
    }

    void print()
    {
      cout << "ALLMAILS" << endl;
      for(int i = 0; i<schranka.mail_count; i++)
      {
      cout << schranka.all_mails[i] << endl;
      }

      cout << "SENDERS" << endl;

      for(int i =0; i< senders.num_senders;i++){
        senders.all_senders[i].print_person();
      }

      cout << "RECIEVERS" << endl;

      for(int i =0; i< recievers.num_recievers;i++){
        recievers.all_recievers[i].print_person();
      }
    }
  
};

#ifndef __PROGTEST__
bool                         matchOutput                   ( const CMail     & m,
                                                             const char      * str )
{
  ostringstream oss;
  oss << m;
  return oss . str () == str;
}

int main ( void )
{
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;
  s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  
  CMailIterator i2 = s0 . outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i0 = s0 . inbox ( "alice" );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
  assert ( ! ++i1 );


  CMailIterator i3 = s0 . outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . outbox ( "thomas" );
  s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . inbox ( "alice" );
  s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
  assert ( ! ++i13 );


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
