#include "LBP.hpp"

LBP::LBP( int points )
: LBPAbstract( points ){
    g_print("Using LBP...");
}

gint LBP::Run( guchar pixel, guchar * neighbours ) const{
    gint result = 0;
    for( int i = 0 ; i < m_Points; ++i ){
        // Shifts result to the left in every iteration 
        result <<= 1;
        if( neighbours[ i ] >= pixel ) ++result; 
    }
    return result;
}