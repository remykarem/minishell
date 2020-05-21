# minishell

System calls: open, read, close, opendir, closedir
Library functions: string.h,

Single-threaded

## Compiling

```bash
cc preview.c -lreadline -o preview.exe
```

## Usage

```bash
./preview.exe /path/to/directory/
```

## Features

* To be able to open the directory with negligible latency
* To be able to view a list of some files
    * Cycle through files***
    * Directories are in different colour
* To be able to read some contents of a file at a time
* Text-based files only (.txt, .csv, .log)

## Semi-features

* cd in this shell
* less
* To be able to copy files (?)



## Non-features

* No writing for now
* Sort files

## Use cases

* 20GB of data in a directory
