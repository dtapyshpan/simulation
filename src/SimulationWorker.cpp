#include <cstdio>
#include <cstdlib>
#include <cmath>
//#include <ctime>
#include <cassert>
#include <set>
#include <vector>
#include <queue>

#include <unistd.h>

#include "../include/SimulationWorker.h"

const int SimulationWorker::dx[] = { -1, -1, -1, 0, 1, 0 };
const int SimulationWorker::dy[] = { -1, 0, 1, 1, 0, -1 };
const int SimulationWorker::cntNeighbors = 6;

SimulationWorker::SimulationWorker( DrawWidget *const drawW )
{
	isActive = 0;
	drawMWP = drawW;
	qRegisterMetaType< ModelData >();
	connect( this, SIGNAL(paintData(const ModelData &)), drawMWP, SLOT(drawData(const ModelData &)) );
}

SimulationWorker::~SimulationWorker()
{
	printf("SimulationWorker has been deleted\n");
}

void SimulationWorker::setSimulationEnabled( const int arg )
{
	isActive = arg;
}

void SimulationWorker::setModelData( const ModelData &data )
{
	curData = data;
	height = curData.getHeight();
	width = curData.getWidth();
}

inline int SimulationWorker::checkCoordinates( const int x, const int y )
{
	int ret = 1;
	if( x < 0 || y < 0 ) ret = 0;
	if( x >= height || y >= width ) ret = 0;
	return ret;
}

inline void SimulationWorker::makePermutation( int *uk )
{
	for( int i = 0; i < cntNeighbors; ++i )
	{
		int x = rand() % (i + 1);
		std::swap( uk[i], uk[x] );
	}
}

void SimulationWorker::generateWater()
{
	int n = curData.getSpringSize();
	for( int i = 0; i < n; ++i )
	{
		std::pair < std::pair < int, int >, int > cspring = curData.springCell( i );
		int x = cspring.first.first;
		int y = cspring.first.second;
		int w = cspring.second;
		curData.waterCell( x, y ) = w;
	}
}

void SimulationWorker::makeGraph( GraphData *graph )
{
	for( int i = 0; i < height; ++i ) for( int j = 0; j < width; ++j )
	{
		int cx = i, cy = j;
		int cH = curData.groundCell( cx, cy ), cW = curData.waterCell( cx, cy );
		int cv = cx * width + cy;

		for( int k = 0; k < cntNeighbors; ++k )
		{
			int nx = cx + dx[k], ny = cy + dy[k];
			int nv = nx * width + ny;

			if( !checkCoordinates( nx, ny ) ) continue;

			int nH = curData.groundCell( nx, ny );
			int nW = curData.waterCell( nx, ny );

			if( cH + cW > nH + nW ) continue;

			int cole = cH + cW == nH + nW;
			graph->addEdge( nv, cv, cole );
		}
	}

	graph->markNodes();
	//graph->writeAllRed();
}

int SimulationWorker::transferWater( const int fromG, const int fromW, const int toG, const int toW )
{
  static const double A = 100.0;
	static const double B = 0.05;

	double d;

	if( toG + toW <= fromG ) d = fromW / 2.0;
	else d = ( ( fromG + fromW ) - ( toG + toW ) ) / 2.0;
	
	double p = 1.0 / ( 1.0 + A * exp( -1.0 * B * d ) );

	return int( ceil( d * p ) );
}

/***
random order of the cells
 ***/
void SimulationWorker::moveWater( ShuffleIndexes *const indexes, const GraphData &graph )
{
	int uk[cntNeighbors];
	
	for( int i = 0; i < cntNeighbors; ++i ) uk[i] = i;
	indexes->generatePermutation();
	for( int i = 0; i < height * width; ++i )
	{
		std::pair < int, int > cp = indexes->getCoordinates( i );
		int cx = cp.first, cy = cp.second;
		int cH = curData.groundCell( cx, cy ), cW = curData.waterCell( cx, cy );

		int cvcol = graph.colorVertex( cx * width + cy );
		int cvdeg = graph.degreeVertex( cx * width + cy );
		
		//if( cvcol == 0 ) continue; //isolate vertex
	  //if( cvdeg == 1 && cvcol > 1 ) continue; //bank >2

		makePermutation( uk );
		for( int j = 0; j < cntNeighbors; ++j )
		{
			int nx = cx + dx[uk[j]];
			int ny = cy + dy[uk[j]];

			if( !checkCoordinates( nx, ny ) ) continue;

			int nH = curData.groundCell( nx, ny ), nW = curData.waterCell( nx, ny );

			if( nW == 0 ) continue;
			if( cH + cW >= nH + nW ) continue;
			
			int tw = transferWater( nH, nW, cH, cW );
			curData.waterCell( cx, cy ) += tw;
			curData.waterCell( nx, ny ) -= tw;
		}
	}
}

void SimulationWorker::relaxCells( const int cx, const int cy )
{
	for( int j = 0; j < cntNeighbors; ++j )
	{
		int nx = cx + dx[j];
		int ny = cy + dy[j];

		if( !checkCoordinates( nx, ny ) ) continue;
		
		int nH = curData.groundCell( nx, ny ), nW = curData.waterCell( nx, ny );
		if( nW == 0 ) continue;
		int tw = transferWater( nH, nW, nH, 0 );
		curData.waterCell( nx, ny ) -= tw;
	}
}

void SimulationWorker::collectWater()
{
	//top border; x = -1
	for( int i = 0; i < width; ++i ) relaxCells( -1, i );
	//right border; y = width;
	for( int i = 0; i < height; ++i ) relaxCells( i, width );
	//bottom border; x = height;
	for( int i = 0; i < width; ++i ) relaxCells( height, i );
	//left border; y = -1
	for( int i = 0; i < height; ++i ) relaxCells( i, -1 );
}

void SimulationWorker::startModelling()
{
	cz.lock();
	ShuffleIndexes indexes( curData.getHeight(), curData.getWidth() );
	cz.unlock();

	freopen("test1.out", "wt", stdout);
	
	int s1 = 0;
	printf("befor modelling\n");
	for( int i = 0; i < curData.getHeight(); ++i )
	{
		for( int j = 0; j < curData.getWidth(); ++j )
		{
			int x = curData.waterCell( i, j );
			s1 += x;
			printf("%d ", x);
		}
		printf("\n");
	}

	srand( time( NULL ) );
	GraphData graph( curData.getHeight(), curData.getWidth() );
	while( isActive )
	{
		//makeGraph( &graph );
		moveWater( &indexes, graph );
		collectWater();
		generateWater();

		cz.lock();
		emit paintData( curData );
		cz.unlock();

		usleep( 10 * 1000 );
		graph.clear();
	}

	int s2 = 0;
	printf("\nafter modelling\n");
	for( int i = 0; i < curData.getHeight(); ++i )
	{
		for( int j = 0; j < curData.getWidth(); ++j )
		{
			int x = curData.waterCell( i, j );
			s2 += x;
			printf("%d ", x);
		}
		printf("\n");
	}
	printf("\n\n s1 = %d, s2 = %d\n", s1, s2);

	fclose( stdout );

	cz.lock();
	emit finished();
	cz.unlock();
}
