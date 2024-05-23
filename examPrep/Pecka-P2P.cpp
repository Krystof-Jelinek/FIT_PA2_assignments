#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>


using namespace std;

class P2PMap {
    class Point;
public:
    // ctor
    P2PMap(){
        m_list = new Point*[2];
        m_listSize = 2;
        curent_last = 0;
        m_list[0] = nullptr;
        m_list[1] = nullptr;
    }
    // copy-ctor
    P2PMap(const P2PMap & in){
        m_list = new Point*[in.m_listSize];
        m_listSize = in.m_listSize;
        curent_last = in.curent_last;
        for(int i = 0; i < curent_last;i++){
            m_list[i] = new Point{in.m_list[i]->name};
        }
        for(int i = curent_last; i < m_listSize;i++){
            m_list[i] = nullptr;
        }

        for(int i = 0; i < curent_last;i++){
            for(size_t j = 0; j < in.m_list[i]->destinationsSize; j++){
                m_list[i]->insert(find(in.m_list[i]->destinations[j]->name));
            }
        }
    }
    // dtor
    ~P2PMap(){
        for(int i = 0; i < curent_last;i++){
            delete m_list[i];
        }
        delete[] m_list;
    }
    // Add(pointA, pointB)

    Point * find(const string & name){
        for(int i = 0; i < curent_last; i++){
            if(m_list[i]->name == name){
                return m_list[i];
            }
        }
        return nullptr;
    }

    void resize(){
        int new_listSize = m_listSize*2;
        Point** tmp_new_list = new Point*[new_listSize];
        for(int i = 0; i < m_listSize; i++){
            tmp_new_list[i] = m_list[i];
        }
        for(int i = m_listSize; i < new_listSize; i++){
            tmp_new_list[i] = nullptr;
        }
        delete[] m_list;
        m_list = tmp_new_list;
        m_listSize = new_listSize;
    }

    bool add(const string & first, const string & second){
        if(curent_last >= m_listSize - 1){
            resize();
        }

        if(first == second){
            return false;
        }

        Point * first_p = find(first);
        Point * second_p = find(second);
        if(!first_p){
            m_list[curent_last] = new Point{first};
            first_p = m_list[curent_last];
            curent_last++;
        }
        if(!second_p){
            m_list[curent_last] = new Point{second};
            second_p = m_list[curent_last];
            curent_last++;
        }
        bool res = first_p->insert(second_p);
        second_p->insert(first_p);
        return res;
    }

    P2PMap& operator=(const P2PMap& src) = delete;

private:
    static const int DESTINATION_MAX = 1000;

    struct Point {
        Point()
            : destinationsSize(0), name("")
        {
        }

        Point(const string & n)
            : destinationsSize(0), name(n)
        {
        }

        bool insert(Point * in){
            for(size_t i = 0; i < destinationsSize; i++){
                if(destinations[i]->name == in->name){
                    return false;
                }
            }
            if(destinationsSize == DESTINATION_MAX){
                throw invalid_argument("nelze dale vkladat");
            }
            destinations[destinationsSize] = in;
            destinationsSize++;
            return true;
        }

        size_t destinationsSize = 0;
        std::string name;
        Point* destinations[DESTINATION_MAX];
    };

    Point** m_list;
    int m_listSize;
    int curent_last;

    friend std::ostream& operator<<(std::ostream& os, const P2PMap& x);
};

std::ostream& operator<<(std::ostream& os, const P2PMap& x)
{
    for (int i = 0; i < x.curent_last; i++) {
        const P2PMap::Point& a = *x.m_list[i];
        os << a.name << " [" << a.destinationsSize << "]: ";
        for (size_t j = 0; j < a.destinationsSize; j++)
            os << (j ? ", " : "") << a.destinations[j]->name << " [" << a.destinations[j]->destinationsSize << "]";
        os << std::endl;
    }
    return os;
}

int main()
{
    std::ostringstream oss;
    auto f0 = std::make_unique<P2PMap>();

    assert(f0->add("Prague", "Berlin") == true);
    assert(f0->add("Prague", "Paris") == true);
    assert(f0->add("Berlin", "London") == true);
    assert(f0->add("Madrid", "Paris") == true);
    assert(f0->add("New York", "London") == true);
    assert(f0->add("London", "Berlin") == false);
    assert(f0->add("London", "London") == false);

    oss.str("");
    oss << *f0;
    assert(oss.str() == "Prague [2]: Berlin [2], Paris [2]\n"
                        "Berlin [2]: Prague [2], London [2]\n"
                        "Paris [2]: Prague [2], Madrid [1]\n"
                        "London [2]: Berlin [2], New York [1]\n"
                        "Madrid [1]: Paris [2]\n"
                        "New York [1]: London [2]\n");

    P2PMap f1(*f0);
    assert(f0->add("New York", "Paris") == true);
    assert(f1.add("Prague", "Los Angeles") == true);

    oss.str("");
    oss << *f0;
    assert(oss.str() == "Prague [2]: Berlin [2], Paris [3]\n"
                        "Berlin [2]: Prague [2], London [2]\n"
                        "Paris [3]: Prague [2], Madrid [1], New York [2]\n"
                        "London [2]: Berlin [2], New York [2]\n"
                        "Madrid [1]: Paris [3]\n"
                        "New York [2]: London [2], Paris [3]\n");

    f0.reset();

    oss.str("");
    oss << f1;
    assert(oss.str() == "Prague [3]: Berlin [2], Paris [2], Los Angeles [1]\n"
                        "Berlin [2]: Prague [3], London [2]\n"
                        "Paris [2]: Prague [3], Madrid [1]\n"
                        "London [2]: Berlin [2], New York [1]\n"
                        "Madrid [1]: Paris [2]\n"
                        "New York [1]: London [2]\n"
                        "Los Angeles [1]: Prague [3]\n");

    /*
    */

    return 0;
}
