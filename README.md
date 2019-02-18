# mapmywindows

[![Build status](https://ci.appveyor.com/api/projects/status/phnu2603i6v8c1te?svg=true)](https://ci.appveyor.com/project/TheDcoder/mapmywindows)

A small progarm to show and hide windows in X window system. Most Linux distributions use a display server with X window system, you can easily hide and show windows using this program.

## Usage

`mapmywindows` runs as a daemon (i.e it keeps running in the background until exit), once executed you can use keyboard shortcuts/macros to perform actions. Here are the currently available actions and their default keyboard shortcut combination:

1. `Ctrl+Shift+F7` - Hide active window
2. `Ctrl+Shift+F8` - Show the last hidden window
3. `Ctrl+Shift+F9` - Exit the program

For more use `mapmywindows -h` to display the help text with usage syntax and options.

## Download

Binary packages are available on the [releases](https://github.com/TheDcoder/mapmywindows/releases) page, but may fail to run due to missing dependencies or differences between OS distributions, the packages are compiled in Fedora. Especially the library `libxdo` might be missing and you may have to install it before mapmywindows works.

## Compiling and Installing

I have developed and tested exclusively on Fedora KDE Spin, so these instructions should work well if you are using Fedora. 

### Requirements
1. `libX11` or `Xlib` - Library to communicate with X
2. `libxdo` - Helper library which contains useful functions related to X
3. `libxkeymacro` - Own hand-crafted library to deal with keyboard macro parsing and handling, already included in the repository  as a sub-module!

Install all dependencies on Fedora:
```
$ sudo dnf install libX11 libxdo
$ sudo dnf install libX11-devel libxdo-devel # Development headers for compiling
```

### Compile

```
$ make
```

### Install

```
$ sudo make install
```

## Acknowledgements

This is my first proper program written in C!

After switching from Windows to Linux as my primary operating system a few months back, I was missing the functionality to hide and show certain windows. A program called [Hide my Windows](https://funk.eu/hmw/) made this possible in Windows but I could not find a good alternative program in Linux to replace it, so I started my own journey to write a small replacement program which gets the job done, and here I am after about an year or so.

I started experimenting with different kind of programming languages (shell, euphoria to name a few) but ultimately settled on C because it closely matched my principles and has a mature and widely supported community and codebase.

Thanks to several folks at freenode (##c, ##programming etc.) who have helped me reach this point. I hope to keep learning more from them. And to not forget my friends who were supportive during the whole development process :)
