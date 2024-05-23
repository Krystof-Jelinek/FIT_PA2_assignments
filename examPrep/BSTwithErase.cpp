/**
 * Implementujte metodu insert (vkládání), erase (mazání prvků
 * ze stromu a zároveň je potřeba uchovat správné pořádí podle
 * data vkládání) a vlastní destruktor, ostatní metody jsou
 * naimplentované. Kopírující konstruktor i operátor přiřazení
 * zakázán. Není povolené si přidat žádnou členskou proměnnou,
 * ale lze naimplementovat vlastní pomocné metody.
 *
 * Za správnost šablony neručím :-).
 */
 
#include <string>
#include <cassert>
#include <iostream>
 
using namespace std;

class CTree
{
  protected:
    class CNode
    {
      public:
        CNode(string key) : m_Key(key) {}
        string m_Key;
        CNode * m_L = nullptr;
        CNode * m_R = nullptr;
        CNode * m_father = nullptr;
        CNode * m_PrevOrder = nullptr;
        CNode * m_NextOrder = nullptr;

        bool insert(const string & key, CNode * & retptr){
            if(m_Key == key){
                return false;
            }
            if(key > m_Key){
                if(m_R){
                    return m_R->insert(key, retptr);
                }
                m_R = new CNode{key};
                m_R->m_father = this;
                retptr = m_R;
                return true;
            }
            else{
                if(m_L){
                    return m_L->insert(key, retptr);
                }
                m_L = new CNode{key};
                m_L->m_father = this;
                retptr = m_L;
                return true;
            }
        }

        bool isLeftSon(){
            if(m_father){
                if(m_father->m_L == this){
                    return true;
                }
            }
            return false;
        }
        bool isRightSon(){
            if(m_father){
                if(m_father->m_R == this){
                    return true;
                }
            }
            return false;
        }

        void killme(){
            if(m_L){
                m_L->killme();
            }
            if(m_R){
                m_R->killme();
            }
            delete this;
        }


    };
  public:
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr;
    CNode * m_Last = nullptr;

    void erase_from_linkedlist(CNode * to_del){
        if((to_del->m_NextOrder == nullptr) && (to_del->m_PrevOrder == nullptr)){
            m_First = nullptr;
            m_Last = nullptr;
            return;
        }
        CNode * tmp_prev = to_del->m_PrevOrder;
        CNode * tmp_next = to_del->m_NextOrder;
        if(tmp_prev){
            to_del->m_PrevOrder->m_NextOrder = tmp_next;
        }
        else{
            m_First = to_del->m_NextOrder;
        }
        if(tmp_next){
            to_del->m_NextOrder->m_PrevOrder = tmp_prev;
        }
        else{
            m_Last = to_del->m_PrevOrder;
        }
    }

    CTree() = default;
    ~CTree()
    {
      if(m_Root){
        m_Root->killme();
      }
    }

    void connectLeftSon(CNode * son, CNode * father){
        if(father){
            father->m_L = son;
        }
        if(son){
            son->m_father = father;
        }
    }

    void connectRightSon(CNode * son, CNode * father){
        if(father){
            father->m_R = son;
        }
        if(son){
            son->m_father = father;
        }
    }

    CTree(const CTree &src) = delete;
    bool operator=(const CTree &other) = delete;
    bool isSet(string key)
    {
      CNode * n = m_Root;
      while (n)
      {
        if (key == n->m_Key)
          return true;
        else if (key > n->m_Key)
          n = n->m_R;
        else
          n = n->m_L;
      }
      return false;
    }

    CNode * find_node(const string & key){
        CNode * n = m_Root;
        while (n)
        {
            if (key == n->m_Key)
                return n;
            else if (key > n->m_Key)
                n = n->m_R;
            else
                n = n->m_L;
        }
        return nullptr;
    }

    CNode * find_sucesor(CNode * n){
        n = n->m_R;
        while (n)
        {
            if(n->m_L){
                n = n->m_L;
                continue;
            }
            return n;
        }
        throw logic_error("chyba sucesor");
    }

    bool insert(string key)
    {
        if(!m_Root){
            m_Root = new CNode{key};
            m_First = m_Root;
            m_Last = m_Root;
            return true;
        }

        CNode * tmp_ptr;
        bool res = m_Root->insert(key,tmp_ptr);
        if(res){
            m_Last->m_NextOrder = tmp_ptr;
            tmp_ptr->m_PrevOrder = m_Last;
            m_Last = tmp_ptr;
            return true;
        }
        return false;
    }

    bool eraseRoot(string key){
        CNode * to_del = m_Root;
        
        erase_from_linkedlist(to_del);

        if((!m_Root->m_L)&&(!m_Root->m_R)){
            delete m_Root;
            m_Root = nullptr;
            return true;
        }

        //mam praveho
        if((!to_del->m_L)){
            connectRightSon(to_del->m_R, to_del->m_father);
            m_Root = to_del->m_R;
            delete to_del;
            return true;
        }
        //mam leveho
        if((!to_del->m_R)){
            connectLeftSon(to_del->m_L, to_del->m_father);
            m_Root = to_del->m_L;
            delete to_del;
            return true;
        }

        //mam oba
        CNode * sucesor = find_sucesor(to_del);

        //edgecase
        if(sucesor == to_del->m_R){
            sucesor->m_father = nullptr;
            connectLeftSon(to_del->m_L, sucesor);
            m_Root = sucesor;
            delete to_del;
            return true;
        }
        else{
            connectLeftSon(sucesor->m_R, sucesor->m_father);
            connectLeftSon(to_del->m_L, sucesor);
            connectRightSon(to_del->m_R, sucesor);
            connectLeftSon(sucesor, to_del->m_father);
            m_Root = sucesor;
            delete to_del;
            return true;
        }


    }

    bool erase(string key)
    {
        if(!isSet(key)){
            return false;
        }

        CNode * to_del = find_node(key);

        if(to_del == m_Root){
            return eraseRoot(key);
        }

        //spojak done
        erase_from_linkedlist(to_del);

        //nemam syny
        if((!to_del->m_L) && (!to_del->m_R)){
            if(to_del->isLeftSon()){
                connectLeftSon(nullptr, to_del->m_father);
            }
            if(to_del->isRightSon()){
                connectRightSon(nullptr, to_del->m_father);
            }
            delete to_del;
            return true;
        }

        //mam praveho
        if((!to_del->m_L)){
            if(to_del->isLeftSon()){
                connectLeftSon(to_del->m_R, to_del->m_father);
                delete to_del;
                return true;
            }
            if(to_del->isRightSon()){
                connectRightSon(to_del->m_R, to_del->m_father);
                delete to_del;
                return true;
            }
        }
        //mam leveho
        if((!to_del->m_R)){
            if(to_del->isLeftSon()){
                connectLeftSon(to_del->m_L, to_del->m_father);
                delete to_del;
                return true;
            }
            if(to_del->isRightSon()){
                connectRightSon(to_del->m_L, to_del->m_father);
                delete to_del;
                return true;
            }
        }

        //mam oba
        CNode * sucesor = find_sucesor(to_del);

        //edgecase
        if(sucesor == to_del->m_R){
            sucesor->m_father = to_del->m_father;
            if(to_del->isLeftSon()){
                to_del->m_father->m_L = sucesor;
            }
            if(to_del->isRightSon()){
                to_del->m_father->m_R = sucesor;
            }
            connectLeftSon(to_del->m_L, sucesor);
            delete to_del;
            return true;
        }
        else{
            connectLeftSon(sucesor->m_R, sucesor->m_father);

            if(to_del->isLeftSon()){
                connectLeftSon(sucesor, to_del->m_father);
            }
            if(to_del->isRightSon()){
                connectRightSon(sucesor, to_del->m_father);
            }
            connectLeftSon(to_del->m_L, sucesor);
            connectRightSon(to_del->m_R, sucesor);
            delete to_del;
            return true;
        }
    }
};
 
class CTester : public CTree
{
public:
	CTester () = default;
 
	static void test ()
	{
		CTester t0;
		assert ( t0 . insert ( "PA1" ) == true );
		assert ( t0 . m_First -> m_Key == "PA1" && t0 . m_First -> m_NextOrder == nullptr );
		assert ( t0 . isSet ( "PA1" ) == true );
		assert ( t0 . insert ( "UOS" ) == true );
		assert ( t0 . insert ( "PA2" ) == true );
		assert ( t0 . isSet ( "PA2" ) == true );
		assert ( t0 . isSet ( "PA3" ) == false );
 
		assert ( t0 . insert ( "PA2" ) == false );
		assert ( t0 . insert ( "CAO" ) == true );
		assert ( t0 . insert ( "LIN" ) == true );
		assert ( t0 . insert ( "AAG" ) == true );
		assert ( t0 . insert ( "AG1" ) == true );
		assert ( t0 . insert ( "ZDM" ) == true );
 
		assert ( t0 . m_First -> m_Key == "PA1"
				 && t0 . m_First -> m_NextOrder -> m_Key == "UOS"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "CAO"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "LIN"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key ==
					"AAG"
				 &&
				 t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key ==
				 "AG1"
				 &&
				 t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key ==
				 "ZDM" );
 
		assert ( t0 . m_Last -> m_Key == "ZDM"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_Key == "AAG"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "LIN"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "CAO"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key ==
					"PA2"
				 &&
				 t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key ==
				 "UOS"
				 &&
				 t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key ==
				 "PA1" );
 
		assert ( t0 . erase ( "" ) == false );
 
		assert ( t0 . erase ( "ZDM" ) == true );
		assert ( t0 . m_First -> m_Key == "PA1"
				 && t0 . m_First -> m_NextOrder -> m_Key == "UOS"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "CAO"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "LIN"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key ==
					"AAG"
				 &&
				 t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key ==
				 "AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "AAG"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_Key == "LIN"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "CAO"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA2"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key ==
					"UOS"
				 &&
				 t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key ==
				 "PA1" );
		assert ( t0 . isSet ( "ZDM" ) == false );
 
		assert ( t0 . erase ( "AAG" ) == true );
		assert ( t0 . m_First -> m_Key == "PA1"
				 && t0 . m_First -> m_NextOrder -> m_Key == "UOS"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "CAO"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "LIN"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key ==
					"AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "LIN"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_Key == "CAO"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA2"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "UOS"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key ==
					"PA1" );
		assert ( t0 . isSet ( "AAG" ) == false );
 
		assert ( t0 . erase ( "CAO" ) == true );
		assert ( t0 . m_First -> m_Key == "PA1"
				 && t0 . m_First -> m_NextOrder -> m_Key == "UOS"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "LIN"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "LIN"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA2"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "UOS"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA1" );
		assert ( t0 . isSet ( "CAO" ) == false );
 
		assert ( t0 . erase ( "UOS" ) == true );
		assert ( t0 . m_First -> m_Key == "PA1"
				 && t0 . m_First -> m_NextOrder -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_Key == "LIN"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_Key == "AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "LIN"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA2"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA1" );
		assert ( t0 . isSet ( "UOS" ) == false );
 
 
		assert ( t0 . erase ( "LIN" ) == true );
		assert ( t0 . m_First -> m_Key == "PA1"
				 && t0 . m_First -> m_NextOrder -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_NextOrder -> m_Key == "AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "PA2"
				 && t0 . m_Last -> m_PrevOrder -> m_PrevOrder -> m_Key == "PA1" );
		assert ( t0 . isSet ( "LIN" ) == false );
 
		assert ( t0 . erase ( "PA1" ) == true );
		assert ( t0 . m_First -> m_Key == "PA2"
				 && t0 . m_First -> m_NextOrder -> m_Key == "AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1"
				 && t0 . m_Last -> m_PrevOrder -> m_Key == "PA2" );
		assert ( t0 . isSet ( "PA1" ) == false );
 
		assert ( t0 . erase ( "PA2" ) == true );
		assert ( t0 . m_First -> m_Key == "AG1" );
		assert ( t0 . m_Last -> m_Key == "AG1" );
		assert ( t0 . isSet ( "PA2" ) == false );
 
		assert ( t0 . erase ( "AG1" ) == true );
		assert ( t0 . m_First == t0 . m_Last );
		assert ( t0 . isSet ( "AG1" ) == false );
 
		assert ( t0 . erase ( "PA2" ) == false );
 
		assert ( t0 . insert ( "PA1" ) == true );
		assert ( t0 . insert ( "UOS" ) == true );
		assert ( t0 . insert ( "PA2" ) == true );
		assert ( t0 . insert ( "PA2" ) == false );
		assert ( t0 . insert ( "CAO" ) == true );
		assert ( t0 . insert ( "LIN" ) == true );
		assert ( t0 . insert ( "AAG" ) == true );
		assert ( t0 . insert ( "AG1" ) == true );
		assert ( t0 . insert ( "ZDM" ) == true );
		assert ( t0 . insert ( "FOO" ) == true );
 
		assert ( t0 . erase ( "CAO" ) == true );
		assert ( t0 . erase ( "CANNOT_ERASED" ) == false );
		assert ( t0 . erase ( "PA1" ) == true );
		assert ( t0 . erase ( "CAO" ) == false );
		assert ( t0 . erase ( "LIN" ) == true );
		assert ( t0 . erase ( "AAG" ) == true );
		assert ( t0 . erase ( "AG1" ) == true );
		assert ( t0 . erase ( "FOO" ) == true );
		assert ( t0 . erase ( "PA2" ) == true );
		assert ( t0 . erase ( "ZDM" ) == true );
		assert ( t0 . erase ( "UOS" ) == true );
 
		assert ( t0 . insert ( "PA1" ) == true );
		assert ( t0 . insert ( "UOS" ) == true );
		assert ( t0 . insert ( "PA2" ) == true );
		assert ( t0 . insert ( "PA2" ) == false );
		assert ( t0 . insert ( "CAO" ) == true );
		assert ( t0 . insert ( "LIN" ) == true );
		assert ( t0 . insert ( "AAG" ) == true );
		assert ( t0 . insert ( "AG1" ) == true );
		assert ( t0 . insert ( "ZDM" ) == true );
 
		assert ( t0 . erase ( "UOS" ) == true );
		assert ( t0 . erase ( "PA2" ) == true );
		assert ( t0 . erase ( "ZDM" ) == true );
		assert ( t0 . erase ( "PA1" ) == true );
		assert ( t0 . erase ( "AAG" ) == true );
		assert ( t0 . erase ( "AG1" ) == true );
		assert ( t0 . erase ( "LIN" ) == true );
		assert ( t0 . erase ( "CAO" ) == true );
 
		assert ( t0 . insert ( "PA1" ) == true );
		assert ( t0 . insert ( "CAO" ) == true );
		assert ( t0 . insert ( "UOS" ) == true );
		assert ( t0 . insert ( "PA2" ) == true );
		assert ( t0 . insert ( "PA2" ) == false );
		assert ( t0 . insert ( "LIN" ) == true );
		assert ( t0 . insert ( "AAG" ) == true );
		assert ( t0 . insert ( "AG1" ) == true );
		assert ( t0 . insert ( "ZDM" ) == true );
 
		assert ( t0 . erase ( "PA1" ) == true );
		assert ( t0 . erase ( "ZDM" ) == true );
		assert ( t0 . erase ( "UOS" ) == true );
		assert ( t0 . erase ( "PA2" ) == true );
		assert ( t0 . erase ( "AG1" ) == true );
		assert ( t0 . erase ( "CAO" ) == true );
		assert ( t0 . erase ( "AAG" ) == true );
		assert ( t0 . erase ( "LIN" ) == true );
 
 
		assert ( t0 . insert ( "20" ) == true );
		assert ( t0 . insert ( "10" ) == true );
		assert ( t0 . insert ( "5" ) == true );
		assert ( t0 . insert ( "15" ) == true );
		assert ( t0 . insert ( "17" ) == true );
		assert ( t0 . insert ( "13" ) == true );
		assert ( t0 . insert ( "14" ) == true );
		assert ( t0 . insert ( "11" ) == true );
		assert ( t0 . insert ( "12" ) == true );
		assert ( t0 . insert ( "3" ) == true );
		assert ( t0 . insert ( "7" ) == true );
		assert ( t0 . insert ( "25" ) == true );
 
		assert ( t0 . erase ( "10" ) == true );
		assert ( t0 . erase ( "25" ) == true );
		assert ( t0 . erase ( "20" ) == true );
		assert ( t0 . erase ( "17" ) == true );
	}
};
 
int main ()
{
	CTester::test ();
    CTester t1;
      assert(t1.insert( "kkk"));
      assert(t1.insert( "aaa"));
      assert(t1.insert( "ppp"));
      assert(t1.insert( "mmm"));
      assert(t1.insert( "nnn"));
      assert(t1.insert( "lll"));
      assert(t1.erase("kkk"));
      assert(t1.m_First->m_Key == "aaa");
      cout << "jop" << endl;
	return EXIT_SUCCESS;
}