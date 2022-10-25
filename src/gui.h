#ifndef SRC_GUI_H_
#define SRC_GUI_H_

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wand/MagickWand.h>

#include "parse.h"

// Коэффициенты масштабирования для выравнивания пропорций
#define TRANS_X 0.288888
#define TRANS_Y 0.166667

// Градусы в радианы
#define GR2RAD 0.0174533

double ZOOM_X = 100.0;
double ZOOM_Y = 100.0;
int trx = 0;
int try = 0;
int trz = 0;
double ROT_X = 0;
double ROT_Y = 0;
double ROT_Z = 0;
double linewdth = 1;
double vertsize = 1;
matrix_t tmp_mat;
int pr_type = 0;
int line_type = 0;
int parsed;
int file_counter = 0;
int toggled = 0;
const gchar* dm;

GtkBuilder* builder;
GtkWidget* main_window;
GtkWidget* graph_window;
GtkWidget* da;
GtkFileChooserDialog* dialog;
GtkFileChooserButton* fc;
GtkFileChooser* chooser;
GtkButton* zoomin;
GtkButton* zoomout;
GtkSpinButton* spin_x;
GtkSpinButton* spin_y;
GtkSpinButton* spin_z;
GtkSpinButton* line_width;
GtkSpinButton* vert_size;
GtkRadioButton* c_p_rb;
GtkRadioButton* d_l_rb;
FILE* obj_file;
data_t object;
GtkColorButton* lines_color;
GdkRGBA lcol;
GtkColorButton* vertex_color;
GdkRGBA vcol;
GtkColorButton* bg_color;
GdkRGBA bgcol;
GtkComboBox* disp_method;
GtkLabel* obj_info;

void on_draw(GtkWidget* widget, cairo_t* cr, gpointer user_data);
void draw_axes(cairo_t* cr, GdkRectangle dar, gdouble dx);
void close_app(GtkWidget* widget, gpointer user_data);
void file_set(GtkFileChooserButton* widget, gpointer user_data);
void draw_facets(cairo_t* cr, gdouble dx);
void draw_vertex(cairo_t* cr, gdouble dx);
void zoom_in(GtkButton* button, gpointer data);
void zoom_out(GtkButton* button, gpointer data);
void translatex(GtkSpinButton* spin_button, gpointer data);
void translatey(GtkSpinButton* spin_button, gpointer data);
void translatez(GtkSpinButton* spin_button, gpointer data);
void window_init();
void on_rotate(GtkSpinButton* spin_button, gpointer user_data);
void save_conf();
void read_conf(char* conf_file);

#endif  // SRC_GUI_H_
void save_as(GtkButton *jpeg_button, gpointer user_data);
void save_as_png(char* filename, int mode);
void run_save(GtkButton* button, gpointer user_data);
void make_gif(GtkToggleButton *gif_toggle_button, gpointer user_data);
void make_pic();
void remove_trash();
