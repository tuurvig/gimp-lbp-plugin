#ifndef OUT_IMAGE_REGION
#define OUT_IMAGE_REGION

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include "ImageRegion.hpp"

/**
 * Handles the output image region of a GIMP drawable. Is able to refresh this region.
 */
class OutImageRegion final : public ImageRegion{
public:
    OutImageRegion( GimpDrawable * drawable, gint x, gint y, gint width, gint height );
    void WriteRow( gint row, guchar * buffer );

    void DrawPreview( GimpPreview * preview );
    void UpdateDrawable( GimpDrawable * drawable );
private:
    GimpPixelRgn m_GimpRgn;
};

#endif