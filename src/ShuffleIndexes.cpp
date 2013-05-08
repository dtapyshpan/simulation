#include <cstdio>
#include <cassert>

#include "../include/ShuffleIndexes.h"

ShuffleIndexes::ShuffleIndexes( const int h, const int w )
{
	cnt = h * w;
	indexes = new std::pair < int, int > [cnt];
	permutation = new int [cnt];
	for( int i = 0; i < h; ++i )
		for( int j = 0; j < w; ++j ) indexes[i * w + j] = std::make_pair( i, j );
	for( int i = 0; i < cnt; ++i ) permutation[i] = i;
}

ShuffleIndexes::~ShuffleIndexes()
{
	delete [] indexes;
	delete [] permutation;
	indexes = NULL;
	permutation = NULL;
	printf("shuffle indexes has been deleted\n");
}

void ShuffleIndexes::generatePermutation()
{
	for( int i = 1; i < cnt; ++i )
	{
		int x = rand() % (i + 1);
		std::swap( permutation[i], permutation[x] );
	}
}

std::pair < int, int > ShuffleIndexes::getCoordinates( const int id )
{
	assert( id >= 0 && id < cnt );
	return indexes[permutation[id]];
}
