#include "ImageProcessor.hpp"

ImageProcessor::ImageProcessor( GimpDrawable * drawable, GimpPreview * preview, gint radius )
: m_Drawable( drawable ), m_Preview( preview ), m_Radius( radius ), m_InRegion( nullptr ), m_OutRegion( nullptr ){
    int x1, y1, x2, y2;
    int width, height;

    gimp_drawable_mask_bounds( m_Drawable->drawable_id, &x1, &y1, &x2, &y2 );
    width = x2 - x1;
    height = y2 - y1;

    if( m_Preview ){
        gimp_preview_get_position( m_Preview, &x1, &y1 );
        gimp_preview_get_size( m_Preview, &width, &height );
    }

    //size info of calculated area
    g_print("\nProcessing image of these parameters\n");
    g_print("width: %d, height: %d\n", (int)width, (int)height);
    g_print("x1: %d, y1: %d\n", (int)x1, (int)y1);
    g_print("x2: %d, y2: %d\n", (int)x2, (int)y2);

    m_InRegion = new InImageRegion( m_Drawable, m_Radius, x1, y1, width, height );
    m_OutRegion = new OutImageRegion( m_Drawable, x1, y1, width, height );
}

ImageProcessor::~ImageProcessor(){
    delete m_InRegion;
    delete m_OutRegion;
}

void ImageProcessor::Process( LBPMethod method, gint points ){
    if( m_InRegion->Width() <= m_Radius || m_InRegion->Height() <= m_Radius ){
        g_message( "ERROR, this radius cannot be applied for this width and height" );
        return;
    }

    gimp_progress_init( "Calculating LBP..." );

    // ProcessTile stores few rows of greyscale pixels of the image and takes care of processing borders. 
    ProcessTile tile( m_Radius, points, m_InRegion->Width(), m_InRegion->Channels(), method ); 
    
    int row = 0;
    // Reads from in region those rows that are smaller than radius and mirrors them simultaneously.
    for( ; row < m_Radius; ++row ){
        m_InRegion->LoadRow( row, tile[ m_Radius + row ] + m_Radius );
        tile.MirrorRowBorders( m_Radius + row );
        tile.CopyRow( m_Radius + row, m_Radius - row - 1 );
    }

    // Reads one new row to the last row in tile, applies filter and shifts the first row as the last one.
    for( ; row < m_InRegion->Height(); ++row ){
        m_InRegion->LoadRow( row, tile[ tile.LastIndex() ] + m_Radius );
        tile.MirrorRowBorders( tile.LastIndex() );
        m_OutRegion->WriteRow( row - m_Radius, tile.GetFiltered() );
        tile.RotateRows();

        if( row % 50 == 0 ){
            gimp_progress_update( (gdouble)( row - m_Radius ) / ( gdouble )m_OutRegion->Height() );
        }
    }

    // Processes those rows that are closer to the lower image border than radius, by mirroring last rows.
    int upperBound = m_InRegion->Height() + m_Radius;
    int i = 0;
    for( ; row < upperBound; ++row ){
        tile.CopyRow( tile.LastIndex() - 2 * i - 1, tile.LastIndex() );
        m_OutRegion->WriteRow( row - m_Radius, tile.GetFiltered() );
        tile.RotateRows();
        ++i;

        if( row % 10 == 0 ){
            gimp_progress_update( (gdouble)( row - m_Radius ) / ( gdouble )m_OutRegion->Height() );
        }
    }
    
    if( m_Preview ) m_OutRegion->DrawPreview( m_Preview );
    else m_OutRegion->UpdateDrawable( m_Drawable );

    g_print( "Finished\n" );
    gimp_progress_update ( (gdouble) 1 );
}