#ifndef SHUFFLEINDEXES_H
#define SHUFFLEINDEXES_H

#include <cstdlib>
#include <algorithm>

class ShuffleIndexes
{
public:

  ShuffleIndexes( const int, const int );
  ~ShuffleIndexes();
  
  void generatePermutation();
  std::pair< int, int > getCoordinates( const int );
  double getUniformRandomVariable();

private:

  static const int RANDMAX;

  int cnt, *permutation;
  std::pair< int, int > *indexes;
  double *uniformRandomVariable;

  void shuffleRandomVariables();
};

#endif
