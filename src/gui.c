#include "gui.h"

int main(int argc, char* argv[]) {
  gtk_disable_setlocale();
  gtk_init(&argc, &argv);
  window_init();
  gtk_widget_show_all(main_window);
  gtk_main();
}

void on_draw(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
  GdkRectangle dar;
  gdouble dx = 0.5, dy = 0.5;
  GdkWindow* drwindow = gtk_widget_get_window(widget);
  gdk_window_get_geometry(drwindow, &dar.x, &dar.y, &dar.width, &dar.height);

  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(bg_color), &bgcol);
  cairo_set_source_rgba(cr, bgcol.red, bgcol.green, bgcol.blue, bgcol.alpha);
  cairo_paint(cr);

  cairo_translate(cr, dar.width / 2, dar.height / 2);
  cairo_scale(cr, ZOOM_X, -ZOOM_Y);

  cairo_device_to_user_distance(cr, &dx, &dy);
  gdk_cairo_get_clip_rectangle(cr, &dar);
  draw_axes(cr, dar, dx);
  if (parsed) {
    draw_facets(cr, dx);
    draw_vertex(cr, dx);
  }
}

void draw_axes(cairo_t* cr, GdkRectangle dar, gdouble dx) {
  cairo_set_font_size(cr, dx * 25);
  cairo_matrix_t font_reflection_matrix;
  cairo_get_font_matrix(cr, &font_reflection_matrix);
  font_reflection_matrix.yy = font_reflection_matrix.yy * -1;
  cairo_set_font_matrix(cr, &font_reflection_matrix);
  cairo_set_line_width(cr, dx);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
  cairo_move_to(cr, 0.0, 0.0);
  cairo_line_to(cr, dar.width / 2, 0.0);
  cairo_move_to(cr, 0.0, 1.0);
  cairo_show_text(cr, "z");
  cairo_move_to(cr, 0.0, 0.0);
  cairo_line_to(cr, 0.0, dar.height / 2);
  cairo_move_to(cr, 1.0, 0.0);
  cairo_show_text(cr, "y");
  cairo_move_to(cr, 0.0, 0.0);
  cairo_line_to(cr, -3 * dar.height * TRANS_X, -3 * dar.height * TRANS_Y);
  cairo_move_to(cr, -3.0 * TRANS_X, -3.0 * TRANS_Y);
  cairo_show_text(cr, "x");
  cairo_stroke(cr);
}

void close_app(GtkWidget* widget, gpointer user_data) {
  save_conf();
  if (object.count_of_vertexes) clean_obj(&object);
  remove_trash();
  gtk_main_quit();
}

void file_set(GtkFileChooserButton* widget, gpointer user_data) {
  gtk_spin_button_set_value(spin_x, 0);
  gtk_spin_button_set_value(spin_y, 0);
  gtk_spin_button_set_value(spin_z, 0);

  char filename[256];
  char info[256];
  if (object.count_of_vertexes) clean_obj(&object);
  char* tmp = (char*)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
  snprintf(filename, sizeof(filename), "%s", tmp);
  if (parse(filename, &object) == 0) parsed = 1;
  tmp_mat = object.matrix_3d;
  snprintf(info, sizeof(info), "Object info:\n\nVertices - %d\n\nFacets - %d\n",
           object.count_of_vertexes, object.count_of_facets);
  gtk_label_set_text(obj_info, info);
}

void draw_facets(cairo_t* cr, gdouble dx) {
  cairo_set_line_width(cr, dx * linewdth);
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(lines_color), &lcol);
  cairo_set_source_rgba(cr, lcol.red, lcol.green, lcol.blue, lcol.alpha);
  if (line_type) {
    const double dashes[2] = {0.05, 0.1};
    cairo_set_dash(cr, dashes, 1, 0.0);
  }
  for (int f = 0; f < object.count_of_facets; f++) {
    for (int v = 0; v < object.polygons[f].numbers_of_vertexes_in_facets; v++) {
      int facet = object.polygons[f].vertexes[v];
      if (facet) {
        double z = object.matrix_3d.matrix[0][facet] + trx;
        double x = object.matrix_3d.matrix[1][facet] + try;
        double y = object.matrix_3d.matrix[2][facet] + trz;
        if (pr_type) {
          x /= (1 - z * 0.01);
          y /= (1 - z * 0.01);
        }
        if (v == 0) cairo_move_to(cr, x - z * TRANS_X, y - z * TRANS_Y);
        cairo_line_to(cr, x - z * TRANS_X, y - z * TRANS_Y);
      }
    }
    cairo_close_path(cr);
  }
  cairo_stroke(cr);
}

void draw_vertex(cairo_t* cr, gdouble dx) {
  dm = gtk_combo_box_get_active_id(disp_method);
  cairo_set_line_width(cr, dx * vertsize * 1.5);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(vertex_color), &vcol);
  cairo_set_source_rgba(cr, vcol.red, vcol.green, vcol.blue, vcol.alpha);
  if (*dm != 'n') {
    if (*dm == 's') {
      cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
    } else {
      cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    }
  }
  for (int i = 1; i < object.count_of_vertexes + 1; i++) {
    double z = object.matrix_3d.matrix[0][i] + trx;
    double x = object.matrix_3d.matrix[1][i] + try;
    double y = object.matrix_3d.matrix[2][i] + trz;
    if (pr_type) {
      x /= (1 - z * 0.01);
      y /= (1 - z * 0.01);
    }
    cairo_move_to(cr, x - z * TRANS_X, y - z * TRANS_Y);
    cairo_line_to(cr, x - z * TRANS_X + 0.0005, y - z * TRANS_Y + 0.0005);
  }
  cairo_stroke(cr);
  gtk_widget_queue_draw(graph_window);
}

void translatex(GtkSpinButton* spin_button, gpointer data) {
  trx = (int)gtk_spin_button_get_value(spin_button);
  gtk_widget_queue_draw(graph_window);
  if (toggled) make_pic();
}

void translatey(GtkSpinButton* spin_button, gpointer data) {
  try = (int)gtk_spin_button_get_value(spin_button);
  gtk_widget_queue_draw(graph_window);
  if (toggled) make_pic();
}

void translatez(GtkSpinButton* spin_button, gpointer data) {
  trz = (int)gtk_spin_button_get_value(spin_button);
  gtk_widget_queue_draw(graph_window);
  if (toggled) make_pic();
}

void zoom_in(GtkButton* button, gpointer data) {
  ZOOM_X *= 1.5;
  ZOOM_Y *= 1.5;
  gtk_widget_queue_draw(graph_window);
  if (toggled) make_pic();
}

void zoom_out(GtkButton* button, gpointer data) {
  ZOOM_X /= 1.5;
  ZOOM_Y /= 1.5;
  gtk_widget_queue_draw(graph_window);
  if (toggled) make_pic();
}

void window_init() {
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "3d_viewer.glade", NULL);
  main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  graph_window = GTK_WIDGET(gtk_builder_get_object(builder, "graph_window"));
  fc = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file_chooser"));
  da = GTK_WIDGET(gtk_builder_get_object(builder, "da"));
  dialog = GTK_FILE_CHOOSER_DIALOG(gtk_builder_get_object(builder, "dialog"));
  g_signal_connect(G_OBJECT(dialog), "delete-event",
                   G_CALLBACK(gtk_widget_hide_on_delete), NULL);
  chooser = GTK_FILE_CHOOSER(dialog);
  zoomin = GTK_BUTTON(gtk_builder_get_object(builder, "plus_scale_btn"));
  zoomout = GTK_BUTTON(gtk_builder_get_object(builder, "minus_scale_btn"));
  spin_x = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_rot_x"));
  spin_y = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_rot_y"));
  spin_z = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "spin_rot_z"));
  lines_color =
      GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "lines_color"));
  vertex_color =
      GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "vertex_color"));
  bg_color = GTK_COLOR_BUTTON(gtk_builder_get_object(builder, "bg_color"));
  line_width = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "line_width"));
  vert_size = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "vert_size"));
  disp_method = GTK_COMBO_BOX(gtk_builder_get_object(builder, "disp_method"));
  c_p_rb = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "c_p_rb"));
  d_l_rb = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "d_l_rb"));
  obj_info = GTK_LABEL(gtk_builder_get_object(builder, "obj_info"));
  gtk_builder_connect_signals(builder, NULL);
  read_conf("viewer.conf");
  g_object_unref(builder);
}

void on_rotate(GtkSpinButton* spin_button, gpointer user_data) {
  double x = gtk_spin_button_get_value(spin_x);
  double y = gtk_spin_button_get_value(spin_y);
  double z = gtk_spin_button_get_value(spin_z);
  object.matrix_3d = tmp_mat;
  ROT_X = GR2RAD * x;
  ROT_Y = GR2RAD * y;
  ROT_Z = GR2RAD * z;
  matrix_t rotx = create_matrix(3, 3);
  matrix_t roty = create_matrix(3, 3);
  matrix_t rotz = create_matrix(3, 3);
  set_rot_matrix_x(&rotx, ROT_X);
  set_rot_matrix_y(&roty, ROT_Y);
  set_rot_matrix_z(&rotz, ROT_Z);
  object.matrix_3d = mult_matrix(&rotx, &object.matrix_3d);
  object.matrix_3d = mult_matrix(&roty, &object.matrix_3d);
  object.matrix_3d = mult_matrix(&rotz, &object.matrix_3d);
  remove_matrix(&rotx);
  remove_matrix(&roty);
  remove_matrix(&rotz);
  gtk_widget_queue_draw(graph_window);
  if (toggled) make_pic();
}

void set_lines_color(GtkColorButton* widget, gpointer user_data) {
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &lcol);
}

void set_vertex_color(GtkColorButton* widget, gpointer user_data) {
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &vcol);
}

void set_bg_color(GtkColorButton* widget, gpointer user_data) {
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(widget), &bgcol);
}

void set_line_width(GtkSpinButton* spin_button, gpointer user_data) {
  linewdth = gtk_spin_button_get_value(line_width);
  gtk_widget_queue_draw(graph_window);
}

void set_vert_size(GtkSpinButton* spin_button, gpointer user_data) {
  vertsize = gtk_spin_button_get_value(vert_size);
  gtk_widget_queue_draw(graph_window);
}

void projection_type_c(GtkToggleButton* togglebutton, gpointer user_data) {
  pr_type = 1;
  gtk_widget_queue_draw(graph_window);
}

void projection_type_p(GtkToggleButton* togglebutton, gpointer user_data) {
  pr_type = 0;
  gtk_widget_queue_draw(graph_window);
}

void line_type_d(GtkToggleButton* togglebutton, gpointer user_data) {
  line_type = 1;
  gtk_widget_queue_draw(graph_window);
}

void line_type_s(GtkToggleButton* togglebutton, gpointer user_data) {
  line_type = 0;
  gtk_widget_queue_draw(graph_window);
}

void save_conf() {
  FILE* config = fopen("viewer.conf", "w");
  if (config) {
    fprintf(config, "projection_type = %d\n", pr_type);
    fprintf(config, "line_type = %d\n", line_type);
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(lines_color), &lcol);
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(vertex_color), &vcol);
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(bg_color), &bgcol);
    fprintf(config, "lines_color = %s\n", gdk_rgba_to_string(&lcol));
    fprintf(config, "vert_color = %s\n", gdk_rgba_to_string(&vcol));
    fprintf(config, "bg_color = %s\n", gdk_rgba_to_string(&bgcol));
    fprintf(config, "line_size = %.f\n", linewdth);
    fprintf(config, "vert_size = %.f\n", vertsize);
    fprintf(config, "display_method = %s\n",
            gtk_combo_box_get_active_id(disp_method));
    fclose(config);
  }
}

void read_conf(char* conf_file) {
  FILE* config = fopen(conf_file, "r");
  if (config) {
    char buff[256];
    fscanf(config, "projection_type = %d", &pr_type);
    if (pr_type) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(c_p_rb), TRUE);
    }
    fscanf(config, "%*s %*s %d\n", &line_type);
    if (line_type) {
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(d_l_rb), TRUE);
    }
    fscanf(config, "%*s %*s %255s\n", buff);
    gdk_rgba_parse(&lcol, buff);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(lines_color), &lcol);
    fscanf(config, "%*s %*s %255s\n", buff);
    gdk_rgba_parse(&vcol, buff);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(vertex_color), &vcol);
    fscanf(config, "%*s %*s %255s\n", buff);
    gdk_rgba_parse(&bgcol, buff);
    gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(bg_color), &bgcol);
    fscanf(config, "%*s %*s %lf\n", &linewdth);
    gtk_spin_button_set_value(line_width, linewdth);
    fscanf(config, "%*s %*s %lf\n", &vertsize);
    gtk_spin_button_set_value(vert_size, vertsize);
    fscanf(config, "%*s %*s %255s\n", buff);
    gtk_combo_box_set_active_id(disp_method, buff);
    fclose(config);
  }
}

void run_save(GtkButton* button, gpointer user_data) {
  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (!res) g_print("error");
}

void save_as(GtkButton* button, gpointer user_data) {
  const char* button_text = (const char*)gtk_button_get_label(button);
  char* filename = NULL;
  filename = gtk_file_chooser_get_filename(chooser);
  if (filename) {
    strcat(filename, ".");
    strcat(filename, button_text);
  } else {
    filename = (char*)calloc(13, sizeof(char));
    strcpy(filename, "untitled.");
    strcat(filename, button_text);
  }

  save_as_png("tmp.png", 0);

  MagickWandGenesis();
  MagickWand* mw = NewMagickWand();
  if (MagickReadImage(mw, "tmp.png") == MagickTrue) {
    MagickWriteImage(mw, filename);
    remove("tmp.png");
  }
  if (mw) DestroyMagickWand(mw);
  MagickWandTerminus();

  gtk_widget_hide(GTK_WIDGET(dialog));
  if (filename) free(filename);
}

void save_as_png(char* filename, int mode) {
  GtkAllocation alloc;
  int width, height;
  gtk_widget_get_allocation(graph_window, &alloc);
  if (mode) {
    width = 640;
    height = 480;
  } else {
    width = alloc.width;
    height = alloc.height;
  }
  cairo_surface_t* surface =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
  cairo_t* cr = cairo_create(surface);
  GdkWindow* drwindow = gtk_widget_get_window(graph_window);
  gdk_cairo_set_source_window(cr, drwindow, (width - alloc.width) / 2,
                              (height - alloc.height) / 2);
  cairo_paint(cr);
  cairo_surface_write_to_png(surface, filename);
}

void make_gif(GtkToggleButton* gif_toggle_button, gpointer user_data) {
  toggled = gtk_toggle_button_get_active(gif_toggle_button);
  if (!toggled) {
    char* filename = (char*)calloc(15, sizeof(char));
    MagickWandGenesis();
    MagickWand* mw = NewMagickWand();
    for (int i = 0; i < 50; i++) {
      sprintf(filename, "pics/%d.png", i);
      MagickReadImage(mw, filename);
      MagickSetImageDelay(mw, 10);
    }
    MagickWriteImages(mw, "output.gif", MagickTrue);
    file_counter = 0;
    remove_trash();
    if (filename) free(filename);
    if (mw) DestroyMagickWand(mw);
    MagickWandTerminus();
  }
}

void make_pic() {
  char* filename = (char*)calloc(15, sizeof(char));
  sprintf(filename, "pics/%d.png", file_counter);
  save_as_png(filename, 1);
  file_counter++;
  file_counter = file_counter % 50;
  if (filename) free(filename);
}

void remove_trash() {
  char* filename = (char*)calloc(15, sizeof(char));
  for (int i = 0; i < 50; i++) {
    sprintf(filename, "pics/%d.png", i);
    remove(filename);
  }
  if (filename) free(filename);
}
