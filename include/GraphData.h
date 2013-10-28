#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <vector>
#include <queue>
#include <cstring>

class GraphData
{
public:

  GraphData( const int, const int );
  ~GraphData();
  
  void clear();
  void addEdge( const int, const int, const int );
  void markNodes();
  void writeAllRed();
  int colorVertex( const int ) const;
  int degreeVertex( const int ) const;

private:

  struct Vertex
  {
    //deg -- degree of the vertex, i.e. amount of the incomming edges
    //deg: 0 -- unreachable vertex; 1 -- one neighbor; >1 -- a lot of neighbors
    //col: 0 -- unreachable vertex; 1 -- red vertex; >1 -- white vertex
    int deg, col;
    Vertex() : deg(0), col(0) {}
  };

  struct Edge
  {
    //col: 0 -- w; 1 -- y;
    int v, col;
    Edge( const int a, const int b ) : v(a), col(b) {}
  };

  int height, width;
  std::vector< Vertex > v;
  std::vector< Edge > *e;

  void bfs( const int );
};

#endif
