# psfedit

A basic psf2 font editor and viewer.

## Screenshots

<img width="566" height="180" alt="image" src="https://github.com/user-attachments/assets/72de53fb-c8c5-4645-a099-89c4538e367c" />

### Viewer
<img width="198" height="121" alt="image" src="https://github.com/user-attachments/assets/5ad2c5c8-2e02-4671-a6f7-e12b437d4d49" />

### Editor
<img width="256" height="238" alt="image" src="https://github.com/user-attachments/assets/0b8b3cbc-56bd-4fb3-8871-807309996394" />

## Installation
**For Linux users on x86_64:**
You can find compiled binaries at the [releases page](https://github.com/AntKru/psfedit/releases).
The binary is in the bin directory inside the archive.
You still have to install GNU readline.
After that you can skip to [Usage](#Usage)

**For eveyone else:**
You have to compile it on your own, but here is a simple guide.
I have tested it on Debian trixie and Arch.
You have to install
 - meson (`meson` package on Debian)
 - GNU readline (`libreadline-dev` package on Debian)
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
> [!IMPORTANT]
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
You can also create new fonts by using `-c`/`-u` options. For more information:
```sh
psfedit --help
```
If you just want to try it out, you could find fonts in `/usr/share/kbd/consolefonts` or `/usr/share/consolefonts`, depending on your distro.
You might have to `gunzip` them.

## Used libraries
 - GNU readline (GPLv3): https://tiswww.cwru.edu/php/chet/readline/rltop.html
