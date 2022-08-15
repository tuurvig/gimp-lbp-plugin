#include "ULBP.hpp"

ULBP::ULBP( int points )
: LBPAbstract( points ){
    g_print("Using ULBP...");
}

gint ULBP::Run( guchar pixel, guchar * neighbours ) const{
    gint result = 0;

    gint u = 0;
    bool zero = false;
    
    // Adds 1 to a smallest bit in result number if the neighbour is lighter or same as pixel
    for( int i = 0 ; i < m_Points; ++i ){
        result <<= 1;
        if( neighbours[ i ] >= pixel ){
            ++result;

            if( zero ){
                zero = false;
                ++u;
                if( u == 2 ) return 255 << 16;
            }
        }
        else{
            zero = true;
        }

    }

    return result;
}