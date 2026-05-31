#ifndef FLAGS_H
#define FLAGS_H

typedef enum
{
    HELP = 'h',
    CREATE = 'c',
    INSERT = 'i',
    DELETE = 'd',
    EXTRACT = 'e',
    LIST = 'l'
} flag;

void flag_check(int, char **);

#endif /* FLAGS_H */
