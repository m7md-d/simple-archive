#include <stdio.h>
#include "flags.h"
#include "arc.h"


/* print usage information listing all options */
void print_help(void)
{
    printf("usage: archon [-c|-i|-d|-e|-l|-n|-r|-h] <archive> [file]\n\n");
    printf("  -c <archive>                         create archive\n");
    printf("  -i <archive> <file>                  insert file\n");
    printf("  -d <archive> <file>                  delete file\n");
    printf("  -x <archive> <file>                  extract file\n");
    printf("  -l <archive>                         list contents\n");
    printf("  -n <archive>                         number of files\n");
    printf("  -r <archive> <old_file> <new_file>   replace file\n");
    printf("  -h                                   this help\n");
}

/* parse command-line flags and dispatch to the matching arc operation */
void flag_check(int argc, char **argv)
{
    if (argc < 2) {
        print_help();
        return;
    }

    flag f = argv[1][1];

    switch (f)
    {
    case HELP:
        print_help();
        break;

    case CREATE:
        if (argc < 3) {
            fprintf(stderr, "usage: <%s> -c <archive>\n", *argv);
            return;
        }
        creat_arc(argv[2]);
        break;

    case INSERT:
        if (argc < 4) {
            fprintf(stderr, "usage: <%s> -i <archive> <file>\n", *argv);
            return;
        }
        insert_file(argv[2], argv[3]);
        break;

    case DELETE:
        if (argc < 4) {
            fprintf(stderr, "usage: <%s> -d <archive> <file>\n", *argv);
            return;
        }
        delete_file(argv[2], argv[3]);
        break;

    case EXTRACT:
        if (argc < 4) {
            fprintf(stderr, "usage: <%s> -e <archive> <file>\n", *argv);
            return;
        }
        extract_file(argv[2], argv[3]);
        break;

    case LIST:
        if (argc < 3) {
            fprintf(stderr, "usage: <%s> -l <archive>\n", *argv);
            return;
        }
        list_files(argv[2]);
        break;

    case NFILES:
        if (argc < 3) {
            fprintf(stderr, "usage: <%s> -n <archive>\n", *argv);
            return;
        }
        list_files(argv[2]);
        break;

    case REPLACE:
        if (argc < 3) {
            fprintf(stderr, "usage: <%s> -r <archive> <old_file> <new_file>\n", *argv);
            return;
        }
        list_files(argv[2]);
        break;

    default:
        fprintf(stderr, "unknown flag: %s\n", argv[1]);
        print_help();
        break;
    }
}
