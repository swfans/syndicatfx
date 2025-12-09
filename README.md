# syndicatfx

**SyndicatFX**, alternative binary for the classic Bullfrog game.

## About

**SyndicatFX** is a port of the 1993 DOS game Syndicate to modern
operating systems. In particular, it runs on GNU, Mac OS and Windows, but it
should also work on any system supported by the SDL library on the i386
architecture.

The port was created by disassembling the original DOS executable and replacing
the DOS-specific parts with platform-independent code, using [SDL](https://www.libsdl.org/)
for input and video display, [WildMIDI](https://github.com/Mindwerks/wildmidi) for music and
[OpenAL](https://www.openal.org/) for sound.

An executable and some, but not all, neccessary data files are provided with this
port. This means that some of game data are not included. To install the
**SyndicatFX**, you will need to have the original data from either
*Syndicate Plus CD*, or digital distribution.

## Community

If you wish to discuss the **SyndicatFX** or if you have any problems,
you may post to [discussion page of the project repo](https://github.com/swfans/syndicatfx/discussions)
or join [Syndicate discord channel](https://discord.gg/cnHmWqe49t).

## Installation

To install **SyndicatFX**, you will need either the original *Syndicate
Plus PC CD*, or a *Synd Plus CD image* from digital distribution
of the game. There were several physical versions released. It is uncertain
whether this port will work with all releases, so choose the most popular,
english version. Floppy disk release was not tested.

The CD version which you use narrows the range of available languages.

* Multi-lingual CD contains `eng` `fre` `ita`,
* German release is required to use `ger`,

Currently, only multi-lingual release is fully supported, as the translation
is built into the executable.

The steps you need to take vary depending on your operating system.

### Installing on GNU or UNIX

If you're installing on GNU or UNIX, you will have to compile from source,
and then add the neccessary data files. The steps are:

1. Insert the *Syndicate Plus PC CD* and make sure it is mounted, or mount a *Synd Plus CD image*
   (e.g. in `/media/cdrom`).
2. Choose an installation folder, like `/usr/local/share/syndicatfx` (this is the default).
3. Do `util/install -f <SOURCE> -t <TARGET> -l <LANG>`, where
   * *\<SOURCE\>* is the game CD mount path, like `/media/cdrom`,
   * *\<TARGET\>* is the destination path, meaning the installation folder you've chosen,
   * *\<LANG\>* is the installation language, and can be one of: `eng` `fre` `ita`.

   The install script will copy files from the CD or mounted CD image, but also
   will download additional data files from their repositories:
   [gfx](https://github.com/swfans/syndicatfx-gfx/releases) and
   [sfx](https://github.com/swfans/syndicatfx-sfx/releases).
4. Now we finally get to this code repository. Download and unpack the source
   tarball or clone git repo. You can also try downloading the prebuilt *patch*
   release; but that will only  work if your Linux environment happens to be
   compatible to the one where the release patch package was built.
   Depending on what you've chosen, follow further steps:

Route "a" - compiling the game by yourself.

7. a. Follow [building instructions](#general-building-instructions) below to get
     a compiled executable and configuration files.
8. a. After the build succeeded, do `make prefix=<TARGET> install` in the
     directory where build commands were executed, to copy built files into the
     *\<TARGET\>/syndicatfx* installation folder you've chosen. If you've built the
     binary yourself, you already have all the tools neccessary for that install
     command to work. After installation, verify the folder to make sure you
     didn't installed these files to `syndicatfx/syndicatfx/` directory; you can just
     move them one dir higher if that's the case.

Route "b" - using the prebuilt *patch* release.

7. b. Extract the downloaded patch file.
8. b. Move all extracted files and folders to the selected installation folder.
     Place them all in one folder, so that sub-olders `conf`, `data`
     and the executable file `syndicatfx` are all next to each other.

If all went well, you can now [start the game](#starting-the-game).

### Installing on Mac OS X

There is support for creating Mac OS X bundles. While you can just install
the game into the UNIX directory structure, using a bundle is a cleaner solution.

#### Making a Mac OS app bundle

If you build an application bundle, you can install the game by just drag-and-drop
into your *Applications* directory.

You can create the bundle by following these instructions:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#building-on-mac-os-x) below to get
   a compiled executable
3. after the code is compiled, install the data, as explained in the
   [GNU or UNIX](#installing-on-gnu-or-unix) section, to any directory,
   say `./data` (in case you wonder, the CD drive should be mounted under
   `/Volumes/SYNDPLUS`)
4. finally, run `util/mkbundle ./data` (replacing `./data` with the directory
   you installed the data to); this will produce an application bundle named
   `SyndicatFX.app` in the current directory
5. if you want there to be a pretty icon for the bundle, copy `res/syndicatfx.icns`
   to `SyndicatFX.app/Contents/Resources`

### Installing on Windows

There are two ways to install **SyndicatFX** on Windows - either download
the installer, or download the source code and compile it by hand.
It is recommended to use the installer!

The installer requires either the original *Syndicate Plus PC CD* to be present in
the CD-ROM drive, or a *Synd Plus CD image* file on disk. The CD image can come
from any popular digital distribution platform. The installer will copy the files
from the original *Syndicate Plus CD* or *Synd Plus CD image* and download and
install additional data specific to the port.

The *Syndicate Plus PC CD* or *Synd Plus CD image* is not required to play the game,
it is used only during the installation process.

If you've decided on the hand-compilation option, proceed with the following steps:

1. Insert the *Syndicate Plus PC CD* and make sure it is mounted, or mount a
   *Synd Plus CD image* (so that you get drive letter or path, ie. `e:`).
2. Choose an installation folder which ends with project name, like
   `c:\Program Files\syndicatfx`.
3. Use `util/install` script to both copy required files from CD and download
   additional data, and place them all into your chosen installation folder.
   If you have UNIX or GNU support on your Windows (ie. Linux Windows Subsystem,
   or MINGW, or MSYS2, or Cygwin), you can just run the script in `bash` - see
   [Installing on GNU or UNIX](installing-on-gnu-or-unix) for details.
   If you don't have that, you will have to analyze said script to figure out
   what should be copied where - detailed instructions are not part of the
   documentation.
5. Now we finally get to this code repository. Download and unpack the source
   tarball or clone git repo. Or you can instead download the prebuilt *patch*
   release. Depending on what you've chosen, follow further steps:

Route "a" - compiling the game by yourself.

6. a. Follow [building instructions](#build-examples-on-windows) below to get
     a compiled executable and configuration files.
7. a. After the build succeeded, do `make prefix=<TARGET> install` in the
     directory where build commands were executed, to copy built files into the
     *\<TARGET\>/syndicatfx* installation folder you've chosen. If you've built the
     binary yourself, you already have all the tools neccessary for that install
     command to work. After installation, verify the folder to make sure you
     didn't installed these files to `syndicatfx/syndicatfx/` directory; you can just
     move them one dir higher if that's the case.

Route "b" - using the prebuilt *patch* release.

6. b. Extract the downloaded patch file.
7. b. Move all extracted files and folders to the selected installation folder.
     Place them all in one folder, so that sub-olders `conf`, `data`
     and the executable file `syndicatfx.exe` are all next to each other.

You can now [start the game](#starting-the-game).

## Starting the game

The original game needed to be started with parameters to play normally. With
SyndicatFX, the parameters can still be used, and are required for language
selection. The executable can be run with
`--help` parameter to show a short description of all parameters.

Parameter `-c` (used in the original game) is required for language selection.

The typical command line for running SyndicatFX on bash-like shell is:

```
./syndicatfx -c 0
```

If running on Windows, the command line above should work as well. Though more
Windows-centric way of doing that is:


```
syndicatfx.exe /c 0
```

The game will start witout intro animation, and present you with a menu where
you can start playing.

In case something doesn't work, check `error.log` file generated while the game
was starting.

## Building

Note that you can oly build the port for x86 architecture, and you either need
32-bit OS, or 64-bit OS with 32-bit user space libraries available.

### General building instructions

To build **SyndicatFX**, you will need the following:

* GNU Autotools
* GNU C compiler
* Python 3 (with module: polib)
* development versions of the following libraries:
  * SDL2 or SDL1.2
  * OpenAL (we recommend OpenAL Soft)
  * libvorbis
  * libogg
  * libpng
  * zlib
  * WildMIDI

Once you've made sure you have the above, proceed with the following steps:

1. go into the directory with `syndicatfx` source release (containing `conf`, `doc`, `src` etc.)
2. do `autoreconf -if` to create build scripts from templates
3. do `./configure` to make the build scripts find required toolchain and libraries
4. do `make` to compile the executable file

You should now have a working `src/syndicatfx` executable file.

### Build examples on Linux

#### Build on Ubuntu 20.04 64-bit

Here are specific commands required to compile the executable on Ubuntu linux.

First, dependencies have to be installed. But this project can be built only
for 32-bit address space - some installed packages have to be compatible with
it. To even allow installing packages for a different architecture, it needs
to be added:

```
dpkg --add-architecture i386
```

Now install the dependencies - remember that some must be 32-bit (i386):

```
sudo apt install gcc-multilib g++-multilib lib32z1
sudo apt install python3 python3-polib
sudo apt install libsdl2-dev:i386
sudo apt install libopenal-dev:i386
sudo apt install libvorbis-dev:i386 libvorbisfile3:i386
sudo apt install libogg-dev:i386
sudo apt install libwildmidi-dev:i386
```

Be warned - your package manager may assume you want to replace the architecture
if you did not explicitly added it. If the information on screen suggests
that the installation would remove a group of currently installed packages,
do not proceed with the changes and find another way.

Now as our host is ready, we can start working on the actual `syndicatfx` sources.
Go to that folder, and generate build scripts from templates using autotools:

```
autoreconf -ivf
```

Next, proceed with the build steps; we will do that in a separate folder.

```
mkdir -p release; cd release
PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig" CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" ../configure --enable-lbs-structs-expand
make V=1
```

Providing `PKG_CONFIG_PATH` allows us to tell `pkg-config` command where to
search for 32-bit packages (it would use a path to 64-bit ones by default).
The `V=1` variable makes `make` print each command it executes, which makes
diagnosing issues easier.

In case you also want a debug build:

```
mkdir -p debug; cd debug
PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig" CPPFLAGS="-DDEBUG -D__DEBUG" CFLAGS="-m32 -g -O0 -Wall" CXXFLAGS="-m32 -g -O0 -Wall" LDFLAGS="-m32 -g -O0 -Wall" ../configure --enable-lbs-structs-expand
make V=1
```

Explanation of the parameters:

* The `-g -O0` flags make it easier to use a debugger like _GDB_ with the
  binary, by storing symbols and disabling code optimizations.
* The `-Wall` flags enable displaying more warnings during compilation.
* The `-DDEBUG -D__DEBUG` defines make the binary store more information in
  `error.log` file. First one enables verbose logging from application code,
  second does the same for libraries used.
* The flags are set separately for C preprocessor (`CPP`), compilers (`C`, `CXX`)
  and linker (`LD`). See [GNU Automake documentation](https://www.gnu.org/software/automake/manual/html_node/Programs.html)
  for details on that.

Some tests to verify the build are included in the project. To execute them:

```
make V=1 check
```

### Build examples on Windows

#### Build on Windows with MSYS2 updated 2023-01

Using Minimal System and the MinGW toolchain available within, it is possible
to build the executable using the same way as for UNIX systems, with bash and autotools.

First install the dependencies - mingw32, since we need 32-bit toolchain:

```
pacman -S mingw-w64-i686-binutils mingw-w64-i686-autotools mingw-w64-i686-pkgconf mingw-w64-i686-make mingw-w64-i686-gcc
pacman -S mingw-w64-i686-libpng
pacman -S mingw-w64-i686-SDL2
pacman -S mingw-w64-i686-openal
pacman -S mingw-w64-i686-libvorbis
pacman -S mingw-w64-i686-libogg
```

The WildMIDI library is not available as MSYS2 pacman package, install manually:

```
pacman -S unzip
wget https://github.com/Mindwerks/wildmidi/releases/download/wildmidi-0.4.5/wildmidi-0.4.5-win32.zip
unzip wildmidi-0.4.5-win32.zip
cp wildmidi-0.4.5-win32/*.h /mingw32/include/
cp wildmidi-0.4.5-win32/*.a /mingw32/lib/
cp wildmidi-0.4.5-win32/*.dll /mingw32/bin/
```

The Python interpeter needs to have an additional module installed:

```
pip install polib
```

Make sure the modules are installed on the Python version used during build - with MSYS2,
you often get separate `i686` and `x86_64` versions of Python. For both of them, you
can separately install `pip` package, and then add any missing modules through it.
To select specific `python3` or `pip` at command line, alter the `PATH` variable,
like in some commands in later part of the instructions.

If you do not have any Python, then install the one which fits other `i686` tools:

```
pacman -S mingw-w64-i686-python3 mingw-w64-i686-python-pip
```

Now as our host is ready, we can start working on the actual `syndicatfx` sources.
We will still have to provide paths to 32-bit configuration - MSYS will prefer
folders with data for 64-bit building.
Go to the `syndicatfx` folder, and generate build scripts from templates using autotools:

```
autoreconf -ivf --include=/mingw32/share/aclocal/
```

Next, proceed with the build steps; we will do that in a separate folder.
Note how we are modifying PATH and PKG_CONFIG_PATH environment variables to make shell
search for mingw32 binaries and pkg-config search for mingw32 packages before
the default mingw64 ones:

```
mkdir -p release; cd release
PATH="/mingw32/bin:$PATH" PKG_CONFIG_PATH="/mingw32/lib/pkgconfig" CFLAGS="-m32" CXXFLAGS="-m32" LDFLAGS="-m32" ../configure --prefix=/mingw32 --enable-lbs-structs-expand
PATH="/mingw32/bin:$PATH" make V=1
```

Depending on versions of packages, your `make` executable may be also called `mingw32-make`.
Also, the build system may expect to find `python` binary within `mingw32` folder,
failing to find it if you have it somewhere else. In both cases, just creating a symbolic
link with name expected by build system will fix the issue.

### Build examples on Mac OS

#### Build on Mac OS X

Mac OS X is at its core a UNIX system. To build the **SyndicatFX** it is
enough to follow the [general building instructions](#general-building-instructions).

The GCC compiler for Mac OS X comes as part of XCode, which you can get from
Apple's web site. You can also make your life a lot easier by using
[Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/)
to install the required packages for you, instead of having to download and
compile them manually.

If you have Mac OS 10.6 ("Snow Leopard") or newer, you will need to make sure
that your libraries contain code for the i386 architecture. In MacPorts this
is achieved by using the `+universal` variant (e.g. `port -vu install libsdl2 +universal`).

When you configure your compilation toolchain and location of libraries,
you will need to pass `CFLAGS="-arch i386"`, like so:

```
./configure --enable-lbs-structs-expand CFLAGS="-arch i386"
```

If you are planning to create [Mac OS app bundle](#making-a-mac-os-app-bundle),
you will also need to pass `data-path`, so your final command will be:


```
./configure --with-data-path="SyndicatFX.app/Contents/Resources" --enable-lbs-structs-expand CFLAGS="-arch i386"
```

Then, do `make` as the [general building instructions](#general-building-instructions) tell.

## Done

That's all. Have a nice play!
