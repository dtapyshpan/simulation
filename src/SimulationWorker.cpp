#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>

#include "../include/SimulationWorker.h"

const int SimulationWorker::dx[] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int SimulationWorker::dy[] = {-1,  0,  1, -1, 1, -1, 0, 1};
const int SimulationWorker::cntNeighbors = 8;
const double SimulationWorker::TMAX = 50.0;
const double SimulationWorker::dT = 0.01;

/*
  dt:        steps:
  0.00001 -- 300000
  0.0001  -- 30000
  0.001   -- 3000
  0.01    -- 300
*/

SimulationWorker::SimulationWorker( DrawWidgetCF *const drawW )
{
  row = column = -1;
  isActive = false;
  drawMWP = drawW;
  qRegisterMetaType< ModelData >();
  connect( this, SIGNAL(paintData(ModelData *)), drawMWP, SLOT(drawData(ModelData *)) );
}

SimulationWorker::~SimulationWorker()
{
  printf("SimulationWorker has been deleted\n");
}

void SimulationWorker::setSimulationEnabled( const bool arg )
{
  isActive = arg;
}

void SimulationWorker::setModelData( ModelData *data )
{
  curData = data;
  row = curData->getRow();
  column = curData->getColumn();
}

inline bool SimulationWorker::checkCoordinates( const int x, const int y )
{
  bool ret = true;
  if( x < 0 || y < 0 ) ret = false;
  if( x >= row || y >= column ) ret = false;
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
  for( int i = 0; i < row; ++i )
    for( int j = 0; j < column; ++j )
      if( curData->springCell( i, j ) > 0.0 ) 
	curData->rwaterCell( i, j ) = curData->springCell( i, j );
}

double SimulationWorker::transferWater( const double uH, const double uD, const double vH, const double vD )
{
  static const double A = 100.0;
  static const double B = 0.005;

  double d = uH - ( vH + vD );
  double D;

  if( d >= uD ) D = uD;
  else if( d == 0 ) D = uD / 2.0;
  else if( d > 0 && d < uD ) D = ( uD + d ) / 2.0;
  else D = (( uH + uD ) - ( vH + vD )) / 2.0;
  double p = 1.0 / ( 1.0 + A * exp( -1.0 * B * D ) );

  if( D * p < 0.0 )
  {
    printf("uH = %lf, uD = %lf, vH = %lf, vD = %lf\n", uH, uD, vH, vD);
    printf("D = %lf, p = %lf, d = %lf\n", D, p, d);
  }

  assert( D * p >= 0.0 );

  return D * p;
}

inline double SimulationWorker::calcMelting( const double arg )
{
  return log10( arg ) / log10( 1.5 );
}

void SimulationWorker::snowMelting( const double temperature, ShuffleIndexes &indexes )
{
  double f1 = calcMelting( temperature );
  for( int r = 0; r < row; ++r )
    for( int c = 0; c < column; ++c ) 
    {
      double v = curData->snowCell( r, c );
      if( v > 0.0 )
      {  
	double f2 = calcMelting( v );
	double urv = indexes.getUniformRandomVariable();
	double cnt = std::min( fabs( f1 * f2 * urv ), v );
	assert( cnt >= 0.0 );
	curData->rsnowCell( r, c ) -= cnt;
	assert( curData->snowCell( r, c ) >= 0.0 );
	curData->rwaterCell( r, c ) += cnt;
	curData->checkMaxDepth( r, c );
      }
    }
}

//random order of the cells
void SimulationWorker::moveWater( ShuffleIndexes *const indexes )
{
  int uk[cntNeighbors];
	
  for( int i = 0; i < cntNeighbors; ++i ) uk[i] = i;
  indexes->generatePermutation();
  for( int i = 0; i < row * column; ++i )
  {
    std::pair< int, int > cp = indexes->getCoordinates( i );
    int cx = cp.first, cy = cp.second;

    makePermutation( uk );
    for( int j = 0; j < cntNeighbors; ++j )
    {
      int nx = cx + dx[uk[j]];
      int ny = cy + dy[uk[j]];

      if( !checkCoordinates( nx, ny ) ) continue;

      double cH = curData->groundCell( cx, cy ), cW = curData->waterCell( cx, cy );
      double nH = curData->groundCell( nx, ny ), nW = curData->waterCell( nx, ny );

      if( nW == 0.0 ) continue;
      if( cH + cW >= nH + nW ) continue;
      //assert( cH >= 0.0 && cW >= 0.0 && nH >= 0.0 && nW >= 0.0 );
      double tw = transferWater( nH, nW, cH, cW );
      curData->rwaterCell( cx, cy ) += tw;
      curData->checkMaxDepth( cx, cy );
      curData->rwaterCell( nx, ny ) -= tw;
      assert( curData->waterCell( nx, ny ) >= 0.0 );
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
		
    double nH = curData->groundCell( nx, ny ), nW = curData->waterCell( nx, ny );
    if( nW == 0.0 ) continue;
    double tw = transferWater( nH, nW, nH, 0.0 );
    curData->rwaterCell( nx, ny ) -= tw;
    assert( curData->waterCell( nx, ny ) >= 0.0 );
  }
}

void SimulationWorker::collectWater()
{
  //top border; x = -1
  for( int i = 0; i < row; ++i ) relaxCells( -1, i );
  //right border; y = width;
  for( int i = 0; i < column; ++i ) relaxCells( i, column );
  //bottom border; x = height;
  for( int i = 0; i < row; ++i ) relaxCells( row, i );
  //left border; y = -1
  for( int i = 0; i < column; ++i ) relaxCells( i, -1 );
}

inline double SimulationWorker::getNoise()
{
  //noise -- [0.75, 1.25]
  double noise = double(rand()) / RAND_MAX;
  noise = fabs( 0.5 - noise );
  noise -= 0.25;
  noise += 1.0;
  return noise;
}

double SimulationWorker::getCoeff( double temperature )
{
  return cos( temperature ) * getNoise();
}

void SimulationWorker::startModeling()
{
  cz.lock();
  ShuffleIndexes indexes( row, column );
  cz.unlock();

  srand( time( NULL ) );

  double currentTemperature = TMAX;
  int t = 0;
  int f = 0;
  while( isActive )
  {
    /*
    currentTemperature += dT;
    double coeff1 = fabs( getCoeff( currentTemperature ) );
    double nextTemperature = TMAX * coeff1;
    double coeff2 = indexes.getUniformRandomVariable() * getNoise();

    printf("t = %d, coeff1 = %lf, coeff2 = %lf\n", ++t, coeff1, coeff2);
    */

    /*
    if( coeff1 < 0.0 )
    {
      //snow
      if( coeff2 >= fabs( coeff1 ) ) continue;
      double cnt = 2 * fabs( nextTemperature ) * getNoise();
      
      int x1 = rand() % row;
      int y1 = rand() % column;
      int l1 = rand() % ( row - x1 );
      int l2 = rand() % ( column - y1 );
      
      printf("cnt = %lf, x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", cnt, x1, y1, x1 + l1, y1 + l2);

      for( int r = x1; r < x1 + l1; ++r )
	for( int c = y1; c < y1 + l2; ++c )
	  curData->rsnowCell( r, c ) += cnt;

      f = 1;
    }
    else
    */
    { 
      //rain
      //snowMelting( nextTemperature, indexes );
      moveWater( &indexes );
      collectWater();
      generateWater();
    }
    
    cz.lock();
    emit paintData( curData );
    cz.unlock();
    
    //if( f ) break;
    //usleep( 10 * 1000 );
  }
  
  cz.lock();
  emit finished();
  cz.unlock();
}
