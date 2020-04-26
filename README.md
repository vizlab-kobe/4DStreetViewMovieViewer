# 4DStreetViewMovieViewer
4DStreetViewMovieViewer is a viewer application for 4D Street View.

## Installation

### Prerequisite

* [KVS](https://github.com/naohisas/KVS.git)
* [Qt](https://www.qt.io)

### Build
Clone 4DStreetViewMovieViewer repository from Github as follows:

```bash
$ git clone https://github.com/vizlab-kobe/4DStreetViewMovieViewer.git
```

#### Lib
Build 4DStreetViewMovieViewer library required to compile each application in App and Test.

```bash
$ cd 4DStreetViewMovieViewer
$ cd Lib
$ ./kvsmake.py
```

#### Test
Some of the test programs are in the Test directory. All of these programs can be built using the kvsmake command in each test program directory (XXX).

```bash
$ cd Test
$ cd XXX
$ kvsmake -G
$ kvsmake
```

#### App
The App directory contains several application programs. To build and run these application programs, refer to the ReadMe files in each directories.

### Publications
1. xxx
1. xxx
