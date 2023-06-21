#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */

class CElement;
class CButton;
class CLabel;
class CInput;
class CComboBox;
class CWindow;
void resize(CButton * change, CWindow * parent);
void resize(CLabel * change, CWindow * parent);
void resize(CComboBox * change, CWindow * parent);
void resize(CInput * change, CWindow * parent);

class CElement{
  public:
  int m_id;
  CRect m_position{-69,-69,-69,-69};
  CRect m_absolute{-69,-69,-69,-69};
  CWindow * m_parent = nullptr;
  

  virtual void recalculate() = 0;
  virtual string selfprint(int x) const = 0;
};



class CButton : public CElement
{
  public:
    string m_name;

                             CButton                       ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & name )
    {
        m_position = CRect(relPos);
        m_id = id;
        m_name = name;
        m_parent = nullptr;
    }
    void recalculate() override{
        if(m_parent == nullptr){
        cout << "Velky spatny1" << endl;
        }
        resize(this,m_parent);

      }
    string selfprint(int x) const override{
        ostringstream tmp;
        tmp << "[" << m_id << "] Button " << "\"" << m_name << "\" " << m_absolute << "\n";
        return tmp.str();  
    }
    
    
};

class CInput : public CElement
{
  public:
  string m_value;
                             CInput                        ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & value )
    {
        m_position = CRect(relPos);
        m_id = id;
        m_value = value;
        m_parent = nullptr;
    }                                                             
    // setValue
    void setValue(const string & x){
        m_value = x;
    } 
    // getValue
    string getValue(){
        return m_value;
    } 
    
    void recalculate() override{
        if(m_parent == nullptr){
        cout << "Velky spatny2" << endl;
        }
        resize(this,m_parent);
    }

    string selfprint(int x) const override{
        ostringstream tmp;
        tmp << "[" << m_id << "] Input " << "\"" << m_value << "\" " << m_absolute << "\n";
        return tmp.str();  
    }

};
class CLabel : public CElement
{
  public:
  string m_label;
                             CLabel                        ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & label )
    {
        m_position = CRect(relPos);
        m_id = id;
        m_label = label;
        m_parent = nullptr;
    } 
    void recalculate() override{
        if(m_parent == nullptr){
        cout << "Velky spatny3" << endl;
        }
        resize(this,m_parent);
    }
    
    string selfprint(int x) const override{
        ostringstream tmp;
        tmp << "[" << m_id << "] Label " << "\"" << m_label << "\" " << m_absolute << "\n";
        return tmp.str();  
    }
    

};

class CComboBox : public CElement
{
  public:
    int m_selected;
    vector<string> m_messages;


                             CComboBox                     ( int               id,
                                                             const CRect     & relPos )
    {
        m_position = CRect(relPos);
        m_id = id;
        m_selected = 0;
        m_parent = nullptr;
    }

    CComboBox(const CComboBox & src){
        m_position = src.m_position;        
        m_id = src.m_id;        
        m_messages = src.m_messages;        
        m_selected = src.m_selected;
    }
    
    CComboBox & add(const string & x){
        m_messages.push_back(x);
        return *this;
    }                                                                            
    void setSelected(int x){
        m_selected = x;
    }
    int getSelected(){
        return m_selected;
    }

    void recalculate() override{
        if(m_parent == nullptr){
        cout << "Velky spatny4" << endl;
        }
        resize(this,m_parent);
    }

    string selfprint(int x) const override{
        if(x == 2){
            ostringstream tmp;
            tmp << "[" << m_id << "] ComboBox " << m_absolute << "\n";
            int i = 0;
            for(auto itr = m_messages.begin();itr != m_messages.end();itr++){
                if(i == m_selected){
                    tmp << "|  +->" << (*itr) << "<\n";
                    i++;
                    continue;
                }
            tmp << "|  +- " << (*itr) << "\n";
            i++;
        }
        return tmp.str();  
        }
        ostringstream tmp;
        tmp << "[" << m_id << "] ComboBox " << m_absolute << "\n";
        int i = 0;
        for(auto itr = m_messages.begin();itr != m_messages.end();itr++){
            if(i == m_selected){
                tmp << "   +->" << (*itr) << "<\n";
                i++;
                continue;
            }
            tmp << "   +- " << (*itr) << "\n";
            i++;
        }
        return tmp.str();  
    }

    string selfprint_only() const{
        ostringstream tmp;
        tmp << "[" << m_id << "] ComboBox " << m_absolute << "\n";
        int i = 0;
        for(auto itr = m_messages.begin();itr != m_messages.end();itr++){
            if(i == m_selected){
                tmp << "+->" << (*itr) << "<\n";
                i++;
                continue;
            }
            tmp << "+- " << (*itr) << "\n";
            i++;
        }
        return tmp.str();  
    }
    
     

};


class CWindow 
{
  public:
  int m_id;
  CRect m_position{-69,-69,-69,-69};
  string m_title;
  vector<shared_ptr<CElement>> m_elemets;

                             CWindow                       ( int               id,
                                                             const string    & title,
                                                             const CRect     & absPos )
    {
        m_id = id;
        m_position = CRect(absPos);
        m_title = title;
    }

    CWindow(const CWindow & src){
        m_id = src.m_id;
        m_position = src.m_position;
        m_title = src.m_title;

        for(auto itr = src.m_elemets.begin();itr != src.m_elemets.end();itr++){
            if (auto comboBoxPtr = dynamic_cast<CComboBox*>(itr->get())) {
            m_elemets.push_back(make_shared<CComboBox>(*comboBoxPtr));
            } 
            else if (auto buttonPtr = dynamic_cast<CButton*>(itr->get())) {
            m_elemets.push_back(make_shared<CButton>(*buttonPtr));
            }
            else if (auto ptr = dynamic_cast<CLabel*>(itr->get())) {
            m_elemets.push_back(make_shared<CLabel>(*ptr));
            }
            else if (auto ptr = dynamic_cast<CInput*>(itr->get())) {
            m_elemets.push_back(make_shared<CInput>(*ptr));
            }
        }
        for(auto itr = m_elemets.begin();itr != m_elemets.end();itr++){
            (*itr).get()->m_parent = this;
        }
        setPosition(src.m_position);
    }

    CWindow & operator = (const CWindow & source){
        CWindow tmp(source);
        this->m_elemets = tmp.m_elemets;
        this->m_id = tmp.m_id;
        this->m_position= tmp.m_position;
        this->m_title = tmp.m_title;
        (*this).setPosition(source.m_position);
        return (*this);
    }


    // add
    CWindow & add(const CButton & adding){

        shared_ptr<CElement> itr = make_shared<CButton>(CButton(adding));
        (*itr).m_parent = this;
        m_elemets.push_back(itr);
        (*itr).recalculate();
        return *this;
    }
    CWindow & add(const CInput & adding){

        shared_ptr<CElement> itr = make_shared<CInput>(CInput(adding));
        (*itr).m_parent = this;
        m_elemets.push_back(itr);
        (*itr).recalculate();
        return *this;
    }
    CWindow & add(const CLabel & adding){
        shared_ptr<CElement> itr = make_shared<CLabel>(CLabel(adding));
        (*itr).m_parent = this;
        m_elemets.push_back(itr);
        (*itr).recalculate();
        return *this;
    }
    CWindow & add(const CComboBox & adding){
        shared_ptr<CElement> itr = make_shared<CComboBox>(CComboBox(adding));
        (*itr).m_parent = this;
        m_elemets.push_back(itr);
        (*itr).recalculate();
        return *this;
    }
    // search
    CElement * search(int id){
        
        for(auto itr = m_elemets.begin();itr != m_elemets.end();itr++){
            if((*itr).get()->m_id == id){
                return itr->get(); 
            }
        }
        return nullptr;
    }
    // setPosition
    void setPosition(const CRect & src){
        m_position = src;
        for(auto itr = m_elemets.begin();itr != m_elemets.end();itr++){
            (*itr).get()->recalculate();
        }
    }
    string selfprint() const{
        ostringstream tmp;
        tmp << "[" << m_id << "] Window " << "\"" << m_title << "\" " << m_position << "\n";
        for(auto itr = m_elemets.begin();itr != m_elemets.end();itr++){
            auto zkouska = itr;
            zkouska++;
            if(zkouska != m_elemets.end()){
                tmp << "+- " << (*itr).get()->selfprint(2);
                continue;    
            }
            tmp << "+- " << (*itr).get()->selfprint(1);
        }

        return tmp.str();  
    } 

};

void resize(CButton * change, CWindow * parent){
    change->m_absolute.m_X = parent->m_position.m_X + parent->m_position.m_W * change->m_position.m_X;
    change->m_absolute.m_Y = parent->m_position.m_Y + parent->m_position.m_H * change->m_position.m_Y; 
    change->m_absolute.m_W = parent->m_position.m_W * change->m_position.m_W;
    change->m_absolute.m_H = parent->m_position.m_H * change->m_position.m_H;
}
void resize(CInput * change, CWindow * parent){
    change->m_absolute.m_X = parent->m_position.m_X + parent->m_position.m_W * change->m_position.m_X;
    change->m_absolute.m_Y = parent->m_position.m_Y + parent->m_position.m_H * change->m_position.m_Y; 
    change->m_absolute.m_W = parent->m_position.m_W * change->m_position.m_W;
    change->m_absolute.m_H = parent->m_position.m_H * change->m_position.m_H;
}
void resize(CComboBox * change, CWindow * parent){
    change->m_absolute.m_X = parent->m_position.m_X + parent->m_position.m_W * change->m_position.m_X;
    change->m_absolute.m_Y = parent->m_position.m_Y + parent->m_position.m_H * change->m_position.m_Y; 
    change->m_absolute.m_W = parent->m_position.m_W * change->m_position.m_W;
    change->m_absolute.m_H = parent->m_position.m_H * change->m_position.m_H;
}
void resize(CLabel * change, CWindow * parent){
    change->m_absolute.m_X = parent->m_position.m_X + parent->m_position.m_W * change->m_position.m_X;
    change->m_absolute.m_Y = parent->m_position.m_Y + parent->m_position.m_H * change->m_position.m_Y; 
    change->m_absolute.m_W = parent->m_position.m_W * change->m_position.m_W;
    change->m_absolute.m_H = parent->m_position.m_H * change->m_position.m_H;
}

// output operators
ostringstream & operator << (ostringstream & oss,const CButton & src){
    oss << (src.selfprint(1));
    return oss;
}
ostringstream & operator << (ostringstream & oss,const CLabel & src){
    oss << (src.selfprint(1));
    return oss;
}
ostringstream & operator << (ostringstream & oss,const CInput & src){
    oss << (src.selfprint(1));
    return oss;
}
ostringstream & operator << (ostringstream & oss,const CComboBox & src){
    oss << (src.selfprint_only());
    return oss;
}
ostringstream & operator << (ostringstream & oss,const CWindow & src){
    oss << (src.selfprint());
    return oss;
}
ostringstream & operator << (ostringstream & oss,const CElement & src){
    if (const CComboBox* cb = dynamic_cast<const CComboBox*>(&src)) {
        // This is a CComboBox object
        oss << cb->selfprint_only();
        return oss;
    }
    oss << (src.selfprint(1));
    return oss;
}

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  /*CButton TTT(1,CRect(10,20,30,40),"chupapimunano");
  cout << TTT.selfprint();
  CLabel b(2,CRect(1,2,3,4),"tohlechchji");
  CInput c(3,CRect(5,5,5,5),"AAAAAAA");

  CComboBox d(4,CRect(69,69,69,69));
  d.add("prvni");
  d.add("druha");
  d.add("treti");
  assert(d.getSelected() == 0);
  d.setSelected(1);
  assert(d.getSelected() == 1);

  CWindow okno(10,"vrchni window", CRect(1,1,1,1));
  okno . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) );*/
  

  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
    assert ( toString ( *a . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return EXIT_SUCCESS;
  
}
#endif /* __PROGTEST__ */
