#include "tray.h"
#include <string.h>
#include <stddef.h>
#include <libayatana-appindicator/app-indicator.h>

#define TRAY_APPINDICATOR_ID "tray-id"

static struct tray *tray_instance;
static AppIndicator *indicator = NULL;
static bool continue_running = true;

static void _tray_cb(GtkMenuItem *item, gpointer data) {
    (void)item;
    struct tray_menu_item *m = (struct tray_menu_item *)data;
    m->cb(m);
}

static void _tray_menu_item_cb(GtkMenuItem *item, gpointer data) {
  (void)item;
  struct tray_menu_item *m = (struct tray_menu_item *)data;
  m->cb(m);
}

static GtkMenuShell *_tray_menu_item(struct tray_menu_item *m) {
  GtkMenuShell *menu = (GtkMenuShell *)gtk_menu_new();
  for (; m != NULL && m->text != NULL; m++) {
    GtkWidget *item;
    if (strcmp(m->text, "-") == 0) {
      item = gtk_separator_menu_item_new();
    } else {
      if (m->submenu != NULL) {
        item = gtk_menu_item_new_with_label(m->text);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),
                                  GTK_WIDGET(_tray_menu_item(m->submenu)));
      } else {
        item = gtk_check_menu_item_new_with_label(m->text);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), !!m->checked);
      }
      gtk_widget_set_sensitive(item, !m->disabled);
      if (m->cb != NULL) {
        g_signal_connect(item, "activate", G_CALLBACK(_tray_menu_item_cb), m);
      }
    }
    gtk_widget_show(item);
    gtk_menu_shell_append(menu, item);
  }
  return menu;
}

struct tray * tray_get_instance() {
  return tray_instance;
}

bool tray_init(struct tray *tray) {
  if (gtk_init_check(0, NULL) == FALSE) {
    return false;
  }
  indicator = app_indicator_new(TRAY_APPINDICATOR_ID, tray->icon_filepath,
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  tray_update(tray);
  return true;
}

bool tray_loop(bool blocking) {
  gtk_main_iteration_do(blocking);
  return continue_running;
}

void tray_update(struct tray *tray) {
    struct _GtkMenu *menu = GTK_MENU(_tray_menu_item(tray->menu));
  app_indicator_set_menu(indicator, GTK_MENU(_tray_menu_item(tray->menu)));
  app_indicator_set_icon(indicator, tray->icon_filepath);
  // GTK is all about reference counting, so previous menu should be destroyed
  // here
  //app_indicator_set_menu(indicator, GTK_MENU(_tray_menu_item(tray->menu)));

  tray_instance = tray;
}

void tray_exit(void) { continue_running = false; }

