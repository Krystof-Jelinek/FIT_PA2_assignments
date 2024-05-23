#include<iostream>
#include<string>
#include<cassert>
#include<sstream>
using namespace std;
 
class CTree{
public:
    CTree() = default;
    CTree(const CTree & src) = delete;
    CTree & operator = (const CTree & src) = delete;
    ~CTree(){
        if(m_Root){
            m_Root->killme();
        }
    }
    bool isSet(const string & key){
        if(m_Root){
            return m_Root->find(key);
        }
        return false;
    }
    bool insert(const string & key, const string & val){
        if(!m_Root){
            m_Root = new CNode{key, val};
            m_First = m_Root;
            m_Last = m_Root;
            return true;
        }
        CNode * tmp;
        bool res = m_Root->insert(key,val, tmp);
        if(res){
            m_Last->m_NextOrder = tmp;
            m_Last = tmp;
        }
        return res;
    }

    friend ostream & operator << (ostream & os, const CTree & src){
        CNode * ptr = src.m_First;
        os << "{";
        while(ptr){
            if(ptr == src.m_Last){
                os << ptr->m_Key << " => " << ptr->m_Val;
            }
            else{
                os << ptr->m_Key << " => " << ptr->m_Val << ", ";
            }
            ptr = ptr->m_NextOrder;
        }
        os << "}";
        return os;
    }
protected:
    class CNode{
    public:
        bool insert(const string & key, const string & val, CNode * & added_ptr){
            if(key == m_Key){
                return false;
            }
            if(key > m_Key){
                if(m_R == nullptr){
                    m_R = new CNode{key, val};
                    added_ptr = m_R;
                    return true;
                }
                else{
                    return m_R->insert(key,val,added_ptr);
                }
            }
            if(key < m_Key){
                if(m_L == nullptr){
                    m_L = new CNode{key, val};
                    added_ptr = m_L;
                    return true;
                }
                else{
                    return m_L->insert(key,val,added_ptr);
                }
            }
            throw logic_error("tohle legit neni mozny");
        }

        bool find(const string & key){
            if(m_Key == key){
                return true;
            }
            if(key < m_Key){
                if(m_L){
                    return m_L->find(key);
                }
                return false;
            }
            if(key > m_Key){
                if(m_R){
                    return m_R->find(key);
                }
                return false;
            }
            throw logic_error("imposible");
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

        CNode(const string & key, const string & val)
        :m_Key(key), m_Val(val) {}
        string m_Key, m_Val;
        bool last = false;
        CNode * m_L = nullptr, * m_R = nullptr; 
        CNode * m_NextOrder = nullptr;  

    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();
};
 
int main(void){
    CTree t;
    stringstream ss;
    ss << t;
    assert(ss.str() == "{}");
    ss.clear();
    ss.str(""); 
    assert(t.insert("PA1", "done"));
    assert(t.m_First == t.m_Last);
    assert(t.m_First->m_Key == "PA1");
    assert(!t.isSet("UOS"));
    assert(t.insert("PA2", "fail"));
    assert(t.insert("UOS", "funny"));
 
    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny}");
    ss.clear();
    ss.str("");
 
 
    assert(t.m_Root->m_L== nullptr);
    assert(t.m_Last->m_Key == "UOS");
    assert(t.m_Root->m_R->m_Key == "PA2");
    assert(t.m_Root->m_R->m_L == nullptr);
    assert(t.m_Root->m_R->m_R->m_Key == "UOS");
    assert(t.m_First->m_NextOrder->m_NextOrder == t.m_Last);
    assert(t.isSet("PA2"));
 
    assert(t.insert("CAO", "lul"));
    assert(t.insert("LIN", "F"));
    assert(t.m_Root->m_L->m_Key == "CAO");
    assert(t.m_Root->m_L->m_L == nullptr);
    assert(t.m_Root->m_L->m_R->m_Key == "LIN");
    assert(t.m_Last == t.m_Root->m_L->m_R );
    assert(t.m_Root->m_L->m_R->m_L == nullptr);
    assert(t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder = t.m_Root->m_L->m_R);
    assert(t.isSet("LIN"));
 
    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F}");
    ss.clear();
    ss.str("");
 
    assert(t.insert("SAP", "shit"));
    assert(t.m_Root->m_R->m_R->m_L->m_Key == "SAP");
    assert(t.m_Last == t.m_Root->m_R->m_R->m_L);
 
 
    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit}");
    ss.clear();
    ss.str("");
 
    assert(!t.isSet("PA3"));
    assert(t.isSet("LIN"));
    assert(t.isSet("SAP"));
 
    return 0;
}