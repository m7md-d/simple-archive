#ifndef ARC_H
#define ARC_H
#include <stdlib.h>
typedef unsigned int uint;
typedef unsigned char uchar;

/* an in-memory representation of a single archived file */
typedef struct file
{
    char * name;
    size_t size;
    uchar *data;
} file;

/* an in-memory representation of the whole archive */
typedef struct arc
{
    char magic_n[8];
    uint ver;
    uint file_count;
    file *files;
} arc;


/* create a new empty archive at arc_name */
void creat_arc(char *arc_name);                                     /* -c */


/* insert a copy of filename into the archive */
void insert_file(char *arc_name, char *filename);                   /* -i */


/* delete the entry named filename from the archive */
void delete_file(char *arc_name, char *filename);                   /* -d */


/* extract the entry named filename to disk */
void extract_file(char *arc_name, char *filename);                  /* -x */


/* print a listing of every file in the archive */
void list_files(char *arc_name);                                    /* -l */


/* print the number of files in the archive */
void number_files(char *arc_name);                                  /* -n */


/* replace the entry named old_file with a copy of new_file */
void replace_file(char *arc_name, char *old_file, char *new_file);  /* -r */

#endif /* ARC_H */
