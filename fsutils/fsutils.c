#include <stdio.h>
#include <string.h>
#include "lib/ext2.h"
#include "lib/fat16.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./fsutils --info|--tree|--cat <filesystem_image>\n");
        return 1;
    }

    if (strcmp(argv[1], "--info") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: ./fsutils --info <filesystem_image>\n");
            return 1;
        }

        char path[256];
        snprintf(path, sizeof(path), "files/%s", argv[2]);
        FILE *fp = fopen(path, "rb");
        if (!fp)
        {
            perror("Error opening file");
            return 1;
        }

        if (is_ext2(fp))
        {
            print_ext2(fp);
        }
        else if (is_fat16(fp))
        {
            print_fat16(fp);
        }
        else
        {
            fprintf(stderr, "Unsupported or unknown filesystem format\n");
        }

        fclose(fp);
    }
    else if (strcmp(argv[1], "--tree") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: ./fsutils --tree <filesystem_image>\n");
            return 1;
        }

        char path[256];
        snprintf(path, sizeof(path), "files/%s", argv[2]);
        FILE *fp = fopen(path, "rb");
        if (!fp)
        {
            perror("Error opening file");
            return 1;
        }

        if (is_ext2(fp))
        {
            print_tree_ext2(fp);
        }
        else if (is_fat16(fp))
        {
            // Show tree fat
        }
        else
        {
            fprintf(stderr, "Unsupported or unknown filesystem format\n");
        }

        fclose(fp);
    }
    else if (strcmp(argv[1], "--cat") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: ./fsutils --cat <filesystem_image> <file>\n");
            return 1;
        }
        // Fase3 y 4
    }
    else
    {
        fprintf(stderr, "ERROR: Invalid usage\n");
        return 1;
    }

    return 0;
}
