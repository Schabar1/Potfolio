#include "gbv.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char current_archive[256];

typedef struct
{
    char key[KEY_SIZE + 1];
    int num_docs;
    long dir_offset;
} Superblock;

int find_document(const Library *lib, const char *name)
{
    for (int i = 0; i < lib->count; i++)
        if (strcmp(lib->docs[i].name, name) == 0)
            return i;

    return -1;
}

int gbv_create(const char *filename, const char *key)
{
    if (!key || strlen(key) != KEY_SIZE)
        return 1;

    FILE *f = fopen(filename, "wb");
    if (!f)
        return 1;

    Superblock sb;
    strncpy(sb.key, key, KEY_SIZE);
    sb.num_docs = 0;
    sb.dir_offset = sizeof(Superblock);

    fwrite(&sb, sizeof(sb), 1, f);

    fclose(f);
    return 0;
}

int gbv_open(Library *lib, const char *filename, const char *key)
{
    lib->docs = NULL;
    lib->count = 0;

    if (!key || strlen(key) != KEY_SIZE)
        return 1;

    strncpy(current_archive, filename, sizeof(current_archive));
    current_archive[sizeof(current_archive) - 1] = '\0';

    FILE *f = fopen(filename, "rb");

    if (!f)
    {
        if (gbv_create(filename, key) != 0)
            return 1;

        f = fopen(filename, "rb");
        if (!f)
            return 1;
    }

    Superblock sb;
    if (fread(&sb, sizeof(sb), 1, f) != 1)
    {
        fclose(f);
        return 1;
    }

    if (strncmp(sb.key, key, KEY_SIZE) != 0)
    {
        printf("Acesso Negado\n");
        fclose(f);
        return 1;
    }

    lib->count = sb.num_docs;

    if (lib->count > 0)
    {
        lib->docs = malloc(lib->count * sizeof(Document));
        if (!lib->docs)
        {
            fclose(f);
            return 1;
        }

        fseek(f, sb.dir_offset, SEEK_SET);
        fread(lib->docs, sizeof(Document), lib->count, f);
    }
    else
    {
        lib->docs = NULL;
    }

    fclose(f);
    return 0;
}

int gbv_add(Library *lib, const char *archive, const char *docname)
{
    FILE *src = fopen(docname, "rb");
    if (!src)
        return 1;

    FILE *dst = fopen(archive, "rb+");
    if (!dst)
    {
        fclose(src);
        return 1;
    }

    Superblock sb;
    fread(&sb, sizeof(sb), 1, dst);

    fseek(dst, sb.dir_offset, SEEK_SET);

    Document newdoc;
    newdoc.offset = ftell(dst);
    newdoc.size = 0;
    newdoc.date = time(NULL);

    strncpy(newdoc.name, docname, MAX_NAME);
    newdoc.name[MAX_NAME - 1] = '\0';

    char buffer[BUFFER_SIZE];
    size_t n;

    while ((n = fread(buffer, 1, BUFFER_SIZE, src)) > 0)
    {
        fwrite(buffer, 1, n, dst);
        newdoc.size += n;
    }

    Document *tmp = realloc(lib->docs, (lib->count + 1) * sizeof(Document));

    if (!tmp)
    {
        fclose(src);
        fclose(dst);
        return 1;
    }

    lib->docs = tmp;
    lib->docs[lib->count] = newdoc;
    lib->count++;

    sb.num_docs = lib->count;
    sb.dir_offset = ftell(dst);

    fwrite(lib->docs, sizeof(Document), lib->count, dst);

    rewind(dst);
    fwrite(&sb, sizeof(sb), 1, dst);

    fclose(src);
    fclose(dst);

    return 0;
}

int gbv_remove(Library *lib, const char *docname)
{
    int idx = find_document(lib, docname);
    if (idx < 0)
        return 1;

    for (int i = idx; i < lib->count - 1; i++)
        lib->docs[i] = lib->docs[i + 1];

    lib->count--;

    Document *tmp = realloc(lib->docs, lib->count * sizeof(Document));

    if (tmp || lib->count == 0)
        lib->docs = tmp;

    FILE *f = fopen(current_archive, "rb+");
    if (!f)
        return 1;

    Superblock sb;
    fread(&sb, sizeof(sb), 1, f);

    sb.num_docs = lib->count;

    fseek(f, sb.dir_offset, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, f);

    rewind(f);
    fwrite(&sb, sizeof(sb), 1, f);

    fclose(f);

    return 0;
}

int gbv_list(const Library *lib)
{
    char date_str[64];

    for (int i = 0; i < lib->count; i++)
    {
        format_date(lib->docs[i].date, date_str, sizeof(date_str));

        printf("%s %ld %s %ld\n", lib->docs[i].name, lib->docs[i].size, date_str, lib->docs[i].offset);
    }

    return 0;
}

int cmp_name(const void *a, const void *b)
{
    return strcmp(((Document *)a)->name, ((Document *)b)->name);
}

int cmp_date(const void *a, const void *b)
{
    const Document *d1 = a;
    const Document *d2 = b;

    if (d1->date < d2->date)
        return -1;
    if (d1->date > d2->date)
        return 1;
    return 0;
}

int cmp_size(const void *a, const void *b)
{
    const Document *d1 = a;
    const Document *d2 = b;

    if (d1->size < d2->size)
        return -1;
    if (d1->size > d2->size)
        return 1;
    return 0;
}

int gbv_order(Library *lib, const char *archive, const char *criteria)
{
    if (strcmp(criteria, "nome") == 0)
        qsort(lib->docs, lib->count, sizeof(Document), cmp_name);

    else if (strcmp(criteria, "data") == 0)
        qsort(lib->docs, lib->count, sizeof(Document), cmp_date);

    else if (strcmp(criteria, "tamanho") == 0)
        qsort(lib->docs, lib->count, sizeof(Document), cmp_size);

    else
        return 1;

    FILE *f = fopen(archive, "rb+");
    if (!f)
        return 1;

    Superblock sb;
    fread(&sb, sizeof(sb), 1, f);

    fseek(f, sb.dir_offset, SEEK_SET);
    fwrite(lib->docs, sizeof(Document), lib->count, f);

    fclose(f);

    return 0;
}

int gbv_view(const Library *lib, const char *docname)
{
    int idx = find_document(lib, docname);

    if (idx < 0)
    {
        printf("Documento não encontrado\n");
        return 1;
    }

    Document doc = lib->docs[idx];

    FILE *f = fopen(current_archive, "rb");
    if (!f)
        return 1;

    char buffer[BUFFER_SIZE];

    long total_blocks = (doc.size + BUFFER_SIZE - 1) / BUFFER_SIZE;

    long block = 0;
    char cmd;

    while (1)
    {
        long pos = doc.offset + block * BUFFER_SIZE;

        fseek(f, pos, SEEK_SET);

        size_t bytes = fread(buffer, 1, BUFFER_SIZE, f);

        printf("\nBloco %ld/%ld\n", block + 1, total_blocks);

        fwrite(buffer, 1, bytes, stdout);

        printf("\n[n] próximo | [p] anterior | [q] sair: ");

        scanf(" %c", &cmd);

        if (cmd == 'q')
            break;

        if (cmd == 'n' && block < total_blocks - 1)
            block++;

        if (cmd == 'p' && block > 0)
            block--;
    }

    fclose(f);
    return 0;
}
