#ifndef MODELDATA_H
#define MODELDATA_H

#include <vector>

#include "Constants.h"
#include "FileExx.h"

class ModelData
{
public:

  ModelData();
  //first argument -- height; second -- width
  ModelData( const int, const int );
  ModelData( const ModelData & );
  ~ModelData();

  int getWidth() const;
  int getHeight() const;

  void destroyData();
  void readDataFromFile( const char *const );
  double getGroundColor( const int, const int ) const;
  double getWaterColor( const int, const int ) const;
  void operator =( const ModelData & );
  int groundCell( const int, const int ) const;
  int waterCell( const int, const int ) const;
  int& waterCell( const int, const int );
	
  int getSpringSize() const;
  std::pair < std::pair < int, int >, int > springCell( const int );

private:

  int height, width;
  int **groundHeight, **waterDepth;
  std::vector < std::pair < std::pair < int, int >, int > > spring;

  void allocateMemory( int **(&) );
  void disposeMemory( int **(&) );

  void rangeCheck( const int, const int ) const;
};

#endif
