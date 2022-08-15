//----------------------------------------------------------------------------------------
/**
 * \file       LBPKvasnica.cpp
 * \author     Richard Kvasnica
 * \date       2020/11/01
 * \brief      Main file of plugin. Contains necessary functions for plugin to work.
*/
//----------------------------------------------------------------------------------------


#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include "ImageProcessor.hpp"
#include <string>

const std::string PLUGIN_NAME = "plug-in-lbp-kvasnica";
const std::string MODIFY_PARAM_LABEL = "<b>Modify parameters</b>";
const std::string RADIUS_LABEL = "_Radius:";
const std::string POINTS_LABEL = "_Points:";
const std::string LBP_METHOD_LABEL = "<b>Pick LBP method</b>";
const std::string LBP_CHOICE = "LBP";
const std::string ULBP_CHOICE = "ULBP";
const std::string mLBP_CHOICE = "mLBP";

const gint MIN_RADIUS = 1;
const gint MIN_POINTS = 4;
const gint MAX_RADIUS = 190;
const gint MAX_POINTS = 24;
const gint DEFAULT_POINTS = 8;
const gint DEFAULT_RADIUS = 1;


// mandatory function declaration
static void query (void);
static void run   (const gchar      *name,
                   gint              nparams,
                   const GimpParam  *param,
                   gint             *nreturn_vals,
                   GimpParam       **return_vals);


GimpPlugInInfo PLUG_IN_INFO{ nullptr, nullptr, query, run };

void ProcessImage( GimpDrawable * drawable, GimpPreview * preview );
gboolean LBPDialog (GimpDrawable *drawable);
void MethodChanged( GtkWidget * combo );

struct UIInfo{
  gint radius;
  gint points;
  LBPMethod method;
  gboolean preview;
} GLOBAL_UI_INFO;


MAIN()


static void query (void){
    static GimpParamDef args[] = {
        {
            GIMP_PDB_INT32,
            ( gchar * )"run-mode",
            ( gchar * )"Run mode"
        },
        {
            GIMP_PDB_IMAGE,
            ( gchar * )"image",
            ( gchar * )"Input image"
        },
        {
            GIMP_PDB_DRAWABLE,
            ( gchar * )"drawable",
            ( gchar * )"Input drawable"
        }
    };

    gimp_install_procedure(
        PLUGIN_NAME.c_str(),
        "Local Binary Patterns",
        "Computes Local Binary Pattern",
        "Richard Kvasnica",
        "Copyright Richard Kvasnica",
        "2020",
        "_plug-in-lbp-kvasnica",
        "RGB, RGBA",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0, args, NULL
    );

    gimp_plugin_menu_register(PLUGIN_NAME.c_str(), "<Image>/Filters/Misc");    
}


static void run(
    const gchar     *name,
    gint             nparams,
    const GimpParam *param,
    gint            *nreturn_vals,
    GimpParam      **return_vals
){
    static GimpParam   values[1];
    GimpPDBStatusType  status = GIMP_PDB_SUCCESS;
    GimpRunMode        run_mode;
    GimpDrawable      *drawable;

    *nreturn_vals = 1;
    *return_vals = values;
    values[0].type = GIMP_PDB_STATUS;
    values[0].data.d_status = status;

    run_mode = ( GimpRunMode )param[0].data.d_int32;

    drawable = gimp_drawable_get( param[2].data.d_drawable );

    GLOBAL_UI_INFO.points = DEFAULT_POINTS;
    GLOBAL_UI_INFO.radius = DEFAULT_RADIUS;
    GLOBAL_UI_INFO.preview = true;
    
    switch (run_mode)
    {
    case GIMP_RUN_INTERACTIVE:
      /* Get options last values if needed */
      //gimp_get_data (PLUGIN_NAME.c_str(), &GLOBAL_UI_INFO);

      /* Display the dialog */
      if ( !LBPDialog (drawable) )
        return;
      break;

    // case GIMP_RUN_NONINTERACTIVE:
    //   if (nparams != 5)
    //     status = GIMP_PDB_CALLING_ERROR;
    //   if (status == GIMP_PDB_SUCCESS)
    //     GLOBAL_UI_INFO.radius = param[3].data.d_int32;
    //     GLOBAL_UI_INFO.points = param[4].data.d_int32;
    //   break;

    case GIMP_RUN_WITH_LAST_VALS:
      /*  Get options last values if needed  */
      gimp_get_data (PLUGIN_NAME.c_str(), &GLOBAL_UI_INFO);
      break;

    default:
      break;
    }

  ProcessImage( drawable, nullptr );

  gimp_displays_flush ();
  gimp_drawable_detach (drawable);

  /*  Finally, set options in the core  */
  if (run_mode == GIMP_RUN_INTERACTIVE)
    gimp_set_data (PLUGIN_NAME.c_str(), &GLOBAL_UI_INFO, sizeof (UIInfo));

  return;
}

void methodChanged( GtkWidget * combo ){
  int activeID = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
  switch( activeID ){
    case 0: GLOBAL_UI_INFO.method = e_LBP; break;
    case 1: GLOBAL_UI_INFO.method = e_mLBP; break;
    case 2: GLOBAL_UI_INFO.method = e_ULBP; break;
  }
}

void ProcessImage( GimpDrawable * drawable, GimpPreview * preview ){
  g_print( "\nSelected method: %d\n", GLOBAL_UI_INFO.method );
  ImageProcessor ip( drawable, preview, GLOBAL_UI_INFO.radius );
  ip.Process( GLOBAL_UI_INFO.method, GLOBAL_UI_INFO.points );
}

gboolean LBPDialog (GimpDrawable *drawable)
{
  GtkWidget *dialog;
  GtkWidget *mainVBox;
  GtkWidget *mainHBox;
  GtkWidget *frame;
  GtkWidget *alignment;
  GtkWidget *frameLabel;

  GtkWidget *radiusLabel;
  GtkWidget *radiusSpinButton;
  GtkObject *radiusSpinButtonAdj;
  
  GtkWidget *pointsLabel;
  GtkWidget *pointsSpinButton;
  GtkObject *pointsSpinButtonAdj;
  
  GtkWidget *methodFrame;
  GtkWidget *methodAlignment;
  GtkWidget *methodLabel;
  GtkWidget *methodCombo;
  GtkWidget *methodChoiceBox;

  GtkWidget * preview;

  gboolean   run;

  gimp_ui_init ("kvasnicaLBP", FALSE);

  dialog = gimp_dialog_new ("LBP", "kvasnicaLBP",
                            NULL, ( GtkDialogFlags )0,
                            gimp_standard_help_func, PLUGIN_NAME.c_str(),
                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OK,     GTK_RESPONSE_OK,
                            NULL);
                            
  // main vertical box                          
  mainVBox = gtk_vbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), mainVBox);
  gtk_widget_show (mainVBox);

  // parameter selection frame
  frame = gtk_frame_new (NULL);
  gtk_widget_show (frame);
  gtk_box_pack_start (GTK_BOX (mainVBox), frame, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame), 6);

  // alignment for the frame
  alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment);
  gtk_container_add (GTK_CONTAINER (frame), alignment);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 6, 6, 6, 6);

  // nain horizontal box
  mainHBox = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (mainHBox);
  gtk_container_add (GTK_CONTAINER (alignment), mainHBox);

  radiusLabel = gtk_label_new_with_mnemonic ( RADIUS_LABEL.c_str() );
  gtk_widget_show ( radiusLabel );
  gtk_box_pack_start (GTK_BOX (mainHBox), radiusLabel, FALSE, FALSE, 6);
  gtk_label_set_justify (GTK_LABEL (radiusLabel), GTK_JUSTIFY_RIGHT);

  // spinbutton for radius input
  radiusSpinButtonAdj = gtk_adjustment_new (DEFAULT_RADIUS, MIN_RADIUS, MAX_RADIUS, 1, 5, 5);
  radiusSpinButton = gtk_spin_button_new (GTK_ADJUSTMENT (radiusSpinButtonAdj), 1, 0);
  gtk_box_pack_start (GTK_BOX (mainHBox), radiusSpinButton, FALSE, FALSE, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (radiusSpinButton), TRUE);
  gtk_widget_show (radiusSpinButton);

  pointsLabel = gtk_label_new_with_mnemonic ( POINTS_LABEL.c_str() );
  gtk_widget_show ( pointsLabel );
  gtk_box_pack_start (GTK_BOX (mainHBox), pointsLabel, FALSE, FALSE, 6);
  gtk_label_set_justify (GTK_LABEL (pointsLabel), GTK_JUSTIFY_LEFT);

  // spinbutton for points input
  pointsSpinButtonAdj = gtk_adjustment_new (DEFAULT_POINTS, MIN_POINTS, MAX_POINTS, 1, 5, 5);
  pointsSpinButton = gtk_spin_button_new (GTK_ADJUSTMENT (pointsSpinButtonAdj), 1, 0);
  gtk_widget_show (pointsSpinButton);
  gtk_box_pack_start (GTK_BOX (mainHBox), pointsSpinButton, FALSE, FALSE, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (pointsSpinButton), TRUE);

  frameLabel = gtk_label_new ( MODIFY_PARAM_LABEL.c_str() );
  gtk_widget_show (frameLabel);
  gtk_frame_set_label_widget (GTK_FRAME (frame), frameLabel);
  gtk_label_set_use_markup (GTK_LABEL (frameLabel), TRUE);

  methodFrame = gtk_frame_new(NULL);
  gtk_widget_show( methodFrame );
  gtk_box_pack_start( GTK_BOX( mainVBox ), methodFrame, true, true, 0 );
  gtk_container_set_border_width( GTK_CONTAINER(methodFrame), 6 );

  methodAlignment = gtk_alignment_new( 0.5, 0.5, 1, 1 );
  gtk_widget_show( methodAlignment );
  gtk_container_add( GTK_CONTAINER( methodFrame ), methodAlignment );
  gtk_alignment_set_padding( GTK_ALIGNMENT( methodAlignment ), 6, 6, 6, 6 );

  methodLabel = gtk_label_new( LBP_METHOD_LABEL.c_str() );
  gtk_widget_show( methodFrame );
  gtk_frame_set_label_widget( GTK_FRAME( methodFrame ), methodLabel );
  gtk_label_set_use_markup( GTK_LABEL( methodLabel ), true );

  methodChoiceBox = gtk_hbox_new( false, 0 );
  gtk_widget_show( methodChoiceBox );
  gtk_container_add( GTK_CONTAINER( methodAlignment ), methodChoiceBox );

  // COMBO BOX SETTINGS
  methodCombo = gtk_combo_box_text_new();
  gtk_box_pack_start (GTK_BOX ( methodChoiceBox ), methodCombo, false, false, 6);
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(methodCombo), LBP_CHOICE.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(methodCombo), mLBP_CHOICE.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(methodCombo), ULBP_CHOICE.c_str());
  gtk_combo_box_set_active (GTK_COMBO_BOX(methodCombo), 0);
  gtk_widget_show (methodCombo);

  //preview = gimp_drawable_preview_new( drawable, &GLOBAL_UI_INFO.preview );
  preview = gimp_drawable_preview_new_from_drawable_id( drawable->drawable_id );
  gtk_box_pack_start( GTK_BOX( mainVBox ), preview, true, true, 0 );
  gtk_widget_show( preview );

  ProcessImage( drawable, GIMP_PREVIEW(preview) );

  g_signal_connect_swapped( preview, "invalidated",
                            G_CALLBACK( ProcessImage ), 
                            drawable );

  g_signal_connect_swapped( radiusSpinButtonAdj, "value_changed",
                            G_CALLBACK( gimp_preview_invalidate ),
                            preview );

  g_signal_connect_swapped( pointsSpinButtonAdj, "value_changed",
                            G_CALLBACK( gimp_preview_invalidate),
                            preview );
  
  g_signal_connect_swapped( methodCombo, "changed",
                            G_CALLBACK( gimp_preview_invalidate),
                            preview );

  g_signal_connect (radiusSpinButtonAdj, "value_changed",
                    G_CALLBACK (gimp_int_adjustment_update),
                    &GLOBAL_UI_INFO.radius);

  g_signal_connect (pointsSpinButtonAdj, "value_changed",
                    G_CALLBACK (gimp_int_adjustment_update),
                    &GLOBAL_UI_INFO.points);

  g_signal_connect (methodCombo, "changed",
                    GTK_SIGNAL_FUNC( methodChanged ),
                    methodCombo );

  // show plugin's main GUI window
  gtk_widget_show (dialog);

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}