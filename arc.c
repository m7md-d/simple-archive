#include "arc.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define DISK_MAGIC      "ARCHON"
#define DISK_MAGIC_LEN  6
#define DISK_MAX_FILES  256
#define DISK_MAX_NAME   256
#define DISK_VERSION    1

typedef struct
{
    char     magic[DISK_MAGIC_LEN];
    uint8_t  version;
    uint32_t file_count;
} DiskHeader;

typedef struct
{
    char     name[DISK_MAX_NAME];
    uint64_t size;
    uint64_t offset;
} DiskMeta;

static void free_arc(arc *a)
{
    if (a->files) {
        for (uint i = 0; i < a->file_count; i++) {
            free(a->files[i].name);
            free(a->files[i].data);
        }
        free(a->files);
        a->files = NULL;
    }
    a->file_count = 0;
}

static int read_arc(char *path, arc *a)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s'\n", path);
        return 0;
    }

    DiskHeader h;
    if (fread(&h, sizeof(DiskHeader), 1, fp) != 1) {
        fprintf(stderr, "Error: invalid archive\n");
        fclose(fp);
        return 0;
    }

    if (memcmp(h.magic, DISK_MAGIC, DISK_MAGIC_LEN) != 0) {
        fprintf(stderr, "Error: not a valid archive\n");
        fclose(fp);
        return 0;
    }

    if (h.file_count > DISK_MAX_FILES) {
        fprintf(stderr, "Error: too many files\n");
        fclose(fp);
        return 0;
    }

    memset(a->magic_n, 0, sizeof(a->magic_n));
    memcpy(a->magic_n, h.magic, DISK_MAGIC_LEN);
    a->ver        = h.version;
    a->file_count = h.file_count;
    a->files      = NULL;

    if (a->file_count == 0) {
        fclose(fp);
        return 1;
    }

    DiskMeta *dm = malloc(sizeof(DiskMeta) * a->file_count);
    if (!dm) {
        fprintf(stderr, "Error: out of memory\n");
        fclose(fp);
        return 0;
    }

    if (fread(dm, sizeof(DiskMeta), a->file_count, fp) != a->file_count) {
        fprintf(stderr, "Error: corrupt archive\n");
        free(dm);
        fclose(fp);
        return 0;
    }

    a->files = malloc(sizeof(file) * a->file_count);
    if (!a->files) {
        fprintf(stderr, "Error: out of memory\n");
        free(dm);
        fclose(fp);
        return 0;
    }

    for (uint i = 0; i < a->file_count; i++) {
        a->files[i].name = strdup(dm[i].name);
        a->files[i].size = (size_t)dm[i].size;
        a->files[i].data = malloc(a->files[i].size);
        if (!a->files[i].data) {
            fprintf(stderr, "Error: out of memory\n");
            for (uint j = 0; j < i; j++) {
                free(a->files[j].name);
                free(a->files[j].data);
            }
            free(a->files);
            free(dm);
            fclose(fp);
            return 0;
        }
        fseek(fp, (long)dm[i].offset, SEEK_SET);
        if (fread(a->files[i].data, 1, a->files[i].size, fp) != a->files[i].size) {
            fprintf(stderr, "Error: corrupt archive\n");
            for (uint j = 0; j <= i; j++) {
                free(a->files[j].name);
                free(a->files[j].data);
            }
            free(a->files);
            free(dm);
            fclose(fp);
            return 0;
        }
    }

    free(dm);
    fclose(fp);
    return 1;
}

static int write_arc(char *path, arc *a)
{
    char tmp[512];
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    FILE *fp = fopen(tmp, "wb");
    if (!fp) {
        fprintf(stderr, "Error: cannot write\n");
        return 0;
    }

    uint64_t off = sizeof(DiskHeader) + a->file_count * sizeof(DiskMeta);

    DiskMeta *dm = NULL;
    if (a->file_count > 0) {
        dm = malloc(sizeof(DiskMeta) * a->file_count);
        if (!dm) {
            fprintf(stderr, "Error: out of memory\n");
            fclose(fp);
            return 0;
        }
        for (uint i = 0; i < a->file_count; i++) {
            memset(dm[i].name, 0, DISK_MAX_NAME);
            strncpy(dm[i].name, a->files[i].name, DISK_MAX_NAME - 1);
            dm[i].size   = (uint64_t)a->files[i].size;
            dm[i].offset = off;
            off += a->files[i].size;
        }
    }

    DiskHeader h;
    memcpy(h.magic, DISK_MAGIC, DISK_MAGIC_LEN);
    h.version    = DISK_VERSION;
    h.file_count = a->file_count;

    fwrite(&h, sizeof(DiskHeader), 1, fp);
    if (a->file_count > 0)
        fwrite(dm, sizeof(DiskMeta), a->file_count, fp);

    for (uint i = 0; i < a->file_count; i++)
        fwrite(a->files[i].data, 1, a->files[i].size, fp);

    fclose(fp);
    free(dm);

    remove(path);
    if (rename(tmp, path) != 0) {
        fprintf(stderr, "Error: cannot finalize\n");
        remove(tmp);
        return 0;
    }
    return 1;
}

void creat_arc(char *name)
{
    FILE *fp = fopen(name, "rb");
    if (fp) {
        fclose(fp);
        fprintf(stderr, "Error: '%s' already exists\n", name);
        return;
    }

    DiskHeader h;
    memcpy(h.magic, DISK_MAGIC, DISK_MAGIC_LEN);
    h.version    = DISK_VERSION;
    h.file_count = 0;

    fp = fopen(name, "wb");
    if (!fp) {
        fprintf(stderr, "Error: cannot create '%s'\n", name);
        return;
    }

    fwrite(&h, sizeof(DiskHeader), 1, fp);
    fclose(fp);

    printf("created archive: %s\n", name);
}

void insert_file(char *arc_name, char *filename)
{
    arc a;
    if (!read_arc(arc_name, &a)) return;

    if (a.file_count >= DISK_MAX_FILES) {
        fprintf(stderr, "Error: archive full\n");
        free_arc(&a);
        return;
    }

    char *base = strrchr(filename, '/');
    base = base ? base + 1 : filename;

    for (uint i = 0; i < a.file_count; i++) {
        if (strcmp(a.files[i].name, base) == 0) {
            fprintf(stderr, "Error: '%s' already in archive\n", base);
            free_arc(&a);
            return;
        }
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s'\n", filename);
        free_arc(&a);
        return;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);

    uchar *data = malloc(sz);
    if (!data) {
        fprintf(stderr, "Error: out of memory\n");
        fclose(fp);
        free_arc(&a);
        return;
    }
    fread(data, 1, sz, fp);
    fclose(fp);

    uint n = a.file_count;
    file *nf = realloc(a.files, sizeof(file) * (n + 1));
    if (!nf) {
        fprintf(stderr, "Error: out of memory\n");
        free(data);
        free_arc(&a);
        return;
    }
    a.files = nf;
    a.files[n].name = strdup(base);
    a.files[n].size = (size_t)sz;
    a.files[n].data = data;
    a.file_count++;

    if (write_arc(arc_name, &a))
        printf("inserted: %s (%ld bytes)\n", base, sz);

    free_arc(&a);
}

void delete_file(char *arc_name, char *filename)
{
    arc a;
    if (!read_arc(arc_name, &a)) return;

    int found = -1;
    for (uint i = 0; i < a.file_count; i++) {
        if (strcmp(a.files[i].name, filename) == 0) {
            found = (int)i;
            break;
        }
    }
    if (found < 0) {
        fprintf(stderr, "Error: '%s' not found\n", filename);
        free_arc(&a);
        return;
    }

    free(a.files[found].name);
    free(a.files[found].data);

    for (uint i = found; i < a.file_count - 1; i++)
        a.files[i] = a.files[i + 1];
    a.file_count--;

    if (a.file_count == 0) {
        free(a.files);
        a.files = NULL;
    } else {
        file *sh = realloc(a.files, sizeof(file) * a.file_count);
        if (sh) a.files = sh;
    }

    if (write_arc(arc_name, &a))
        printf("deleted: %s\n", filename);

    free_arc(&a);
}

void extract_file(char *arc_name, char *filename)
{
    arc a;
    if (!read_arc(arc_name, &a)) return;

    int found = -1;
    for (uint i = 0; i < a.file_count; i++) {
        if (strcmp(a.files[i].name, filename) == 0) {
            found = (int)i;
            break;
        }
    }
    if (found < 0) {
        fprintf(stderr, "Error: '%s' not found\n", filename);
        free_arc(&a);
        return;
    }

    file *f = &a.files[found];
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: cannot write '%s'\n", filename);
        free_arc(&a);
        return;
    }
    fwrite(f->data, 1, f->size, fp);
    fclose(fp);

    printf("extracted: %s (%zu bytes)\n", filename, f->size);
    free_arc(&a);
}

void list_files(char *arc_name)
{
    arc a;
    if (!read_arc(arc_name, &a)) return;

    printf("\n");
    printf("  archive: %s\n", arc_name);
    printf("  version: %u\n", a.ver);
    printf("  files:   %u\n", a.file_count);
    printf("\n");

    if (a.file_count == 0) {
        printf("  (empty)\n");
    } else {
        printf("  %-4s  %-40s  %s\n", "No.", "Name", "Bytes");
        printf("  %-4s  %-40s  %s\n", "---", "----", "-----");
        for (uint i = 0; i < a.file_count; i++)
            printf("  %-4u  %-40s  %zu\n",
                   i + 1, a.files[i].name, a.files[i].size);
    }
    printf("\n");

    free_arc(&a);
}
