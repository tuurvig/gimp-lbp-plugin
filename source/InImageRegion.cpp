#include "InImageRegion.hpp"

InImageRegion::InImageRegion( GimpDrawable * drawable, gint radius, gint x, gint y, gint width, gint height )
: ImageRegion( x, y, width, height ), m_Radius( radius )
, m_Channels( gimp_drawable_bpp( drawable->drawable_id ) )
, m_GimpRgn(), m_Buffer( nullptr ){
    m_Buffer = new guchar[ m_Width * m_Channels ];
    gimp_pixel_rgn_init( &m_GimpRgn, drawable, m_X, m_Y, m_Width, m_Height, false, false );
}

InImageRegion::~InImageRegion(){
    delete [] m_Buffer;
}

// out buffer needs to have size of image region width
void InImageRegion::LoadRow( int row, guchar * out ){
    gimp_pixel_rgn_get_row(&m_GimpRgn, m_Buffer, m_X, m_Y + row, m_Width);
    
    GetGreyscale( out );
}

void InImageRegion::GetGreyscale( guchar * out ) const{
    switch( m_Channels ){
        case 1: 
            for( int i = 0; i < m_Width; ++i ) 
                out[ i ] = m_Buffer[ i ];
            break;
        case 2:
            for( int i = 0; i < m_Width; ++i )
                out[ i ] = m_Buffer[ i * 2 ];
            break;
        case 3:
        case 4:
            int pos = 0;
            for( int i = 0; i < m_Width; ++i ){
                pos = i*m_Channels;
                out[ i ] = ( guchar )
                m_Buffer[ pos ] * R_GREY +
                m_Buffer[ pos + 1 ] * G_GREY +
                m_Buffer[ pos + 2 ] * B_GREY;
            }
        break;
    }
}