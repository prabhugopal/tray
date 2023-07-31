#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QEventLoop>
#include <QTimer>
#include "tray.h"

static QApplication *app = nullptr;
static QSystemTrayIcon *trayIcon = nullptr;
static tray *tray = nullptr;
static bool shouldExit = false;

static void updateMenu(tray_menu_item *menu, QMenu *qmenu) {
    for (; menu != NULL && menu->text != NULL; menu++) {
        if (strcmp(menu->text, "-") == 0) {
            qmenu->addSeparator();
        } else {
            QAction *action = qmenu->addAction(menu->text);
            action->setDisabled(menu->disabled);
            action->setCheckable(true);
            action->setChecked(menu->checked);
            QObject::connect(action, &QAction::triggered, [=]() { if (menu->cb) menu->cb(menu); });

            if (menu->submenu != NULL) {
                QMenu *submenu = new QMenu;
                action->setMenu(submenu);
                updateMenu(menu->submenu, submenu);
            }
        }
    }
}

extern "C" {

TRAY_EXPORT
struct tray * tray_get_instance() {
    return tray;
}

TRAY_EXPORT
int tray_init(struct tray *t) {
    int argc = 0;
    char *argv[] = { nullptr };

    app = new QApplication(argc, argv);
    trayIcon = new QSystemTrayIcon;
    tray = t;

    trayIcon->setIcon(QIcon(tray->icon_filepath));
    QObject::connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            if (tray->cb)
                tray->cb(tray);
        }
    });

    tray_update(tray);

    return 0;
}

TRAY_EXPORT
int tray_loop(int blocking) {
    QEventLoop loop;
    QTimer::singleShot(blocking ? -1 : 0, &loop, &QEventLoop::quit);
    loop.exec();

    if (shouldExit)
        return -1;

    return 0;
}

TRAY_EXPORT
void tray_update(struct tray *t) {
    QMenu *menu = new QMenu;
    updateMenu(t->menu, menu);

    trayIcon->setContextMenu(menu);
    trayIcon->setToolTip(t->tooltip);
    trayIcon->show();
}

TRAY_EXPORT
void tray_exit(void) {
    shouldExit = true;
}

} // extern "C"
