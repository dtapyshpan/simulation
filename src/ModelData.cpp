#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "../include/ModelData.h"

ModelData::ModelData()
{
  printf("ModelData created1\n");
  initData();
}

ModelData::ModelData( const int r, const int c )
{
  printf("ModelData created2\n");
  initData( r, c );
	
  allocateMemory( groundHeight );
  allocateMemory( waterDepth );
  allocateMemory( spring );
  //allocateMemory( snow );

  assert( groundHeight );
  assert( waterDepth );
  assert( spring );
  //assert( snow );
}

ModelData::ModelData( const ModelData &arg )
{
  printf("ModelData created3\n");
  initData( arg );
  
  allocateMemory( groundHeight );
  allocateMemory( waterDepth );
  allocateMemory( spring );
  //allocateMemory( snow );

  assert( groundHeight );
  assert( waterDepth );
  assert( spring );
  //assert( snow );

  copyData( arg );
}

ModelData::~ModelData()
{
  destroyData();
  printf("model data has been deleted\n");
}

void ModelData::initData( const int r, const int c )
{
  row = r;
  column = c;
  MAXHEIGHT = -1.0;
  MAXDEPTH = -1.0;
  groundHeight = waterDepth = snow = spring = NULL;
}

void ModelData::initData( const ModelData &arg )
{
  row = arg.row;
  column = arg.column;
  MAXHEIGHT = arg.MAXHEIGHT;
  MAXDEPTH = arg.MAXDEPTH;
  groundHeight = waterDepth = /*snow =*/ spring = NULL;
}

void ModelData::copyData( const ModelData &arg )
{
  for( int r = 0; r < row; ++r )
    for( int c = 0; c < column; ++c )
    {
      groundHeight[r][c] = arg.groundHeight[r][c];
      waterDepth[r][c] = arg.waterDepth[r][c];
      spring[r][c] = arg.spring[r][c];
      //snow[r][c] = arg.snow[r][c];
    }
}

int ModelData::getRow() const
{
  return row;
}

int ModelData::getColumn() const
{
  return column;
}

void ModelData::allocateMemory( double **(&matrix) )
{
  assert( row != -1 && column != -1 );

  matrix = new double * [row];
  for( int r = 0; r < row; ++r )
    matrix[r] = new double [column];
}

void ModelData::disposeMemory( double **(&matrix) )
{
  if( !matrix ) return;

  for( int r = 0; r < row; ++r )
    delete[] matrix[r];
  delete[] matrix;

  matrix = NULL;
}

void ModelData::destroyData()
{
  disposeMemory( groundHeight );
  disposeMemory( waterDepth );
  disposeMemory( spring );
  //disposeMemory( snow );

  assert( !groundHeight );
  assert( !waterDepth );
  assert( !spring );
  //assert( !snow );

  row = column = -1;
  MAXHEIGHT = MAXDEPTH = -1.0;
}

void ModelData::readDataFromFile( const char *const fileName )
{
  FILE *f = fopen( fileName, "r" );

  if( !f ) throw( FileExx( fileName ) );
	
  destroyData();
	
  assert( fscanf( f, "%d%d", &row, &column ) == 2 );

  allocateMemory( groundHeight );
  assert( groundHeight );
  MAXHEIGHT = -1.0;
  for( int r = 0; r < row; ++r )
    for( int c = 0; c < column; ++c )
    {
      assert( fscanf( f, "%lf", &groundHeight[r][c] ) == 1 );
      assert( groundHeight[r][c] >= 0.0 );
      if( MAXHEIGHT < groundHeight[r][c] )
    	MAXHEIGHT = groundHeight[r][c] + 1.0;
    }

  allocateMemory( waterDepth );
  assert( waterDepth );
  MAXDEPTH = -1.0;
  for( int r = 0; r < row; ++r )
    for( int c = 0; c < column; ++c )
    {
      assert( fscanf( f, "%lf", &waterDepth[r][c] ) == 1 );
      assert( waterDepth[r][c] >= 0.0 );
      if( MAXDEPTH < waterDepth[r][c] )
	    MAXDEPTH = waterDepth[r][c] + 1.0;
    }

  allocateMemory( spring );
  assert( spring );
  for( int r = 0; r < row; ++r )
    for( int c = 0; c < column; ++c )
    {
      assert( fscanf( f, "%lf", &spring[r][c] ) == 1 );
      assert( spring[r][c] >= 0.0 );
      if( MAXDEPTH < spring[r][c] )
	    MAXDEPTH = spring[r][c] + 1.0;
    }
  //MAXDEPTH *= 8.0;
  /*
  allocateMemory( snow );
  assert( snow );
  for( int r = 0; r < row; ++r )
    for( int c = 0; c < column; ++c )
    {
      assert( fscanf( f, "%lf", &snow[r][c] ) == 1 );
      assert( snow[r][c] >= 0.0 );
    }
  */ 
  fclose( f );
}

void ModelData::saveToFile( const char *const fileName )
{
  FILE *f = fopen( fileName, "w" );

  assert( f );
  assert( fprintf( f, "%d %d\n", row, column ) >= 0 );
  //ground
  for( int i = 0; i < row; ++i )
  {
    for( int j = 0; j < column; ++j )
      assert( fprintf( f, "%.20lf ", groundHeight[i][j] ) >= 0 );
    assert( fprintf( f, "\n") >= 0 );
  }
  //water
  for( int i = 0; i < row; ++i )
  {
    for( int j = 0; j < column; ++j )
      assert( fprintf( f, "%.20lf ", waterDepth[i][j] ) >= 0 );
    assert( fprintf( f, "\n") >= 0 );
  }
  //spring
  for( int i = 0; i < row; ++i )
  {
    for( int j = 0; j < column; ++j )
      assert( fprintf( f, "%.20lf ", spring[i][j] ) >= 0 );
    assert( fprintf( f, "\n") >= 0 );
  }
  /*
  //snow
  for( int i = 0; i < row; ++i )
  {
    for( int j = 0; j < column; ++j )
      assert( fprintf( f, "%.20lf ", snow[i][j] ) >= 0 );
    assert( fprintf( f, "\n") >= 0 );
  }
  */
  fclose( f );
}

void ModelData::operator =( const ModelData &arg )
{
  destroyData();
  initData( arg );

  allocateMemory( groundHeight );
  allocateMemory( waterDepth );
  allocateMemory( spring );
  //allocateMemory( snow );

  assert( groundHeight );
  assert( waterDepth );
  assert( spring );
  //assert( snow );

  copyData( arg );
}

double ModelData::getGroundColor( const int r, const int c ) const
{
  rangeCheck( r, c );
  double cH = groundHeight[r][c];
  if( cH > MAXHEIGHT ) cH = MAXHEIGHT;
  assert( 0 <= cH && cH <= MAXHEIGHT );
  return cH / MAXHEIGHT;
}

double ModelData::getWaterColor( const int r, const int c ) const
{
  rangeCheck( r, c );
  double cD = waterDepth[r][c];
  /*
  if( cD > MAXDEPTH )
  {
    printf("r = %d, c = %d, cD = %lf, MAXDEPTH = %lf\n", r, c, cD, MAXDEPTH);
    cD = MAXDEPTH;
    fflush( stdout );
  }
  */
  assert( 0 <= cD && cD <= MAXDEPTH );
  return cD / MAXDEPTH;
}

double ModelData::getSnowColor( const int r, const int c ) const
{
  rangeCheck( r, c );
  double cS = snow[r][c];
  return 0.5;
}

double ModelData::groundCell( const int r, const int c ) const
{
  rangeCheck( r, c );
  return groundHeight[r][c];
}

double ModelData::waterCell( const int r, const int c ) const
{
  rangeCheck( r, c );
  return waterDepth[r][c];
}

double ModelData::springCell( const int r, const int c ) const
{
  rangeCheck( r, c );
  return spring[r][c];
}

double ModelData::snowCell( const int r, const int c ) const
{
  rangeCheck( r, c );
  return snow[r][c];
}

double& ModelData::rgroundCell( const int r, const int c )
{
  rangeCheck( r, c );
  return groundHeight[r][c];
}

double& ModelData::rwaterCell( const int r, const int c )
{
  rangeCheck( r, c );
  return waterDepth[r][c];
}

double& ModelData::rspringCell( const int r, const int c )
{
  rangeCheck( r, c );
  return spring[r][c];
}

double& ModelData::rsnowCell( const int r, const int c )
{
  rangeCheck( r, c );
  return snow[r][c];
}

void ModelData::checkMaxDepth( const int r, const int c )
{
  if( waterDepth[r][c] > MAXDEPTH )
    MAXDEPTH = waterDepth[r][c];
}

void ModelData::rangeCheck( const int r, const int c ) const
{
  assert( r >= 0 && c >= 0 );
  assert( r < row && c < column );
}
