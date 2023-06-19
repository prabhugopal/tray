#ifndef TRAY_H
#define TRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

struct tray {
  const char *icon_name;
  char *tooltip;
  void (*cb)(struct tray *); // called on left click, leave null to just open menu
  struct tray_menu *menu;
};

struct tray_menu {
  const char *text;
  int disabled;
  int checked;
  void (*cb)(struct tray_menu *);
  struct tray_menu *submenu;
};

extern struct tray *tray_instance;

int tray_init(struct tray *tray);

int tray_loop(int blocking);

void tray_update(struct tray *tray);

//TODO: Can we drop this?  It is harmful on MacOS.
void tray_exit(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* TRAY_H */
