#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#endif /* __PROGTEST__ */

using namespace std;

template <typename DT>
class myvector{
  public:
  size_t capacity;
  size_t size;
  DT * arr;

  myvector(){
    capacity = 2;
    size = 0;
    arr = new DT[capacity];
  }

  ~myvector() {
    delete[] arr;
  }

  void resize() {
    capacity *= 2;
    DT *temp = new DT[capacity];
    for (size_t i = 0; i < size; ++i) {
        temp[i] = arr[i];
    }
    delete[] arr;
    arr = temp;
  }

  void push_back(const DT & element) {
    if (size >= capacity) {
      resize();
    }
    arr[size] = element;
    size++;
  }

  DT & operator[](size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index in vector out of range");
    }
    return arr[index];
  }

  DT& at(size_t index) const{
    if (index >= size) {
      throw std::out_of_range("Index in vector out of range");
    }
    return arr[index];
  }

  void insert(size_t index, const DT &element) {
    if (index > size) {
      throw std::out_of_range("Index in vector out of range");
    }
    if (size >= capacity) {
      resize();
    }
      // Shift elements to make space for the new element
    for (size_t i = size; i > index; --i) {
      arr[i] = arr[i - 1];
    }
    arr[index] = element;
    size++;
  }

  void erase(size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index in vector out of range");
    }
    // Shift elements to overwrite the element at the specified index
    for (size_t i = index; i < size - 1; ++i) {
      arr[i] = arr[i + 1];
    }
    size--;
  }

  myvector(const myvector& other) : capacity(other.capacity), size(other.size), arr(new DT[other.capacity]) {
    for (size_t i = 0; i < other.size; ++i) {
      arr[i] = other.arr[i];
    }
  }

  myvector& operator=(const myvector& other) {
    if (this != &other) {
      delete[] arr;
      capacity = other.capacity;
      size = other.size;
      arr = new DT[capacity];
      for (size_t i = 0; i < other.size; ++i) {
        arr[i] = other.arr[i];
      }
    }
    return *this;
  }

};

class CBaseString{
  public:
  char * value;
  int length;

  CBaseString(){
    value = new char[1];
    value[0] = '\0';
    length = 0;
  }

  CBaseString(const char * in){
    length = strlen(in);
    value = new char[length + 1];
    strcpy(value, in);
  }

  CBaseString(const CBaseString & in){
    length = in.length;
    value = new char[length + 1];
    strcpy(value, in.value);
  }

  CBaseString & operator =(const CBaseString & in){
    if (this != &in) { // Check for self-assignment
      delete[] value; // Deallocate old memory
      length = in.length;
      value = new char[length + 1]; // Allocate new memory
      strcpy(value, in.value); // Copy content
    }
    return *this;
  }

  ~CBaseString() {
    //cout << "Destroyed:" << value << endl;
    delete[] value;
  }

};

class CStringPart{
  public:
  int begin;
  int end;
  int length;
  size_t chars_before_me = 0;
  shared_ptr<CBaseString> basestring;

  CStringPart(){
    begin = 0;
    end = 0;
    length = 0;
    basestring = make_shared<CBaseString>();
  }

  CStringPart(const char * in){
    basestring = make_shared<CBaseString>(in);
    begin = 0;
    end = basestring.get()->length - 1;
    length = basestring.get()->length;
  }

  CStringPart(const CStringPart & in){
    begin = in.begin;
    end = in.end;
    length = in.length;
    basestring = in.basestring;
    chars_before_me = in.chars_before_me;
  }

  size_t get_used_lenght() const{
    return end - begin + 1;
  }

  CStringPart& operator=(const CStringPart &in) {
    if (this != &in) { // Check for self-assignment
      begin = in.begin;
      end = in.end;
      length = in.length;
      basestring = in.basestring;
      chars_before_me = in.chars_before_me;
    }
    return *this;
  }

};

//vraci index prvniho prvku který pred sebou má stejne nebo více než index prvků
size_t my_lower_bound(const myvector<CStringPart>& vec, size_t index) {
    size_t lo = 0;
    size_t hi = vec.size;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (vec.arr[mid].chars_before_me + vec.arr[mid].get_used_lenght() < index) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}


class CPatchStr
{
  public:

    myvector<CStringPart> field;
    size_t string_length;
    bool empty;

    CPatchStr (){
      CStringPart tmp;
      field.push_back(tmp);
      empty = true;
      string_length = 0;
    }
    
    CPatchStr ( const char * str ){
      CStringPart tmp(str);
      field.push_back(tmp);
      string_length = tmp.length;
      if(tmp.length > 0){
        empty = false;
      }
      else{
        empty = true;
      }
    }

    CPatchStr(const CPatchStr & in){
      field = in.field;
      empty = in.empty;
      string_length = in.string_length;
    }

    CPatchStr & operator = (const CPatchStr & in){
      if(this != &in){
        this->field = in.field;
        this->empty = in.empty;
        this->string_length = in.string_length;
      }
      return *this;
    }

    void update_chars_before(){
      if(empty){
        return;
      }
      for(size_t i = 0; i < field.size; i++){
        if(i == 0){
          field[0].chars_before_me = 0;
          continue;
        }
        field[i].chars_before_me = field[i-1].chars_before_me + field[i-1].get_used_lenght();
      }
    }

    void insertPatchintovector(size_t position,const CPatchStr & in){
      if(in.empty){
        return;
      }
      if(position == field.size){
        for(size_t i = 0; i < in.field.size; i++){
          field.push_back(in.field.at(i));
          string_length += in.field.at(i).get_used_lenght();
        }
        return;
      }
      for(size_t i = 0; i < field.size; i++){
        if(i == position){
          size_t j;
          for(j = in.field.size - 1;j > 0; j--){
            field.insert(position, in.field.at(j));
            string_length += in.field.at(j).get_used_lenght();
          }
          if(j == 0){
            field.insert(position, in.field.at(0));
            string_length += in.field.at(0).get_used_lenght();
            return;
          }
        }
      }
    }
    
    //vraci index prvniho prvku který pred sebou má stejne nebo více než index prvků
    size_t find_index(size_t from) const{
      return my_lower_bound(field, from);
    }

    CPatchStr   subStr    ( size_t            from,
                            size_t            len ) const
    {
      if(from + len > string_length){
        throw std::out_of_range("Index in substr out of range");
      }
      if(len == 0){
        CPatchStr tmp;
        return tmp;
      }
      CPatchStr res;
      size_t i = find_index(from+1);
      from = from - field.at(i).chars_before_me;

      //for(size_t i = 0; i < field.size; i++){
        //if(from + 1<= field.at(i).get_used_lenght()){
          CStringPart begin_part = field.at(i);
          begin_part.begin += from;
          //pokud zustanu uvnitr jednoho stringu
          if(begin_part.get_used_lenght() >= len){
            size_t difference = begin_part.get_used_lenght() - len;
            begin_part.end = begin_part.end - difference;
            res.field.erase(0);
            res.field.push_back(begin_part);
            res.empty = false;
            res.string_length += begin_part.get_used_lenght();
            return res;
          }
          //budu iterovat pres vice stirngu
          res.field.erase(0);
          res.field.push_back(begin_part);
          res.empty = false;
          res.string_length += begin_part.get_used_lenght();
          len = len - begin_part.get_used_lenght();
          while(true){
            i++;
            //ukoncujici
            if(field.at(i).get_used_lenght() > len){
              CStringPart tmp = field.at(i);
              tmp.end = tmp.begin + len - 1;
              res.field.push_back(tmp);
              res.string_length += tmp.get_used_lenght();
              return res;
            }
            else{
              CStringPart tmp = field.at(i);
              res.field.push_back(tmp);
              res.string_length += tmp.get_used_lenght();
              len = len - tmp.get_used_lenght();
            }
            //ukoncujici podminka na rovnost
            if(len == 0){
              return res;
            }
          }
        //}
        //from = from - field.at(i).get_used_lenght();
      //}
      throw std::out_of_range("exited substr for cycle thats rly wrong");      
    }

    CPatchStr & append    ( const CPatchStr & src ){
      if(src.empty == true){
        return *this;
      }
      if(this->empty && !src.empty){
        this->field.size = 0;
        this->empty = false;
      }
      if(this == &src){
        CPatchStr use = src;
        for(size_t i = 0; i < use.field.size; i++){
        this->field.push_back(use.field.at(i));
        this->string_length += use.field.at(i).get_used_lenght();
        }
        update_chars_before();
        return *this;
      }
      for(size_t i = 0; i < src.field.size; i++){
        this->field.push_back(src.field.at(i));
        this->string_length += src.field.at(i).get_used_lenght();
      }
      update_chars_before();
      return *this;
    }

    CPatchStr & do_tricky_insert( size_t            pos,
                            const CPatchStr src )
    {
      size_t counter = 0;
      for(size_t i = 0; i <= field.size; i++){
        if(counter == pos){
          insertPatchintovector(i, src);
          update_chars_before();
          return *this;
        }
        if(counter > pos){
          size_t difference = counter - pos;
          CStringPart tmp = field.at(i-1);
          field.insert(i-1, tmp);
          field[i-1].end = field[i-1].end - difference;
          field[i].begin = field[i].end - (difference - 1);
          insertPatchintovector(i,src);
          update_chars_before();
          return *this;
        }
        counter += field[i].get_used_lenght();
      }

      throw std::out_of_range("skipped for cycle in insert that shouldnt happen");
    }

    CPatchStr & insert    ( size_t            pos,
                            const CPatchStr & src )
    {
      if(pos > string_length){
        throw std::out_of_range("Index in insert out of range");
      }
      if(src.empty){
        return *this;
      }
      if(pos == string_length){
        append(src);
        update_chars_before();
        return *this;
      }
      if(this == &src){
        CPatchStr copy = src;
        return do_tricky_insert(pos, copy);
      }
      size_t counter = 0;
      for(size_t i = 0; i <= field.size; i++){
        if(counter == pos){
          insertPatchintovector(i, src);
          update_chars_before();
          return *this;
        }
        if(counter > pos){
          size_t difference = counter - pos;
          CStringPart tmp = field.at(i-1);
          field.insert(i-1, tmp);
          field[i-1].end = field[i-1].end - difference;
          field[i].begin = field[i].end - (difference - 1);
          insertPatchintovector(i,src);
          update_chars_before();
          return *this;
        }
        counter += field[i].get_used_lenght();
      }

      throw std::out_of_range("skipped for cycle in insert that shouldnt happen");
    }


    CPatchStr & remove    ( size_t            from,
                            size_t            len )
    {
      if(from + len > string_length){
        throw std::out_of_range("Index in remove out of range");
      }
      if(len == 0){return *this;}
      if(len == string_length){
        CPatchStr tmp;
        *this = tmp;
        update_chars_before();
        return *this;        
      }
      if(from == 0){
        return removefrombegining(len);
      }

      size_t current_index = 0;
      for(size_t i = 0; i <= field.size;i++){
        if(current_index >= from){
        //zacal jsem presne na zacatku nejakyho slova
          if(current_index == from){

            //cout << field[i].basestring.get()->value << endl;
            //cout << field[i].begin << endl;
            //cout << field[i].end << endl;
            //cout << field[i].get_used_lenght() << endl;

            //pokud zustanu uvnitr jednoho stringu
            if(size_t(field[i].get_used_lenght()) >= len){
              field[i].begin += len;
              if(field[i].begin > field[i].end){
                field.erase(i);
              }
              string_length = string_length - len;
              update_chars_before();
              return *this;
            }
            while(true){
              if(field[i].get_used_lenght() > len){
                break;
              }
              len = len - field[i].get_used_lenght();
              string_length = string_length - field[i].get_used_lenght();
              field.erase(i);
              if(len == 0){
                break;
              }
            }
            if(len != 0){
              field[i].begin += len;
              string_length = string_length - len;
            }
            update_chars_before();
            return *this;
          }
          else{
            current_index = current_index - from;
            size_t before_end = field[i-1].end;
            size_t new_end = field[i-1].end - current_index;
            
            //pokud budu muset rozpulit interval
            if(before_end - new_end > len){
              CStringPart tmp =  field[i-1];
              field.insert(i-1, tmp);
              field[i-1].end = new_end;
              field[i].begin = new_end + len + 1;
              update_chars_before();
              return *this;
            }

            field[i-1].end = field[i-1].end - current_index;
            len = len - (before_end - field[i-1].end);
            string_length = string_length - (before_end - field[i-1].end);
            //pokud staci jen tento domazat do konce jeden string
            if(0 == len){
              update_chars_before();
              return *this;
            }
            while(true){
              if(field[i].get_used_lenght() > len){
                break;
              }
              len = len - field[i].get_used_lenght();
              string_length = string_length - field[i].get_used_lenght();
              field.erase(i);
              if(len == 0){
                break;
              }
            }
            if(len != 0){
              field[i].begin += len;
              string_length = string_length - len;
            }
            update_chars_before();
            return *this;
          }
        }
        current_index += field[i].get_used_lenght();
      }
      throw std::out_of_range("got out of for cycle in remove");
    }

    //only use when from == 0
    CPatchStr & removefrombegining(size_t len){
      if(field[0].get_used_lenght() == len){
        string_length = string_length - field[0].get_used_lenght();
        field.erase(0);
        if(string_length == 0){
          empty = true;
        }
        update_chars_before();
        return *this;
      }
      if(field[0].get_used_lenght() > len){
        field[0].begin += len; // PLUS
        string_length = string_length - len;
        update_chars_before();
        return *this;
      }
      int i = 0;
      while(true){
        if(field[i].get_used_lenght() > len){
          break;
        }
        len = len - field[i].get_used_lenght();
        string_length = string_length - field[i].get_used_lenght();
        field.erase(i);
        if(len == 0){
          update_chars_before();
          return *this;
        }
      }
      if(len != 0){
      field[i].begin += len;
      string_length = string_length - len;
      }
      update_chars_before();
      return *this;
    }

    char      * toStr     () const{
      if(empty){
        char * tmp = new char[1];
        tmp[0] = '\0';
        return tmp;
      }
      char * res = new char[string_length + 1];
      size_t str_index = 0;
      for(size_t i = 0; i < field.size; i++){
        CStringPart & curent = field.at(i);
        for(int j = curent.begin; j <= curent.end; j++){
          res[str_index] = curent.basestring.get()->value[j];
          str_index++;
        }
      }
      res[str_index] = '\0';
      return res;
    }
};

#ifndef __PROGTEST__
bool stringMatch ( char       * str,
                   const char * expected )
{
  bool res = std::strcmp ( str, expected ) == 0;
  delete [] str;
  return res;
}

void reset(CPatchStr & input){
  CPatchStr test("0123");
  CPatchStr in("4567");
  CPatchStr in2("89AB");
  CPatchStr in3("CDEF");
  CPatchStr in4("GHIJ");
  
  test.append(in).append(in2).append(in3).append(in4);
  input = test;
}

int main ()
{
  /*
  myvector<CStringPart> test;
  CStringPart a("tmp");
  CStringPart b("BBB");
  CStringPart c("CCC");
  CStringPart d("DDD");

  test.push_back(a);
  test.push_back(b);
  test.push_back(c);
  cout << test[2].basestring.get()->value << endl;
  test.insert(0, d);
  cout << test[0].basestring.get()->value << endl;
  cout << test[2].basestring.get()->value << endl;
  test.erase(0);
  cout << test[0].basestring.get()->value << endl;
  */

  /*CPatchStr tmp("ABCD");
  CPatchStr e("E");
  CPatchStr f("F");
  CPatchStr x("");


  e.append(f);

  tmp.append(e);
  tmp.append(x);

  x.append(e);*/
  /*
  CPatchStr tmp("ABE");
  CPatchStr three("F");

  CPatchStr one("C");
  CPatchStr two("D");
  one.append(two);


  tmp.append(three);
  tmp.insert(2, one);

  CPatchStr test("12GH");
  test.insert(1, tmp);
  */

  /*CPatchStr test("12GH");

  test.remove(0,2);*/

  CPatchStr test("0123");
  CPatchStr in("4567");
  CPatchStr in2("89AB");
  CPatchStr in3("CDEF");
  CPatchStr in4("GHIJ");
  
  test.append(in).append(in2).append(in3).append(in4);
  test.remove(0,20);
  test.remove(0,0);
  reset(test);

  cout << my_lower_bound(test.field, 13) << endl;


  reset(test);
  //from beginig tests
  test.remove(0,0);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(0,2);
  assert(stringMatch(test.toStr(),"23456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(0,4);
  assert(stringMatch(test.toStr(),"456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(0,6);
  assert(stringMatch(test.toStr(),"6789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(0,14);
  assert(stringMatch(test.toStr(),"EFGHIJ") == true);
  reset(test);
  test.remove(0,16);
  assert(stringMatch(test.toStr(),"GHIJ") == true);
  reset(test);
  test.remove(0,20);
  assert(stringMatch(test.toStr(),"") == true);
  reset(test);

  //from clean start position
  test.remove(4,1);
  assert(stringMatch(test.toStr(),"012356789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(4,3);
  assert(stringMatch(test.toStr(),"0123789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(4,4);
  assert(stringMatch(test.toStr(),"012389ABCDEFGHIJ") == true);
  reset(test);
  test.remove(4,5);
  assert(stringMatch(test.toStr(),"01239ABCDEFGHIJ") == true);
  reset(test);
  test.remove(4,8);
  assert(stringMatch(test.toStr(),"0123CDEFGHIJ") == true);
  reset(test);
  test.remove(4,11);
  assert(stringMatch(test.toStr(),"0123FGHIJ") == true);
  reset(test);
  test.remove(4,16);
  assert(stringMatch(test.toStr(),"0123") == true);
  reset(test);
  test.remove(8,12);
  assert(stringMatch(test.toStr(),"01234567") == true);
  reset(test);
  test.remove(8,0);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(8,2);
  assert(stringMatch(test.toStr(),"01234567ABCDEFGHIJ") == true);
  reset(test);
  test.remove(16,2);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFIJ") == true);
  reset(test);
  test.remove(16,4);
  assert(stringMatch(test.toStr(),"0123456789ABCDEF") == true);
  reset(test);

  //from middle of field
  test.remove(1,1);
  assert(stringMatch(test.toStr(),"023456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(1,2);
  assert(stringMatch(test.toStr(),"03456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(1,3);
  assert(stringMatch(test.toStr(),"0456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(2,4);
  assert(stringMatch(test.toStr(),"016789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(1,0);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHIJ") == true);
  reset(test);
  test.remove(5,11);
  assert(stringMatch(test.toStr(),"01234GHIJ") == true);
  reset(test);
  test.remove(6,2);
  assert(stringMatch(test.toStr(),"01234589ABCDEFGHIJ") == true);
  reset(test);
  test.remove(7,13);
  assert(stringMatch(test.toStr(),"0123456") == true);
  reset(test);
  test.remove(11,1);
  assert(stringMatch(test.toStr(),"0123456789ACDEFGHIJ") == true);
  reset(test);
  test.remove(17,3);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFG") == true);
  reset(test);
  test.remove(19,1);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHI") == true);
  reset(test);

  //insert tests
  test.insert(20,CPatchStr("XX"));
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHIJXX") == true);
  reset(test);
  test.insert(0,CPatchStr("XX"));
  assert(stringMatch(test.toStr(),"XX0123456789ABCDEFGHIJ") == true);
  reset(test);
  test.insert(2,CPatchStr("XX"));
  assert(stringMatch(test.toStr(),"01XX23456789ABCDEFGHIJ") == true);
  reset(test);
  test.insert(4,CPatchStr("XX"));
  assert(stringMatch(test.toStr(),"0123XX456789ABCDEFGHIJ") == true);
  reset(test);
  test.insert(18,CPatchStr("XX"));
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHXXIJ") == true);
  test.remove(18,2);
  assert(stringMatch(test.toStr(),"0123456789ABCDEFGHIJ") == true);
  reset(test);

  //substr tests
  CPatchStr res;

  res = test.subStr(0,0);
  assert(stringMatch(res.toStr(),"") == true);
  res = test.subStr(0,1);
  assert(stringMatch(res.toStr(),"0") == true);
  res = test.subStr(0,2);
  assert(stringMatch(res.toStr(),"01") == true);
  res = test.subStr(0,4);
  assert(stringMatch(res.toStr(),"0123") == true);
  res = test.subStr(9,1);
  assert(stringMatch(res.toStr(),"9") == true);
  res = test.subStr(9,2);
  assert(stringMatch(res.toStr(),"9A") == true);
  res = test.subStr(9,3);
  assert(stringMatch(res.toStr(),"9AB") == true);
  res = test.subStr(0,1);
  assert(stringMatch(res.toStr(),"0") == true);
  res = test.subStr(0,3);
  assert(stringMatch(res.toStr(),"012") == true);
  res = test.subStr(0,4);
  assert(stringMatch(res.toStr(),"0123") == true);
  res = test.subStr(0,6);
  assert(stringMatch(res.toStr(),"012345") == true);
  res = test.subStr(0,8);
  assert(stringMatch(res.toStr(),"01234567") == true);
  res = test.subStr(0,16);
  assert(stringMatch(res.toStr(),"0123456789ABCDEF") == true);
  res = test.subStr(0,18);
  assert(stringMatch(res.toStr(),"0123456789ABCDEFGH") == true);
  res = test.subStr(0,20);
  assert(stringMatch(res.toStr(),"0123456789ABCDEFGHIJ") == true);
  res = test.subStr(2,4);
  assert(stringMatch(res.toStr(),"2345") == true);
  res = test.subStr(3,9);
  assert(stringMatch(res.toStr(),"3456789AB") == true);
  res = test.subStr(3,12);
  assert(stringMatch(res.toStr(),"3456789ABCDE") == true);
  res = test.subStr(3,13);
  assert(stringMatch(res.toStr(),"3456789ABCDEF") == true);
  res = test.subStr(3,17);
  assert(stringMatch(res.toStr(),"3456789ABCDEFGHIJ") == true);
  res = test.subStr(5,1);
  assert(stringMatch(res.toStr(),"5") == true);
  res = test.subStr(5,2);
  assert(stringMatch(res.toStr(),"56") == true);
  res = test.subStr(5,3);
  assert(stringMatch(res.toStr(),"567") == true);
  res = test.subStr(8,1);
  assert(stringMatch(res.toStr(),"8") == true);
  res = test.subStr(8,3);
  assert(stringMatch(res.toStr(),"89A") == true);
  res = test.subStr(8,4);
  assert(stringMatch(res.toStr(),"89AB") == true);
  res = test.subStr(8,6);
  assert(stringMatch(res.toStr(),"89ABCD") == true);
  res = test.subStr(8,10);
  assert(stringMatch(res.toStr(),"89ABCDEFGH") == true);
  res = test.subStr(8,12);
  assert(stringMatch(res.toStr(),"89ABCDEFGHIJ") == true);
  test.remove(0,2);
  res = test.subStr(1,2);
  assert(stringMatch(res.toStr(),"34") == true);
  reset(test);

  //other tests
  test = CPatchStr();
  assert(stringMatch(test.toStr(),"") == true);
  assert(test.empty == true);
  test.remove(0, 0);
  assert(stringMatch(test.toStr(),"") == true);
  assert(test.empty == true);
  test.insert(0, CPatchStr("A"));
  assert(stringMatch(test.toStr(),"A") == true);
  assert(test.empty == false);
  test.remove(0, 1);
  assert(stringMatch(test.toStr(),"") == true);
  assert(test.empty == true);
  test.insert(0,test);
  assert(stringMatch(test.toStr(),"") == true);
  assert(test.empty == true);
  test.insert(0, CPatchStr("A"));
  test.insert(0,CPatchStr());
  test.insert(1,CPatchStr());
  assert(stringMatch(test.toStr(),"A") == true);
  assert(stringMatch(test.subStr(1,0).toStr(), "") == true);
  assert(stringMatch(test.subStr(0,1).toStr(), "A") == true);
  test.remove(0,1);
  assert(test.empty == true);
  test.insert(0, CPatchStr("A"));
  assert(stringMatch(test.subStr(0,1).toStr(), "A") == true);
  assert(stringMatch(test.toStr(),"A") == true);
  test.insert(1, CPatchStr("B"));
  assert(stringMatch(test.subStr(0,1).toStr(), "A") == true);
  assert(stringMatch(test.toStr(),"AB") == true);
  assert(stringMatch(test.subStr(1,1).toStr(), "B") == true);
  assert(stringMatch(test.subStr(0,2).toStr(), "AB") == true);


  char tmpStr[100];

  CPatchStr a ( "test" );
  assert ( stringMatch ( a . toStr (), "test" ) );
  std::strncpy ( tmpStr, " da", sizeof ( tmpStr ) - 1 );
  a . append ( tmpStr );
  assert ( stringMatch ( a . toStr (), "test da" ) );
  std::strncpy ( tmpStr, "ta", sizeof ( tmpStr ) - 1 );
  a . append ( tmpStr );
  assert ( stringMatch ( a . toStr (), "test data" ) );
  std::strncpy ( tmpStr, "foo text", sizeof ( tmpStr ) - 1 );
  CPatchStr b ( tmpStr );
  assert ( stringMatch ( b . toStr (), "foo text" ) );
  CPatchStr c ( a );
  assert ( stringMatch ( c . toStr (), "test data" ) );
  CPatchStr d ( a . subStr ( 3, 5 ) );
  assert ( stringMatch ( d . toStr (), "t dat" ) );
  d . append ( b );
  assert ( stringMatch ( d . toStr (), "t datfoo text" ) );
  d . append ( b . subStr ( 3, 4 ) );
  assert ( stringMatch ( d . toStr (), "t datfoo text tex" ) );
  c . append ( d );
  assert ( stringMatch ( c . toStr (), "test datat datfoo text tex" ) );
  c . append ( c );
  assert ( stringMatch ( c . toStr (), "test datat datfoo text textest datat datfoo text tex" ) );
  d . insert ( 2, c . subStr ( 6, 9 ) );
  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
  b = "abcdefgh";
  assert ( stringMatch ( b . toStr (), "abcdefgh" ) );
  assert ( stringMatch ( d . toStr (), "t atat datfdatfoo text tex" ) );
  assert ( stringMatch ( d . subStr ( 4, 8 ) . toStr (), "at datfd" ) );
  assert ( stringMatch ( b . subStr ( 2, 6 ) . toStr (), "cdefgh" ) );
  try
  {
    b . subStr ( 2, 7 ) . toStr ();
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::out_of_range & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  a . remove ( 3, 5 );
  assert ( stringMatch ( a . toStr (), "tesa" ) );


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
