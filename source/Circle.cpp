#include "Circle.hpp"
#include <cmath>
#include <libgimp/gimp.h>

Circle::Circle( int radius, int points )
: m_Vectors( nullptr ), m_Radius( radius ), m_Points( points ){
    m_Vectors = new Vector[ points ];

    CalculateDirections();
}

void Circle::CalculateDirections(){
    // radian angle between two points in a unit circle
    double angle = 2 * ( M_PI / ( float )m_Points );
    double pointAngle, x, y; 

    for( int i = 0; i < m_Points; ++i ){
        pointAngle = angle * i;

        // For evaluating the circle from its top in clockwise manner
        // Uses 90-degree rotated unit circle with negated cosine function assigned to y axis
        x =  sin( pointAngle ) * m_Radius;
        y = -cos( pointAngle ) * m_Radius;

        //g_print("( %d, %d )\n", (int)round( x ), (int)round( y ) ); //DEBUG
        
        // Pixel is located in a distance of x and y from center pixel
        m_Vectors[ i ] = { (int)round( x ), (int)round( y ) };
    }
}

Circle::~Circle(){
    delete [] m_Vectors;
}