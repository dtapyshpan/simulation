#include <cstdio>
#include "../include/GraphData.h"

GraphData::GraphData( const int h, const int w )
{
  height = h;
  width = w;
  e = new std::vector < Edge > [h * w];
  v.resize( h * w );
}

GraphData::~GraphData()
{
  if( e ) delete [] e;
  e = NULL;
}

void GraphData::clear()
{
  for( int i = 0; i < height * width; ++i ) 
  {
    v[i].deg = v[i].col = 0;
    e[i].clear();
  }
}

void GraphData::addEdge( const int from, const int to, const int cole )
{
  if( cole == 0 ) ++v[to].deg;
  e[from].push_back( Edge(to, cole) );
}

void GraphData::bfs( const int vert )
{
  std::queue < int > q;
  int seen[height * width];

  memset( seen, 0, sizeof( seen ) );
	
  q.push( vert );
  seen[vert] = 1;
  while( !q.empty() )
  {
    int cv = q.front();
    q.pop();
    int n = int( e[cv].size() );
    for( int i = 0; i < n; ++i ) if( e[cv][i].col == 1 )
    {
      int nv = e[cv][i].v;
      if( seen[nv] ) continue;
      if( v[nv].col != 1 ) continue;
      seen[nv] = 1;
      q.push( nv );
      ++v[nv].col;
    }
  }
}

void GraphData::markNodes()
{
  for( int i = 0; i < height * width; ++i ) v[i].col = v[i].deg;
  for( int i = 0; i < height * width; ++i ) if( v[i].deg > 1 )
  {
    int n = int( e[i].size() );
    for( int j = 0; j < n; ++j ) if( e[i][j].col == 1 )
    {
      int nv = e[i][j].v;
      if( v[nv].col == 1 ) bfs( i );
    }
  }
}

int GraphData::colorVertex( const int id ) const
{
  return v[id].col;
}

int GraphData::degreeVertex( const int id ) const
{
  return v[id].deg;
}

void GraphData::writeAllRed()
{
  printf("red nodes:\n");
  for( int i = 0; i < height * width; ++i ) if( v[i].col == 1 )
  {
    printf("x = %d, y =  %d, col = %d\n", i / width, i % width, v[i].col);
  }
}
