#ifndef INTERFACE_H
#define	INTERFACE_H
#include <gtk/gtk.h>

void initInterface(int argc,char *argv[]);
void on_button_close();
void on_img_chooser_file_set(GtkFileChooserButton *f);
void on_new_activated(GtkMenuItem *m);
void on_open_activated(GtkMenuItem *m);
void on_quit_activated(GtkMenuItem *m);

#endif
