# Archive format

Archon uses a custom binary format identified by the `.pa` extension.

## Header (12 bytes)

| Offset | Size | Type     | Field       |
|--------|------|----------|-------------|
| 0      | 6    | `char[]` | Magic       |
| 6      | 1    | `uint8`  | Version     |
| 7      | 1    | _pad_    | —           |
| 8      | 4    | `uint32` | File count  |

- Magic must be the ASCII bytes `ARCHON`.
- Version is currently `1`.
- The byte at offset 7 is padding (aligns `file_count` to 4 bytes).
- `file_count` is the number of files stored (max 256).

## Metadata entries (272 bytes each)

Immediately after the header comes one entry per file:

| Offset | Size | Type       | Field     |
|--------|------|------------|-----------|
| 0      | 256  | `char[]`   | File name |
| 256    | 8    | `uint64`   | File size |
| 264    | 8    | `uint64`   | Data offset |

- File name is null-padded; unused bytes are zero.
- `size` is the exact byte count of the file data.
- `offset` is the absolute byte offset from the start of the archive file to the file data.

## File data

The raw bytes of each file are stored consecutively at the offsets declared in the metadata.

```
+------------------+
| Header (12 B)    |
+------------------+
| Meta entry 0     |
| Meta entry 1     |
| ...              |
+------------------+
| File data 0      |
| File data 1      |
| ...              |
+------------------+
```

## Limits

- Maximum file count: **256**
- Maximum file name length: **255** bytes (256 with null terminator)
- Archive total: no hard limit (bounded by available memory and disk)
