#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <vector>

class VertexData
{
public:

	VertexData() {}

private:
	
	/*
		0 -- w -- white vertex -- have to be minimum 2 neighbors
		1 -- r -- red vertex -- have to be minimum 1 neighbor
	 */
	int color;

	struct Edge
	{
		/*
			0 -- w -- white edge -- 
			1 -- y --
			2 -- g --
		 */
		int color;
		int neighbor;
		
   	Edge( const int a, const int b ) : color( a ), neighbor( b ) {}
	};
	
	std::vector < Edge > e;
};

#endif
