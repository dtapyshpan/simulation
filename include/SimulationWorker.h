#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QtGui>

#include "ModelData.h"
#include "ShuffleIndexes.h"
#include "DrawWidgetCF.h"

Q_DECLARE_METATYPE( ModelData )

class SimulationWorker : public QObject
{
  Q_OBJECT

public:

  SimulationWorker( DrawWidgetCF *const );
  ~SimulationWorker();

  void setSimulationEnabled( const bool );
  void setModelData( ModelData * );

public slots:

  void startModeling();

signals:
	
  void finished();
  void paintData( ModelData * );

private:

  static const int dx[];
  static const int dy[];
  static const int cntNeighbors;
  static const double TMAX;
  static const double dT;

  ModelData *curData;
  int row, column;

  volatile bool isActive;
  DrawWidgetCF *drawMWP;

  QMutex cz;

  void generateWater();
  //transfer water from(G, W) -> to(G, W)
  static double transferWater( const double, const double, const double, const double );

  static inline double calcMelting( const double );
  void snowMelting( const double, ShuffleIndexes & );

  void moveWater( ShuffleIndexes *const );
  void relaxCells( const int, const int );
  void collectWater();
  static inline void makePermutation( int * );
  inline bool checkCoordinates( const int, const int );
  static inline double getNoise();
  double getCoeff( double );
};

#endif
