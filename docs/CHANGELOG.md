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

## [1.1.0] — 2026-06-02

### Added

- `-n` print the number of files in the archive
- `-r` replace a file inside the archive with a new one

## [1.2.0] — 2026-06-03

### Changed

- `print_help()` now displays an `ARCHON` ASCII banner with ANSI color
- Usage output uses color-coded sections (commands, archive, file, examples)
