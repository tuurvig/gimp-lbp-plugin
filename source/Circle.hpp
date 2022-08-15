#ifndef CIRCLE
#define CIRCLE

struct Vector{
    int x = 0;
    int y = 0;
};

/**
 * Circle with final number of points
 * Structure of vectors
 * Constructed vector is a direction to a pixel from [0,0]
 */
class Circle{
public:
    Circle( int radius, int points );
   ~Circle();

    inline const Vector & operator[]( int x ) const{ 
       return m_Vectors[ x ];
    }
    inline int GetPoints() const { return m_Points; }
private:
    /**
     * Constructs a vector for every point
     */
    void CalculateDirections();

    Vector * m_Vectors;
    int m_Radius;
    int m_Points;
};

#endif