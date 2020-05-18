# 10shell

System calls: open, read, close, opendir, closedir
Library functions: string.h,

Single-threaded

## Compiling

```bash
cc list.c -lreadline -o list
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

## Example

hap6362.log
hap1299874.log

filename lineno[-lineno] inclusive, index starts at one
max 10 lines (?)

Enter to cycle through files
Type filename and enter to preview file
Type directory and enter to cd into directory
Crrl-D to quit

```bash
>>> hap.log 1-5
```

hdjd, fjdjdkso, jejsk
hdjd, fjdjdk, jejsk
hd, fgisb, jejs

## Non-features

* No writing for now
* Sort files

## Use cases

* 20GB of data in a directory
