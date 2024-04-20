#ifndef __PROGTEST__
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>
using namespace std::literals;

class CDummy
{
  public:
                                       CDummy                                  ( char                                  c )
      : m_C ( c )
    {
    }
    bool                               operator ==                             ( const CDummy                        & other ) const = default;
    friend std::ostream              & operator <<                             ( std::ostream                        & os,
                                                                                 const CDummy                        & x )
    {
      return os << '\'' << x . m_C << '\'';
    }
  private:
    char                               m_C;
};
#endif /* __PROGTEST__ */

//#define TEST_EXTRA_INTERFACE

using namespace std;

void generateCombinations(const std::vector<size_t>& elements, size_t k, size_t start,
                          std::vector<size_t>& current, std::vector<std::vector<size_t>>& result) {
    if (current.size() == k) {
        result.push_back(current);
        return;
    }

    for (size_t i = start; i < elements.size(); ++i) {
        current.push_back(elements[i]);

        generateCombinations(elements, k, i + 1, current, result);

        current.pop_back();

        while (i + 1 < elements.size() && elements[i] == elements[i + 1]) {
            ++i;
        }
    }
}

std::vector<std::vector<size_t>> generateCombinationsOfSizeK(const std::vector<size_t>& elements, size_t k) {
    std::vector<std::vector<size_t>> result;
    std::vector<size_t> current;

    std::vector<size_t> sortedElements = elements;
    std::sort(sortedElements.begin(), sortedElements.end());

    generateCombinations(sortedElements, k, 0, current, result);

    return result;
}


template <typename T_>
class CSelfMatch
{
  public:
    // constructor (initializer list)
    CSelfMatch(initializer_list<T_> initList)
        : sequence(initList)
    {
    }

    // constructor (2 iterators)
    template <typename InputIt>
    CSelfMatch(InputIt first, InputIt last)
        : sequence(first, last)
    {
    }
    // constructor (a container)
    template <typename Container>
    CSelfMatch(const Container& cont)
        : sequence(cont.begin(), cont.end())
    {
    }

    // optionally: push_back
    void push_back(const T_& value){
      sequence.emplace_back(value);
    }

    int count_apperance(const vector<T_> & in) const{
      size_t in_length = in.size();
      int count = 0;

      for(auto itr = sequence.begin(); (itr + in_length - 1)!= sequence.end(); itr++){
        auto end = itr + in_length;
        if(equal(itr,end, in.begin(), in.end())){
          count++;
        }
      }
      return count;
    }

    size_t sequenceLen (int n) const{
      int curent_len = sequence.size();
      if(curent_len == 0){
        return 0;
      }

      while(curent_len >= 0){
        for(auto itr = sequence.begin(); (itr + curent_len - 1) != sequence.end(); itr++){
          auto end = itr + curent_len;
          int apperance = count_apperance(vector<T_>(itr,end));
          if(apperance >= n){
            return curent_len;
          }
        }
        curent_len--;
      }
      return 0;
    }


    template<size_t N_> std::vector<std::array<size_t, N_>> findSequences () const{
      size_t search_len = sequenceLen(N_);

      vector<vector<T_>> stored_seq;
      vector<array<size_t, N_>> res;

      if(search_len == 0){
        return res;
      }

      //finding all unique subsequences that have maximal length and storing them
      for(auto itr = sequence.begin(); (itr + search_len - 1) != sequence.end(); itr++){
        auto end = itr + search_len;
        int apperance = count_apperance(vector<T_>(itr,end));
        if(apperance >= int(N_)){
          bool exists = false;
          for (const auto& i : stored_seq) {
            if (i == vector<T_>(itr,end)) {
              exists = true;
              break;
            }
          }
          if(!exists){
            stored_seq.push_back(vector<T_>(itr, end));
          }
        }
      }

      //goes throuh all stored sequences and finds their begin indexes then it generates combinations of these indexes
      for(const auto & seq : stored_seq){
        vector<size_t> tmp_positions;
        for(auto itr = sequence.begin(); (itr + search_len - 1) != sequence.end(); itr++){
          auto end = itr + search_len;
          if(vector<T_>(itr,end) == seq){
            tmp_positions.push_back(distance(sequence.begin(), itr));
          }
        }
        vector<vector<size_t>> tmp = generateCombinationsOfSizeK(tmp_positions, N_);
        for(auto & i : tmp){
          array<size_t, N_> pridam = {0};
          int index = 0;
          for(size_t cur : i){
            pridam[index] = cur;
            index++;
          }
          res.push_back(pridam);
        }
      }
      sort(res.begin(), res.end());
      return res;

    }
  
  vector<T_> sequence;
  
  private:
    // todo
};



#ifndef __PROGTEST__
template<size_t N_>
bool                                   positionMatch                           ( std::vector<std::array<size_t, N_>> a,
                                                                                 std::vector<std::array<size_t, N_>> b )
{
  std::sort ( a . begin (), a . end () );
  std::sort ( b . begin (), b . end () );
  return a == b;
}
int                                    main ()
{
  CSelfMatch<char> r ( "abcdXabcdeYabcdZabcd"s );
  assert ( r . sequenceLen ( 2 ) == 4 );
  r.findSequences<2> ();
  assert ( positionMatch ( r . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 5 }, { 0, 11 }, { 0, 16 }, { 5, 11 }, { 5, 16 }, { 11, 16 } } ) );


  CSelfMatch<char> x0 ( "aaaaaaaaaaa"s );
  //vector<char> tmp = {'x'};
  assert ( x0 . sequenceLen ( 2 ) == 10 );
  x0.findSequences<2>();
  assert ( positionMatch ( x0 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 1 } } ) );
  CSelfMatch<char> x1 ( "abababababa"s );
  //cout << x1.sequenceLen(2) << endl;
  assert ( x1 . sequenceLen ( 2 ) == 9 );
  assert ( positionMatch ( x1 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 2 } } ) );
  CSelfMatch<char> x2 ( "abababababab"s );
  assert ( x2 . sequenceLen ( 2 ) == 10 );
  assert ( positionMatch ( x2 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 2 } } ) );
  CSelfMatch<char> x3 ( "aaaaaaaaaaa"s );
  assert ( x3 . sequenceLen ( 3 ) == 9 );
  assert ( positionMatch ( x3 . findSequences<3> (), std::vector<std::array<size_t, 3> > { { 0, 1, 2 } } ) );
  CSelfMatch<char> x4 ( "abababababa"s );
  assert ( x4 . sequenceLen ( 3 ) == 7 );
  assert ( positionMatch ( x4 . findSequences<3> (), std::vector<std::array<size_t, 3> > { { 0, 2, 4 } } ) );
  CSelfMatch<char> x5 ( "abababababab"s );
  assert ( x5 . sequenceLen ( 3 ) == 8 );
  assert ( positionMatch ( x5 . findSequences<3> (), std::vector<std::array<size_t, 3> > { { 0, 2, 4 } } ) );
  CSelfMatch<char> x6 ( "abcdXabcd"s );
  assert ( x6 . sequenceLen ( 1 ) == 9 );
  assert ( positionMatch ( x6 . findSequences<1> (), std::vector<std::array<size_t, 1> > { { 0 } } ) );
  CSelfMatch<char> x7 ( "abcdXabcd"s );
  assert ( x7 . sequenceLen ( 2 ) == 4 );
  assert ( positionMatch ( x7 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 5 } } ) );
  CSelfMatch<char> x8 ( "abcdXabcdeYabcdZabcd"s );
  assert ( x8 . sequenceLen ( 2 ) == 4 );
  assert ( positionMatch ( x8 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 5 }, { 0, 11 }, { 0, 16 }, { 5, 11 }, { 5, 16 }, { 11, 16 } } ) );
  CSelfMatch<char> x9 ( "abcdXabcdYabcd"s );
  assert ( x9 . sequenceLen ( 3 ) == 4 );
  assert ( positionMatch ( x9 . findSequences<3> (), std::vector<std::array<size_t, 3> > { { 0, 5, 10 } } ) );
  CSelfMatch<char> x10 ( "abcdefghijklmn"s );
  assert ( x10 . sequenceLen ( 2 ) == 0 );
  x10 . findSequences<2> ();
  assert ( positionMatch ( x10 . findSequences<2> (), std::vector<std::array<size_t, 2> > {  } ) );
  CSelfMatch<char> x11 ( "abcXabcYabcZdefXdef"s );
  assert ( x11 . sequenceLen ( 2 ) == 3 );
  assert ( positionMatch ( x11 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 4 }, { 0, 8 }, { 4, 8 }, { 12, 16 } } ) );
  CSelfMatch<int> x12 { 1, 2, 3, 1, 2, 4, 1, 2 };
  assert ( x12 . sequenceLen ( 2 ) == 2 );
  assert ( positionMatch ( x12 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 3 }, { 0, 6 }, { 3, 6 } } ) );
  assert ( x12 . sequenceLen ( 3 ) == 2 );
  assert ( positionMatch ( x12 . findSequences<3> (), std::vector<std::array<size_t, 3> > { { 0, 3, 6 } } ) );
  std::initializer_list<CDummy> init13 { 'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd' };
  CSelfMatch<CDummy> x13 ( init13 . begin (), init13 . end () );
  assert ( x13 . sequenceLen ( 2 ) == 4 );
  assert ( positionMatch ( x13 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 0, 5 }, { 0, 10 }, { 5, 10 } } ) );
  std::initializer_list<int> init14 { 1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2, 1, 0 };
  CSelfMatch<int> x14 ( init14 . begin (), init14 . end () );
  assert ( x14 . sequenceLen ( 2 ) == 5 );
  assert ( positionMatch ( x14 . findSequences<2> (), std::vector<std::array<size_t, 2> > { { 11, 14 }, { 7, 24 } } ) );
  std::initializer_list<int> init15 { 1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2, 1, 0 };
  CSelfMatch<int> x15 ( init15 . begin (), init15 . end () );
  assert ( x15 . sequenceLen ( 3 ) == 4 );
  assert ( positionMatch ( x15 . findSequences<3> (), std::vector<std::array<size_t, 3> > { { 3, 8, 25 } } ) );
  /*
  */
#ifdef TEST_EXTRA_INTERFACE
  CSelfMatch y0 ( "aaaaaaaaaaa"s );
  assert ( y0 . sequenceLen ( 2 ) == 10 );

  std::string s1 ( "abcd" );
  CSelfMatch y1 ( s1 . begin (), s1 . end () );
  assert ( y1 . sequenceLen ( 2 ) == 0 );

  CSelfMatch y2 ( ""s );
  y2 . push_back ( 'a', 'b', 'c', 'X' );
  y2 . push_back ( 'a' );
  y2 . push_back ( 'b', 'c' );
  assert ( y2 . sequenceLen ( 2 ) == 3 );
#endif /* TEST_EXTRA_INTERFACE */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
