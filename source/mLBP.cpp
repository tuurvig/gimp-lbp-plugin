#include "mLBP.hpp"

mLBP::mLBP( int points )
: LBPAbstract( points ){
    g_print("Using mLBP...");
}

gint mLBP::CalculateMean( guchar pixel, guchar * neighbours ) const{
    gint count = pixel;
    for( int i = 0; i < m_Points; ++i ){
        count += neighbours[ i ];
    }

    return count / ( m_Points + 1 );
}

gint mLBP::Run( guchar pixel, guchar * neighbours ) const{
    gint result = 0;

    gint mean = CalculateMean( pixel, neighbours );
    for( int i = 0 ; i < m_Points; ++i ){
        result <<= 1;
        if( neighbours[ i ] >= mean ) ++result; 
    }
    
    return result;
}