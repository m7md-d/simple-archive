#include <stdio.h>
#include "flags.h"
#include "arc.h"

#define C_RESET     "\033[0m"
#define C_BOLD      "\033[1m"
#define C_RED       "\033[1;31m"
#define C_GREEN     "\033[1;32m"
#define C_YELLOW    "\033[1;33m"
#define C_CYAN      "\033[1;36m"
#define C_MAGENTA   "\033[1;35m"
#define C_WHITE     "\033[1;37m"
#define C_GRAY      "\033[0;90m"


/* print usage information listing all options */
void print_help(void)
{
printf("\n");
printf(C_MAGENTA "   █████╗ ██████╗  ██████╗██╗  ██╗ ██████╗ ███╗   ██╗\n");
printf("  ██╔══██╗██╔══██╗██╔════╝██║  ██║██╔═══██╗████╗  ██║\n");
printf("  ███████║██████╔╝██║     ███████║██║   ██║██╔██╗ ██║\n");
printf("  ██╔══██║██╔══██╗██║     ██╔══██║██║   ██║██║╚██╗██║\n");
printf("  ██║  ██║██║  ██║╚██████╗██║  ██║╚██████╔╝██║ ╚████║\n");
printf("  ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═══╝" C_RESET "\n");
printf("\n");
printf(C_WHITE "  Usage: " C_CYAN "archon " C_YELLOW "<option> " C_GREEN "<archive.pa> " C_GRAY "[file]\n\n" C_RESET);
printf(C_BOLD C_WHITE "  Options:\n" C_RESET);
printf("    " C_YELLOW "-c" C_RESET " <archive.pa>                      " C_GRAY "Create a new archive\n" C_RESET);
printf("    " C_YELLOW "-i" C_RESET " <archive.pa> <file>               " C_GRAY "Insert file into archive\n" C_RESET);
printf("    " C_YELLOW "-d" C_RESET " <archive.pa> <file>               " C_GRAY "Delete file from archive\n" C_RESET);
printf("    " C_YELLOW "-e" C_RESET " <archive.pa> <file>               " C_GRAY "Extract file from archive\n" C_RESET);
printf("    " C_YELLOW "-l" C_RESET " <archive.pa>                      " C_GRAY "List all files in archive\n" C_RESET);
printf("    " C_YELLOW "-n" C_RESET " <archive.pa>                      " C_GRAY "Number of files in archive\n" C_RESET);
printf("    " C_YELLOW "-r" C_RESET " <archive.pa> <old> <new>          " C_GRAY "Replace file in archive\n" C_RESET);
printf("    " C_YELLOW "-h" C_RESET "                                   " C_GRAY "Show this help\n\n" C_RESET);
printf(C_BOLD C_WHITE "  Examples:\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-c " C_GREEN "myarchive.pa\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-i " C_GREEN "myarchive.pa " C_WHITE "hello.txt\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-n " C_GREEN "myarchive.pa\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-l " C_GREEN "myarchive.pa\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-e " C_GREEN "myarchive.pa " C_WHITE "hello.txt\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-d " C_GREEN "myarchive.pa " C_WHITE "hello.txt\n" C_RESET);
printf("    " C_CYAN "archon " C_YELLOW "-r " C_GREEN "myarchive.pa " C_WHITE "old.txt new.txt\n\n" C_RESET);
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
        number_files(argv[2]);
        break;

    case REPLACE:
        if (argc < 3) {
            fprintf(stderr, "usage: <%s> -r <archive> <old_file> <new_file>\n", *argv);
            return;
        }
        replace_file(argv[2], argv[3], argv[4]);
        break;

    default:
        fprintf(stderr, "unknown flag: %s\n", argv[1]);
        print_help();
        break;
    }
}
