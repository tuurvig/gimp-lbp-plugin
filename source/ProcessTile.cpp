#include "ProcessTile.hpp"
#include "LBP.hpp"
#include "ULBP.hpp"
#include "mLBP.hpp"
#include <memory>


ProcessTile::ProcessTile( gint radius, gint points, gint width, gint channels, LBPMethod method )
: m_Circle( radius, points ), m_Tile( nullptr ), m_OutBuffer( nullptr )
, m_PointsBuffer( nullptr ), m_Filter( nullptr ), m_Radius( radius )
, m_Width( width ), m_PaddedWidth( width + 2*radius )
, m_Height( 2*radius + 1 ), m_Channels( channels ){
    //g_print("\nProcessTile:\nWidth: %d, Height: %d, Radius: %d\n", m_PaddedWidth, m_Height, m_Radius );

    m_OutBuffer = new guchar[ m_Width * m_Channels ];
    m_PointsBuffer = new guchar[ points ];
    m_Tile = new guchar*[ m_Height ];
    
    for( int i = 0; i < m_Height; ++i ){
        m_Tile[ i ] = new guchar[ m_PaddedWidth ];
    }
    
    switch( method ){
        case e_LBP: m_Filter = new LBP( points ); break;
        case e_mLBP: m_Filter = new mLBP( points ); break;
        case e_ULBP: m_Filter = new ULBP( points ); break;
    }
}

ProcessTile::~ProcessTile(){
    for( int i = 0; i < m_Height; ++i )
        delete [] m_Tile[ i ];
    delete [] m_Tile;
    delete [] m_OutBuffer;
    delete [] m_PointsBuffer;
    delete m_Filter;
}

// shifts every row and places first row as the last one. 1 -> 0 | 2 -> 1 | 3 -> 2 | ....
void ProcessTile::RotateRows(){
    int upperBound = m_Height - 1;
    guchar * first = m_Tile[ 0 ];
    
    for( int i = 0; i < upperBound; ++i ){
        m_Tile[ i ] = m_Tile[ i + 1 ];
    }
    
    m_Tile[ upperBound ] = first;
}

void ProcessTile::CopyRow( int src, int dest ){
    memcpy( m_Tile[ dest ], m_Tile[ src ], m_PaddedWidth );
}

// The raw, actual data, where its mirroring from, starts at index radius and end at width + radius
void ProcessTile::MirrorRowBorders( int row ){
    guchar * rowPtr = m_Tile[ row ];
    for( int i = 0; i < m_Radius; ++i ){
        rowPtr[ m_Radius - i - 1 ] = rowPtr[ m_Radius + i ];
        rowPtr[ m_Width + m_Radius + i ] = rowPtr[ m_Width + m_Radius - i - 1 ];
    }
}

// Fills the entire points buffer with neighbours of pixel [x,y]
// Adds a vector to pixel coordinates to get coordinates of its neighbour 
void ProcessTile::PixelNeighbours( int x, int y ){
    for( int i = 0; i < m_Circle.GetPoints(); ++i ){
        const Vector & v = m_Circle[ i ];
        m_PointsBuffer[ i ] = m_Tile[ y + v.y ][ x + v.x ];
    }
}


guchar * ProcessTile::GetFiltered(){
    guchar pixel = 0;
    gint result = 0;
    gint pos = 0;
    for( int i = 0; i < m_Width; ++i ){
        // always processes the middle row. Needs to reach all the neighbours.
        pixel = m_Tile[ m_Radius ][ m_Radius + i ]; 
        PixelNeighbours( m_Radius + i, m_Radius );

        result = m_Filter->Run( pixel, m_PointsBuffer );
    
        pos = m_Channels * i;
        m_OutBuffer[ pos ] = result;
        m_OutBuffer[ pos + 1 ] = result >> 8;
        m_OutBuffer[ pos + 2 ] = result >> 16;
        
        if( m_Channels == 4 ) m_OutBuffer[ pos + 3 ] = 255;
    }

    return m_OutBuffer;
}