#ifndef IMAGE_REGION
#define IMAGE_REGION

#include <libgimp/gimp.h>


/**
 * Stores basic information about a region in an image
 */
class ImageRegion{
public:
    ImageRegion( gint x, gint y, gint width, gint height );

    inline gint GetX() const { return m_X; }
    inline gint GetY() const { return m_Y; }
    inline gint Width() const { return m_Width; }
    inline gint Height() const { return m_Height; }
protected:
    gint m_X;
    gint m_Y;
    gint m_Width;
    gint m_Height;
};

#endif