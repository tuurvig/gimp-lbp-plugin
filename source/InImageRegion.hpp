#ifndef IN_IMAGE_REGION
#define IN_IMAGE_REGION

#include <libgimp/gimp.h>
#include "ImageRegion.hpp"

/**
 * Defines input region of a GIMP drawable, and its loading operation.
 */
class InImageRegion final : public ImageRegion{
public:
    InImageRegion( GimpDrawable * drawable, gint radius, gint x, gint y, gint width, gint height );
   ~InImageRegion();
    
    /**
     * Loads one row m_Buffer and grayscales the row into *out buffer, 
     * which must have size of this image region's width.
     */ 
    void LoadRow( int row, guchar * out );

    inline gint Channels() const { return m_Channels; }
private:
    const float R_GREY = 0.2126f;
    const float G_GREY = 0.7152f;
    const float B_GREY = 0.0722f;

    /**
     * Transforms the m_Buffer with number of channels into grayscaled
     * pixels that are loaded into out buffer input parameter.
     */
    void GetGreyscale( guchar * out ) const;

    gint m_Radius;
    gint m_Channels;
    GimpPixelRgn m_GimpRgn;
    guchar * m_Buffer;
};

#endif