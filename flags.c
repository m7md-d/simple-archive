#include <stdio.h>
#include "flags.h"
#include "arc.h"


void print_help(void)
{}

void flag_check(int argc, char **argv)
{
    flag f = argv[1][1];

    switch (f)
    {
    case HELP:
        print_help();
        break;
    case CREATE:
        creat_arc(argv[2]);
        break;
    case INSERT:
        insert_file(argv[2], argv[3]);
        break;
    case DELETE:
        delete_file(argv[2], argv[3]);
        break;
    case EXTRACT:
        extract_file(argv[2], argv[3]);
        break;
    case LIST:
        list_files(argv[2]);
        break;
    
    default:
        break;
    }
}
