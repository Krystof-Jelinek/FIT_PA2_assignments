#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#endif /* __PROGTEST__ */

using namespace std;

string create_indentation_string(int indentation, vector<int> & connections){
  string ret;
  for(int i = 0; i < indentation; i++){
    if(find(connections.begin(), connections.end(), i) != connections.end()){
      ret += "|";
    }
    else{
      ret += " ";
    }
  }
  return ret;
}

class CThing{
  public:
  virtual string printMe(int indentation, vector<int> & connections, bool last_in_list) const = 0;

};

ostream & operator<<(ostream& os, const CThing* thing) {
    vector<int> tmp;
    if (thing) {
        os << thing->printMe(0,tmp,false);
    } else {
        os << "nullptr";
    }
    return os;
}

class CCPU : public CThing{
  public:
  int cores;
  int frequence;
  
  CCPU(int crs, int f){
    cores = crs;
    frequence = f;
  }

  string printMe(int indentation,vector<int> & connections, bool last_in_list) const override{
    string str_ind = create_indentation_string(indentation, connections);
    if(last_in_list){
      return str_ind + "\\-CPU, " + to_string(cores) + " cores @ " + to_string(frequence) + "MHz\n";
    }
    return str_ind + "+-CPU, " + to_string(cores) + " cores @ " + to_string(frequence) + "MHz\n";
  }

};

class CMemory : public CThing{
  public:
  int size;
  
  CMemory(int s){
    size = s;
  }

  string printMe(int indentation,vector<int> & connections, bool last_in_list) const override{
    string str_ind = create_indentation_string(indentation, connections);
    if(last_in_list){
      return str_ind + "\\-Memory, " + to_string(size) + " MiB\n";
    }
    return str_ind + "+-Memory, " + to_string(size) + " MiB\n";
  }
};

struct CPartition{
  int size;
  string id;
  CPartition(){}
  CPartition(const string & in, int s){
    size = s;
    id = in;
  }

  string printMe(int indentation,vector<int> & connections, bool last_in_list, int number) const{
    string str_ind = create_indentation_string(indentation, connections);
    if(last_in_list){
      return str_ind + "\\-[" + to_string(number) + "]: " + to_string(size) + " GiB, " + id + "\n";
    }
    return str_ind + "+-[" + to_string(number) + "]: " + to_string(size) + " GiB, " + id + "\n";
  }

};

class CDisk : public CThing{
  public:
  static inline const string SSD = "SSD";
  static inline const string MAGNETIC = "MAGNETIC";

  string type;
  int size;
  vector<CPartition> partitions;

  CDisk(string t, int s){
    if(t == "MAGNETIC"){
      type = "HDD";
    }
    else{
      type = t;
    }
    size = s;
  }

  string printMe(int indentation,vector<int> & connections, bool last_in_list) const override{
    string ret;
    int counter = 0;

    string string_indentation = create_indentation_string(indentation, connections);
    if(last_in_list){
      ret += string_indentation + "\\-" + type + ", " + to_string(size) + " GiB\n";
    }
    else{
      ret += string_indentation + "+-" + type + ", " + to_string(size) + " GiB\n";
    }

    int loop_counter = 0;
    vector<int> new_connections = connections;
    new_connections.push_back(indentation);

    for(const CPartition & i : partitions){
      if(loop_counter == int(partitions.size() - 1)){
      ret += i.printMe(indentation + 2,new_connections, true, counter);
      }
      else{
        ret += i.printMe(indentation + 2,new_connections, false, counter);
      }
      counter++;
      loop_counter++;
    }
    return ret;
  }

  CDisk & addPartition(int size, const string & id){
    partitions.push_back(CPartition{id, size});
    return *this;
  }

};

class CAdress{
  public:
  string adress;

  string printMe(int indentation,vector<int> & connections,bool last_in_list) const{
    string string_indentation = create_indentation_string(indentation, connections);
    if(last_in_list){
      return string_indentation + "\\-" + adress + "\n";
    }
    else{
      return string_indentation + "+-" + adress + "\n";
    }
  }

};

class CComputer : public CThing{
  public:
  vector<shared_ptr<CThing>> parts;
  vector<CAdress> addresses;
  string pc_name;

  CComputer(const string & in){
    pc_name = in;
  }

  string printMe(int indentation,vector<int> & connections,bool last_in_list) const override{
    string res;
    string string_indentation = create_indentation_string(indentation, connections);
    if(last_in_list){
      res += string_indentation + "\\-Host: " + pc_name + "\n";
    }
    else{
      res += string_indentation + "+-Host: " + pc_name + "\n";
    }
    
    for(const CAdress & j : addresses){
      res += j.printMe(indentation +2, connections, false); //TODO tohle se rozbije pokud bude mit pocitac pouze adresy
    }
    int counter = 0;
    vector<int> new_connections = connections;
    new_connections.push_back(indentation);

    for(const shared_ptr<CThing> & i : parts){
      if(counter == int(parts.size()) - 1){
        res += i->printMe(indentation + 2, connections, true);
        continue;
      }
      res += i->printMe(indentation + 2, new_connections, false);
      counter++;
    }
    return res;
  }

  friend ostream & operator << (ostream & ostream, const CComputer & in){
    ostream << &in;
    return ostream;
  }

  CComputer & addAddress(const string & in){
    CAdress tmp;
    tmp.adress = in;
    addresses.push_back(tmp);
    return *this;
  }

  CComputer & addComponent(const CCPU & in){
    parts.push_back(make_shared<CCPU>(in));
    return *this;
  }

  CComputer & addComponent(const CDisk & in){
    parts.push_back(make_shared<CDisk>(in));
    return *this;
  }

  CComputer & addComponent(const CMemory & in){
    parts.push_back(make_shared<CMemory>(in));
    return *this;
  }

  
};

class CNetwork : public CThing{
  public:
  vector<CComputer> computers;
  string network_name;

  CNetwork(const string & in){
    network_name = in;
  }

  string printMe(int indentation,vector<int> & connections,bool last_in_list) const override{
    string ret;
    int counter = 0;
    vector<int> new_connections = connections;
    new_connections.push_back(indentation);

    for(const CComputer & i : computers){
      if(counter == int(computers.size()) - 1){
        ret += i.printMe(indentation, connections, true);
        continue;
      }
      ret += i.printMe(indentation, new_connections, false);
      counter++;
    }
    return ret;
  }

  friend ostream & operator << (ostream & ostream, const CNetwork & in){
    ostream << &in;
    return ostream;
  }

  CNetwork & addComputer(const CComputer & in){
    computers.push_back(in);
    return *this;
  }

  CComputer * findComputer(const string & name){
    for(CComputer & i : computers){
      if(i.pc_name == name){
        return &i;
      }
    }
    return nullptr;
  }

};


#ifndef __PROGTEST__
template<typename T_>
std::string toString ( const T_ & x )
{
  std::ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ()
{
  /*
  CNetwork net("mojesit");
  CComputer pc("mojepc");
  pc.addComponent(CDisk(CDisk::MAGNETIC, 20));
  pc.addComponent(CMemory(20));

  cout << &pc << endl;

  net.addComputer(pc);
  cout << net << endl;

  CDisk disk(CDisk::MAGNETIC, 20);
  cout << &disk << endl;
  */

  CNetwork n ( "FIT network" );
  n . addComputer (
        CComputer ( "progtest.fit.cvut.cz" ) .
          addAddress ( "147.32.232.142" ) .
          addComponent ( CCPU ( 8, 2400 ) ) .
          addComponent ( CCPU ( 8, 1200 ) ) .
          addComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
            addPartition ( 50, "/" ) .
            addPartition ( 5, "/boot" ).
            addPartition ( 1000, "/var" ) ) .
          addComponent ( CDisk ( CDisk::SSD, 60 ) .
            addPartition ( 60, "/data" )  ) .
          addComponent ( CMemory ( 2000 ) ).
          addComponent ( CMemory ( 2000 ) ) ) .
      addComputer (
        CComputer ( "courses.fit.cvut.cz" ) .
          addAddress ( "147.32.232.213" ) .
          addComponent ( CCPU ( 4, 1600 ) ) .
          addComponent ( CMemory ( 4000 ) ).
          addComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
            addPartition ( 100, "/" )   .
            addPartition ( 1900, "/data" ) ) ) .
      addComputer (
        CComputer ( "imap.fit.cvut.cz" ) .
          addAddress ( "147.32.232.238" ) .
          addComponent ( CCPU ( 4, 2500 ) ) .
          addAddress ( "2001:718:2:2901::238" ) .
          addComponent ( CMemory ( 8000 ) ) );
  
  cout << toString ( n );

  /*
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  */
  /*
  CNetwork x = n;
  auto c = x . findComputer ( "imap.fit.cvut.cz" );
  string coee = toString(*c);
  cout << toString ( *c );
  assert ( toString ( *c ) ==
    "Host: imap.fit.cvut.cz\n"
    "+-147.32.232.238\n"
    "+-2001:718:2:2901::238\n"
    "+-CPU, 4 cores @ 2500MHz\n"
    "\\-Memory, 8000 MiB\n" );

  c -> addComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
         addPartition ( 100, "system" ) .
         addPartition ( 200, "WWW" ) .
         addPartition ( 700, "mail" ) );
  assert ( toString ( x ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  +-Memory, 8000 MiB\n"
    "  \\-HDD, 1000 GiB\n"
    "    +-[0]: 100 GiB, system\n"
    "    +-[1]: 200 GiB, WWW\n"
    "    \\-[2]: 700 GiB, mail\n" );
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );

  */

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
