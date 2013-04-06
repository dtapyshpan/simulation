#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QtGui>

#include "ModelData.h"
#include "ShuffleIndexes.h"
#include "DrawWidget.h"
#include "GraphData.h"

Q_DECLARE_METATYPE( ModelData )

class SimulationWorker : public QObject
{
	Q_OBJECT

public:

	SimulationWorker( DrawWidget *const );
	~SimulationWorker();

	void setSimulationEnabled( const int );
	void setModelData( const ModelData & );

public slots:

	void startModelling();

signals:
	
	void finished();
	void paintData( const ModelData & );

private:

	static const int dx[];
	static const int dy[];
	static const int cntNeighbors;

	volatile int isActive;
	ModelData curData;
	DrawWidget *drawMWP;

	QMutex cz;

	int height, width;

	void generateWater();
	void makeGraph(	GraphData * );
	//transfer water from(G, W) -> to(G, W)
	static int transferWater( const int, const int, const int, const int );
	void moveWater( ShuffleIndexes *const, const GraphData & );
	void relaxCells( const int, const int );
	void collectWater();
	static inline void makePermutation( int * );

	inline int checkCoordinates( const int, const int );
};

#endif
