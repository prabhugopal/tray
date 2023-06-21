![tray](tray.jpg)
# System Tray / Menu Bar / Indicator Icon

Cross-platform, super tiny C99 implementation of a system tray/menu bar icon with a popup menu and 
optional primary-click callback. The callback allows primary-click to hide/show a window, for example,
while secondary-click shows a menu.  If no callback is specified, either click will show the menu.

The system can be dynamically updated; icon, tooltip, menu items and status (checked/unchecked &
enabled/disabled) can all be both queried and changed at runtime. Code is C++ friendly and will compile fine in C++98 and up.  This fork is intended to make the 
functionality available as a library, for use from other languages.

Focussed PRs are welcome but please note that the goal is to keep the code as simple as possible.
Functionality beyond presenting a tray icon and menu is out of scope.

## Cross-platform

Works well on:

* Windows XP or newer (shellapi.h)
* MacOS (Cocoa/AppKit)

Known issues:

* Linux/Gtk (libappindicator)

Gnome has decided to deprecate the tray icon as a concept, except for system indicators. This code 
will still work if you have a system indicator installed, but often seems to require root privileges
to render the icon.  It is possible to install a system indicator on Ubuntu 22.04, but without root 
privileges, the icon will not render (it will just appear as "..." in the tray) although the menu
will still work.  This is a known issue and might be resolved by switching to a 
[Qt-based implementation](https://doc.qt.io/qt-6/qsystemtrayicon.html) on Linux. PR's that accomplish 
this are welcome.

## API

The `tray` structure defines the tray interface and a nested menu of NULL-terminated array of entries.
`tray_menu` defines each menu entry text, menu checked and disabled (grayed) flags.

The `tray` and `tray_menu` each have an optional callback if they are selected.

```c
struct tray {
  const char *icon_name;
  char *tooltip;
  void (*cb)(struct tray *); // called on left click, leave null to just open menu
  struct tray_menu *menu;
};

struct tray_menu {
  char *text;
  int disabled;
  int checked;
  void (*cb)(struct tray_menu *);
  struct tray_menu *submenu;
};
```

* `int tray_init(struct tray *)` - creates tray icon. Returns -1 if tray icon/menu can't be created.
* `struct tray * tray_get_instance()` - returns the tray instance.
* `void tray_update(struct tray *)` - updates tray icon and menu.
* `int tray_loop(int blocking)` - runs one iteration of the UI loop. Returns -1 if `tray_exit()` has been called.
* `void tray_exit()` - terminates UI loop.

All functions are meant to be called from the UI thread only.

Menu arrays must be terminated with a NULL item, e.g. the last item in the
array must have text field set to NULL.

## Icons

Icons are platform-specific but generally should have transparent backgrounds and be simple:
- Windows: .ICO with 16x16 & 32x32 sizes included.
- MacOS: .PNG with a notional 22pt height or vector-based .PDF with black-and-white images.
- Linux: .PNG 24x24 pixels.

Tray does not provide any theming or icon management.  It is up to the application to respond
to theme changes and supply appropriate icons e.g. dark mode.

## Prerequisites

* CMake
* [Ninja](https://ninja-build.org/), in order to have the same build commands on all platforms
* AppIndicator on Linux: `sudo apt install libappindicator3-dev`

## Building

```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

## Demo

Build & execute the `tray_example` application:

```
./tray_example
```

## Screenshots

![MacOS screenshot](./screenshot_macosx.png)

![Windows screenshot](./screenshot_windows.png)

![Linux screenshot](./screenshot_linux.png)

## History

This fork is in turn based on a previous [fork by dmikushin](https://github.com/dmikushin/tray) that brings together the
[original work of Serge Zaitsev](https://github.com/zserge/tray) and "the most interesting forks and PRs of
respectable contributors" including:

* [Only process messages coming from the tray window on Windows](https://github.com/zserge/tray/pull/18)
* [Become C++-friendly](https://github.com/zserge/tray/pull/16)
* [Fix all menu items have a check box](https://github.com/zserge/tray/pull/11)
* [Add support for tooltip](https://github.com/zserge/tray/pull/11)
* Darwin implementation translated from C to Objective C adapted from [@trevex fork](https://github.com/trevex/tray)
