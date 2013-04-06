#ifndef SHUFFLEINDEXES_H
#define SHUFFLEINDEXES_H

#include <algorithm>

class ShuffleIndexes
{
public:

	ShuffleIndexes( const int, const int );
	~ShuffleIndexes();
	
	void generatePermutation();
	std::pair < int, int > getCoordinates( const int );

private:

	int cnt;
	std::pair < int, int > *indexes;
	int *permutation;
};

#endif
