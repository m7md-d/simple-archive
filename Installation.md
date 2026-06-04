# Installation

## Requirements

- A C99-compatible compiler (`gcc`, `clang`, `cc`)
- A standard C library (no external dependencies)

## Build

Compile all sources in `src/` against the headers in `include/`:

```sh
gcc -Wall -Wextra -std=c99 src/*.c -Iinclude -o archon
```

This produces an executable named `archon` in the project root. The build uses three source files (`src/main.c`, `src/flags.c`, `src/arc.c`) and three headers (`include/main.h`, `include/flags.h`, `include/arc.h`).

## Install

Optionally copy the binary to a directory in your `PATH`:

```sh
cp archon /usr/local/bin/
```

## Verify

Run the help banner to confirm the build is working:

```sh
$ ./archon -h
   █████╗ ██████╗  ██████╗██╗  ██╗ ██████╗ ███╗   ██╗
  ██╔══██╗██╔══██╗██╔════╝██║  ██║██╔═══██╗████╗  ██║
  ███████║██████╔╝██║     ███████║██║   ██║██╔██╗ ██║
  ██╔══██║██╔══██╗██║     ██╔══██║██║   ██║██║╚██╗██║
  ██║  ██║██║  ██║╚██████╗██║  ██║╚██████╔╝██║ ╚████║
  ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═══╝

  Usage: archon <option> <archive.pa> [file]

  Options:
    -c <archive.pa>             Create a new archive
    -i <archive.pa> <file>      Insert file into archive
    -d <archive.pa> <file>      Delete file from archive
    -x <archive.pa> <file>      Extract file from archive
    -l <archive.pa>             List all files in archive
    -n <archive.pa>             Number of files in archive
    -r <archive.pa> <old> <new> Replace file in archive
    -h                          Show this help
```
