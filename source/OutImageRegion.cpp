#include "OutImageRegion.hpp"

OutImageRegion::OutImageRegion( GimpDrawable * drawable, gint x, gint y, gint width, gint height )
: ImageRegion( x, y, width, height ), m_GimpRgn(){
    gimp_pixel_rgn_init( &m_GimpRgn, drawable, m_X, m_Y, m_Width, m_Height, true, true );
}

void OutImageRegion::WriteRow( gint row, guchar * buffer ){
    gimp_pixel_rgn_set_row( &m_GimpRgn, buffer, m_X, m_Y + row, m_Width );
}

void OutImageRegion::DrawPreview( GimpPreview * preview ){
    gimp_drawable_preview_draw_region( 
        GIMP_DRAWABLE_PREVIEW( preview ), &m_GimpRgn );
}

void OutImageRegion::UpdateDrawable( GimpDrawable * drawable ){
    gimp_drawable_flush( drawable );
    gimp_drawable_merge_shadow( drawable->drawable_id, true );
    gimp_drawable_update( drawable->drawable_id, m_X, m_Y, m_Width, m_Height );
}