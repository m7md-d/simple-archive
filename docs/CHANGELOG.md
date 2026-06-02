# Changelog

## [1.0.0] — 2026-06-02

### Added

- `-c` create a new empty archive
- `-i` insert a file into the archive
- `-d` delete a file from the archive
- `-e` extract a file from the archive
- `-l` list all files in the archive
- `-h` show help
- Binary `.pa` archive format with magic `"ARCHON"`, versioning, and per-file metadata
- Atomic writes via temporary-file + rename
- Duplicate detection on insert
- Missing-file error handling on delete/extract
