# psfedit

A basic psf2 font editor and viewer.

## Screenshots

### CLI
<img width="1894" height="601" alt="image" src="https://github.com/user-attachments/assets/0ff1af1e-6260-4ec7-a507-340f079d8ccb" />

### Editor
### Main screen
<img width="1894" height="935" alt="image" src="https://github.com/user-attachments/assets/b2f1ed0f-ae42-4d20-8cb7-d70ce8437f50" />

### Help Window
<img width="1894" height="935" alt="image" src="https://github.com/user-attachments/assets/18ec7b46-19f6-48ea-9954-7726a5c88dc7" />

### Overview screen
<img width="1894" height="935" alt="image" src="https://github.com/user-attachments/assets/7405f1ba-1e8f-4e16-9a38-049857eb3c29" />

## Installation
 - [x86_64 Linux](#linux-users-on-x86_64)
 - [Other operating systems](#compile-it-yourself)
### Linux users on x86_64
You can find compiled binaries at the [releases page](https://github.com/AntKru/psfedit/releases).
The binary is in the bin directory inside the archive.
> [!NOTE]
> You still have to install GNU readline and ncurses.

After that you can skip to [Usage](#Usage)

### Compile it yourself
You have to compile it on your own, but here is a simple guide.
I have tested it on Debian trixie and Arch.
You have to install
 - meson (`meson` package on Debian)
 - GNU readline (`libreadline-dev` package on Debian)
 - ncurses (`libncurses-dev` package on Debian)
 - C++ compiler with C++23 support (`g++` package on Debian)

### 1. Download the source code
If you just want the **latest release**, download the file `psfedit-vX.X.X.tar.xz` from [here](https://github.com/AntKru/psfedit/releases). Then do the following:
```sh
# In your downloads directory
tar -xf release-name.tar.xz
cd release-name
```
If you want to work on this or have the **newest changes**, you should clone with git:
```sh
git clone https://github.com/AntKru/psfedit
cd psfedit
```

### 2. Compile the source code
```sh
meson setup build
cd build
meson compile
```
> [!TIP]
> If it does not work, make sure that
>  - You have a compiler with C++23 support (see https://en.wikipedia.org/wiki/C%2B%2B23#Compiler_support)
>  - You are in the directory of the downloaded source code (not the `src` directory)
>  If you are sure you have done everything correctly, please create an issue

You should now be able to use the editor by using the binary in the build directory:
```sh
./psfedit -h
```

### 3. Installation (optional)
Still in the same directory:
```sh
meson install
```
This will make the binary available in your path.
> [!TIP]
> You can uninstall psfedit with the following command with root privileges:
> ```sh
> ninja uninstall
> ```
> Make sure that you are in the build directory while doing so.

## Usage
The basic way to use the editor is like this:
```sh
psfedit [FILE PATH]
```
This will put you in an interactive mode, where you can use the `help` command to learn how to use the editor.
After entering editing mode with `m [character]`, press `?` to open the help window. You can scroll down to see all keybinds.

You can also create new fonts by using `-c`/`-u` options. For more information:
```sh
psfedit --help
```
If you just want to try it out, you could find fonts in `/usr/share/kbd/consolefonts` or `/usr/share/consolefonts`, depending on your distro.
You might have to `gunzip` them.

## Used libraries
 - GNU readline (GPLv3): https://tiswww.cwru.edu/php/chet/readline/rltop.html
 - ncurses (X11 License): https://invisible-island.net/ncurses/
