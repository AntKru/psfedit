# psfedit

A psf editor and viewer.

## Building
Requirements:
 - meson
 - C++ compiler with support for C++23
 - GNU readline library
```sh
meson setup build
cd build
ninja
```

## Installation
```sh
meson setup build
cd build
meson install
```

## Usage
```sh
psfedit [FILE PATH]
```

