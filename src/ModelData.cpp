#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "../include/ModelData.h"

ModelData::ModelData() : height( -1 ), width( -1 )
{
	groundHeight = NULL;
	waterDepth = NULL;
}

ModelData::ModelData( const int argh, const int argw )
{
	height = argh;
	width = argw; 

	groundHeight = NULL;
	waterDepth = NULL;
	
	allocateMemory( groundHeight );
	allocateMemory( waterDepth );
	assert( groundHeight );
	assert( waterDepth );
}

ModelData::ModelData( const ModelData &arg ) : groundHeight( NULL ), waterDepth( NULL )
{
	height = arg.height;
	width = arg.width; 
	
	allocateMemory( groundHeight );
	allocateMemory( waterDepth );
	assert( groundHeight );
	assert( waterDepth );
	for( int i = 0; i < height; ++i )
		for( int j = 0; j < width; ++j )
		{
			groundHeight[i][j] = arg.groundHeight[i][j];
			waterDepth[i][j] = arg.waterDepth[i][j];
		}
	spring = arg.spring;

	printf("copying constructor has been invoked\n");
}

ModelData::~ModelData()
{
	destroyData();
	printf("model data has been deleted\n");
}

int ModelData::getWidth() const
{
	return width;
}

int ModelData::getHeight() const
{
	return height;
}

void ModelData::allocateMemory( int **(&matrix) )
{
	assert( width != -1 && height != -1 );

	matrix = new int * [height];
	for( int i = 0; i < height; ++i )
		matrix[i] = new int [width];
}

void ModelData::disposeMemory( int **(&matrix) )
{
	if( !matrix ) return;

	for( int i = 0; i < height; ++i )
		delete [] matrix[i];
	delete [] matrix;

	matrix = NULL;
}

void ModelData::destroyData()
{
	disposeMemory( groundHeight );
 	disposeMemory( waterDepth );
	assert( !groundHeight );
	assert( !waterDepth );
	width = height = -1;
	spring.clear();
	assert( !int( spring.size() ) );
}

void ModelData::readDataFromFile( const char *const fileName )
{
  FILE *f = fopen( fileName, "r" );

	if( !f ) throw( FileExx( fileName ) );
	
	destroyData();
	
	fscanf( f, "%d %d", &height, &width );

	allocateMemory( groundHeight );
	assert( groundHeight );
	for( int i = 0; i < height; ++i )
		for( int j = 0; j < width; ++j )
			fscanf( f, "%d", &groundHeight[i][j] );

	allocateMemory( waterDepth );
	assert( waterDepth );
	for( int i = 0; i < height; ++i )
		for( int j = 0; j < width; ++j )
			fscanf( f, "%d", &waterDepth[i][j] );

	assert( !int( spring.size() ) );
	for( int i = 0; i < height; ++i )
		for( int j = 0; j < width; ++j )
		{
			int x = 0;
			fscanf( f, "%d", &x );
			if( x )	spring.push_back( std::make_pair( std::make_pair( i, j ), waterDepth[i][j] ) );
		}
		
	fclose( f );
}

void ModelData::operator =( const ModelData &arg )
{
	destroyData();

	height = arg.height;
	width = arg.width;

	allocateMemory( groundHeight );
	allocateMemory( waterDepth );
	assert( groundHeight );
	assert( waterDepth );

	for( int i = 0; i < height; ++i )
		for( int j = 0; j < width; ++j )
		{
			groundHeight[i][j] = arg.groundHeight[i][j];
			waterDepth[i][j] = arg.waterDepth[i][j];
		}	
	spring = arg.spring;
}

double ModelData::getGroundColor( const int i, const int j ) const
{
	int cH = groundHeight[i][j];
	assert( 0 <= cH && cH <= MAXHEIGHT );
	return double( cH ) / MAXHEIGHT;
}

double ModelData::getWaterColor( const int i, const int j ) const
{
	int cD = waterDepth[i][j];
	if( cD > MAXDEPTH )
	{
		printf("i = %d, j = %d, cD = %d\n", i, j, cD);
		cD = MAXDEPTH;
		fflush( stdout );
	}
	//assert( 0 <= cD && cD <= MAXDEPTH );
	return double( cD ) / MAXDEPTH;
}

int ModelData::groundCell( const int i, const int j ) const
{
	return groundHeight[i][j];
}

int ModelData::waterCell( const int i, const int j ) const
{
	return waterDepth[i][j];
}

int& ModelData::waterCell( const int i, const int j )
{
	return waterDepth[i][j];
}

int ModelData::getSpringSize() const
{
	return int( spring.size() );
}

std::pair < std::pair < int, int >, int > ModelData::springCell( const int id )
{
	return spring[id];
}
