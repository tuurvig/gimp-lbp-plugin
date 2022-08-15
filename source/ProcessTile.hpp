#ifndef PROCESS_TILE
#define PROCESS_TILE

#include <libgimp/gimp.h>
#include "Circle.hpp"
#include "LBPAbstract.hpp"

enum LBPMethod{ e_LBP, e_mLBP, e_ULBP };

/**
 * Structure that holds only that many pixels that are absolutely necessary to process every pixel in a row.
 * More precisely:
 *      height: 2*radius + 1
 *      width: 2*radius + inputWidth
 * Every pixel needs a space where it can search for neighbouring pixels delimited by a circle
 */
class ProcessTile{
public:
    ProcessTile( gint radius, gint points, gint width, gint channels, LBPMethod method );
   ~ProcessTile(); 

    /**
     * Filters every pixel in the middle row and outputs the results into m_Buffer that is also the return value.
     */
    guchar * GetFiltered();

    /**
     * Shifts every row by one row up and the first replaces the last row.
     */
    void RotateRows();

    /**
     * Whole row of source index is copied into row of destination index
     */
    void CopyRow( int src, int dest );

    /**
     * Mirrors every pixel in the beginning and ending length of a radius on both sides of the row
     */
    void MirrorRowBorders( int row );

    inline gint Width() const { return m_PaddedWidth; }
    inline gint LastIndex() const { return m_Height - 1; }
    
    inline guchar * operator[]( int row ) const{ 
       return m_Tile[ row ];
    }
private:

    /**
     * Fills every neighbour of pixel [x,y] into points buffer
     */
    void PixelNeighbours( int x, int y );

    Circle m_Circle;
    guchar ** m_Tile;
    guchar * m_OutBuffer;
    guchar * m_PointsBuffer;
    LBPAbstract * m_Filter;

    gint m_Radius;
    gint m_Width;
    gint m_PaddedWidth;
    gint m_Height;
    gint m_Channels;
};

#endif