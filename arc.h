#ifndef ARC_H
#define ARC_H
#include <stdlib.h>
typedef unsigned int uint;
typedef unsigned char uchar;

typedef struct file
{
    char * name;
    size_t size;
    uchar *data;
} file;

typedef struct arc
{
    char magic_n[8];
    uint ver;
    uint file_count;
    file *files;
} arc;


void creat_arc(char *arc_name);                      /* -c */
void insert_file(char *arc_name, char *filename);    /* -i */
void delete_file(char *arc_name, char *filename);    /* -d */
void extract_file(char *arc_name, char *filename);   /* -x */
void list_files(char *arc_name);                     /* -l */

#endif /* ARC_H */
