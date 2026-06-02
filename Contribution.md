# Contributing

## Code style

- 4-space indentation, no tabs.
- Opening brace on its own line for functions, structs, and switch bodies.
- Opening brace on the same line for `if`, `for`, `while`.
- Space between `if`/`for`/`while` and the opening parenthesis.
- Return type on the same line as the function name.
- Pointer `*` attached to the name (`char *name`), not the type.

## Adding a new flag

1. Add the flag character to the `flag` enum in `flags.h`.
2. Add a case in `flag_check()` in `flags.c`.
3. Add the corresponding function declaration in `arc.h`.
4. Implement the function body in `arc.c`.
5. Update `print_help()` and `README.md`.

## Archive compatibility

The on-disk format is defined in `docs/ARCHIVE_FORMAT.md`. Any change to the format must increment the version field in the header so that older builds can reject incompatible archives.

## Before submitting

- Build with `-Wall -Wextra -std=c99` — zero warnings required.
- Test with the existing integration pattern (create → insert → list → extract → delete).
