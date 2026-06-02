#ifndef FLAGS_H
#define FLAGS_H

/* command-line flags mapped to their single-character options */
typedef enum
{
    HELP = 'h',
    CREATE = 'c',
    INSERT = 'i',
    DELETE = 'd',
    EXTRACT = 'e',
    LIST = 'l'
} flag;

/* parse argv and dispatch to the matching archive operation */
void flag_check(int, char **);

#endif /* FLAGS_H */
