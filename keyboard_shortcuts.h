#ifndef KEYBOARD_SHORTCUTS_H
#define KEYBOARD_SHORTCUTS_H

#include <stdbool.h>
#include <X11/Xlib.h>

#define SHORTCUT_DELIMITER "+"

bool parse_shortcut(const char *shortcut, KeySym *keysym, unsigned int *modifiers);

#endif
