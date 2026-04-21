# psfedit

A basic psf2 font editor and viewer.

## Screenshots

### CLI
<img width="566" height="180" alt="image" src="https://github.com/user-attachments/assets/72de53fb-c8c5-4645-a099-89c4538e367c" />

<img width="198" height="121" alt="image" src="https://github.com/user-attachments/assets/5ad2c5c8-2e02-4671-a6f7-e12b437d4d49" />

### Editor
### Main screen
<img width="1897" height="951" alt="image" src="https://github.com/user-attachments/assets/c5a92c3d-a20b-473c-a632-450edc817cef" />
### Help Window
<img width="1884" height="500" alt="image" src="https://github.com/user-attachments/assets/1aefdc4a-7020-4821-995f-75355e7ea21c" />
### Overview screen
<img width="1884" height="887" alt="image" src="https://github.com/user-attachments/assets/d18fa420-ade9-4b88-b800-dade9b3dd6a4" />

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
