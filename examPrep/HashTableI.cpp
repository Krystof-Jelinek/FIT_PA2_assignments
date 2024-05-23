#include <string>
#include <iostream>
#include <cassert>
 
using namespace std;
 
struct TItem
{
    TItem(string key, string val, TItem *nextHash, TItem *nextOrd, TItem *prevOrd)
        : m_Key(key), m_Val(val), m_NextHash(nextHash), m_NextOrder(nextOrd), m_PrevOrder(prevOrd) {}
 
    TItem(string key, string val)
        : m_Key(key), m_Val(val), m_NextHash(nullptr), m_NextOrder(nullptr), m_PrevOrder(nullptr) {}

    void kill_me_and_my_friends(){
        if(m_NextHash){
            delete m_NextHash;
        }
        delete this;
    }

    string m_Key, m_Val;
    TItem *m_NextHash, *m_NextOrder, *m_PrevOrder;
};
 
class CHash
{
public:
    CHash(int m) : m_Table(NULL), m_Size(m), m_FirstOrder(NULL), m_LastOrder(NULL)
    {
        m_Table = new TItem *[m];
        for (int i = 0; i < m; i++)
            m_Table[i] = NULL;
    }
    ~CHash()
    {
        for(size_t i = 0; i < m_Size;i++){
            if(m_Table[i]){
                m_Table[i]->kill_me_and_my_friends();
            }
        }
        //zkusit jestli tu jde i jne delete bez []
        delete[] m_Table;
    }
    CHash(CHash &) = delete;
    CHash &operator=(CHash &) = delete;
 
    bool Ins(string key, string val)
    {
        if(IsSet(key)){
            return false;
        }
        
        size_t hash = hashFn(key);
        TItem * was_there = m_Table[hash];
        m_Table[hash] = new TItem(key,val);
        m_Table[hash]->m_NextHash = was_there;

        if(m_FirstOrder == nullptr){
            m_FirstOrder = m_Table[hash];
            m_LastOrder = m_Table[hash];
        }
        else{
            m_LastOrder->m_NextOrder = m_Table[hash];
            m_Table[hash]->m_PrevOrder = m_LastOrder;
            m_LastOrder = m_Table[hash];
        }
        return true;
    }
    bool IsSet(string key)
    {
        TItem *tmp = m_Table[hashFn(key)];
        while (tmp != NULL && tmp->m_Key != key)
            tmp = tmp->m_NextHash;
        if (tmp == NULL)
            return false;
        return true;
    }
    template <typename func>
    void ForEach(func f)
    {
        TItem *tmp = m_FirstOrder;
        while (tmp != NULL)
        {
            f(tmp);
            tmp = tmp->m_NextOrder;
        }
    }
 
private:
    TItem **m_Table;
    unsigned int m_Size;
    TItem *m_FirstOrder, *m_LastOrder;
    unsigned int hashFn(string &str)
    {
        std::hash<std::string> hash_fn;
        return hash_fn(str) % m_Size;
    }
};
 
int main(int argc, char **argv)
{
    CHash hashtable(100);
    hashtable.Ins("h1", "car");
    hashtable.Ins("h1", "phone");
    hashtable.Ins("h2", "field");
    hashtable.Ins("h3", "house");
    hashtable.Ins("h4", "tree");
    hashtable.Ins("h6", "idk");
    hashtable.Ins("h25", "joo");
    hashtable.Ins("h10", "last");
    assert(hashtable.Ins("h10", "last")  == false);
    
    hashtable.ForEach([](TItem *it) {
        cout << it->m_Key << " - " << it->m_Val << endl;
    });
 
    return 0;
}