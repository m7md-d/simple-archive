# Installation

## Requirements

- A C99-compatible compiler (`gcc`, `clang`, `cc`)
- `make` (optional)

## Build

```sh
gcc -Wall -Wextra -std=c99 src/*.c -Iinclude -o archon
```

## Install

Copy the binary to a directory in your `PATH`:

```sh
cp archon /usr/local/bin/
```

## Verify

```sh
$ ./archon -h
usage: archon [-c|-i|-d|-e|-l|-h] <archive> [file]
  ...
```
