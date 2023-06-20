#ifndef TRAY_H
#define TRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#if WIN32
#ifdef TRAY_EXPORTS
#define TRAY_EXPORT __declspec(dllexport)
#else
#define TRAY_EXPORT __declspec(dllimport)
#endif
#else
#ifdef __GNUC__ >= 4 || defined(__clang__)
#define TRAY_EXPORT __attribute__((visibility("default")))
#else
#define TRAY_EXPORT
#endif
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

TRAY_EXPORT
struct tray * tray_get_instance();

TRAY_EXPORT
int tray_init(struct tray *tray);

TRAY_EXPORT
int tray_loop(int blocking);

TRAY_EXPORT
void tray_update(struct tray *tray);

TRAY_EXPORT
void tray_exit(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* TRAY_H */
