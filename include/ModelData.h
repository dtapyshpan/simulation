#ifndef MODELDATA_H
#define MODELDATA_H

#include <vector>
#include "FileExx.h"

class ModelData
{
public:
  ModelData();
  ModelData( const int, const int );
  ModelData( const ModelData & );

  ~ModelData();

  int getRow() const;
  int getColumn() const;

  void destroyData();
  void readDataFromFile( const char *const );
  void saveToFile( const char *const );

  double getGroundColor( const int, const int ) const;
  double getWaterColor( const int, const int ) const;
  double getSnowColor( const int, const int ) const;

  void operator =( const ModelData & );

  double groundCell( const int, const int ) const;
  double waterCell( const int, const int ) const;
  double springCell( const int, const int ) const;
  double snowCell( const int, const int ) const;

  double& rgroundCell( const int, const int );
  double& rwaterCell( const int, const int );
  double& rspringCell( const int, const int );
  double& rsnowCell( const int, const int );

  void checkMaxDepth( const int, const int );
	
private:
  int row, column;
  double **groundHeight, **waterDepth, **spring, **snow;
  double MAXHEIGHT, MAXDEPTH;

  void initData( const int r = 0, const int c = 0 );
  void initData( const ModelData & );

  void allocateMemory( double **(&) );
  void disposeMemory( double **(&) );

  void copyData( const ModelData & );

  void rangeCheck( const int, const int ) const;
};

#endif
