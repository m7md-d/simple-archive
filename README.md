# Simple-Archive

A C program that packs files into a custom archive format (`.pa`) and supports creation, insertion, deletion, extraction, and listing.

## Compilation

```sh
gcc -Wall -Wextra -std=c99 -o archon main.c flags.c arc.c
```

## Usage

```
./archon -c <archive.pa>
./archon -i <archive.pa> <file>
./archon -d <archive.pa> <file>
./archon -e <archive.pa> <file>
./archon -l <archive.pa>
./archon -h
```

- `-c <archive.pa>` — create a new empty archive
- `-i <archive.pa> <file>` — insert a file into the archive
- `-d <archive.pa> <file>` — delete a file from the archive
- `-e <archive.pa> <file>` — extract a file from the archive
- `-l <archive.pa>` — list all files in the archive
- `-h` — show help

## Archive format

A `.pa` archive is a binary file with the following layout:

| Offset | Size | Field |
|--------|------|-------|
| 0      | 6    | Magic bytes `ARCHON` |
| 6      | 1    | Format version (`1`) |
| 7      | 4    | Number of files (uint32) |
| 12     | —    | File metadata entries (272 bytes each) |
| —      | —    | Raw file data |

Each metadata entry consists of:

| Offset | Size | Field |
|--------|------|-------|
| 0      | 256  | File name (null-padded) |
| 256    | 8    | File size in bytes (uint64) |
| 264    | 8    | Offset to file data (uint64) |

The maximum number of files per archive is **256**.

## How it works

1. The archive uses the magic string `"ARCHON"` to identify valid files.
2. On creation, only the header is written (`file_count = 0`).
3. All metadata is stored at the start of the archive; file data follows at the offsets recorded in each entry.
4. Insert reads the entire archive into memory, appends the new file, and rewrites the archive atomically via a temporary file.
5. Delete removes the entry and shifts remaining data; extraction writes the file data to disk.
6. List reads only the header and metadata (no file data needed).

## Limitations

- Maximum files: **256**
- File names are truncated to **255 characters**.
- The entire archive is loaded into memory for insert/delete operations.

## Examples

### Create

```sh
$ ./archon -c myarchive.pa
created archive: myarchive.pa
```

### Insert

```sh
$ echo "hello world" > hello.txt
$ ./archon -i myarchive.pa hello.txt
inserted: hello.txt (12 bytes)
```

### List

```sh
$ ./archon -l myarchive.pa

  archive: myarchive.pa
  version: 1
  files:   1

  No.   Name                                      Bytes
  ---   ----                                      -----
  1     hello.txt                                  12
```

### Extract

```sh
$ ./archon -e myarchive.pa hello.txt
extracted: hello.txt (12 bytes)
$ cat hello.txt
hello world
```

### Delete

```sh
$ ./archon -d myarchive.pa hello.txt
deleted: hello.txt
```

### Error handling

```sh
$ ./archon -c myarchive.pa
Error: 'myarchive.pa' already exists
```
