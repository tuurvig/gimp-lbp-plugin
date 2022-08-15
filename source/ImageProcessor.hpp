#ifndef IMAGE_PROCESSOR
#define IMAGE_PROCESSOR

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include "InImageRegion.hpp"
#include "OutImageRegion.hpp"
#include "ProcessTile.hpp"


/**
 * Class ImageProcessor wrappes the whole process of LBP filter together by
 * defining input and output region with how big radius it should count with.   
 */
class ImageProcessor{
public:
    ImageProcessor( GimpDrawable * drawable, GimpPreview * preview, gint radius );
   ~ImageProcessor();
    
    /**
     * Takes in which method it should use and number of points in circle to process the image.
     * Image is processed by a row interval. Each row is loaded and applied filter on.
     */
    void Process( LBPMethod method, gint points );
private:
    GimpDrawable *m_Drawable;
    GimpPreview *m_Preview;
    gint m_Radius;
    
    InImageRegion * m_InRegion;
    OutImageRegion * m_OutRegion;
};

#endif