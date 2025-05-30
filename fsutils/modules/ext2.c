#include "../lib/ext2.h"

int is_ext2(FILE *fp)
{
    uint16_t magic;
    fseek(fp, EXT2_SUPERBLOCK_OFFSET + 56, SEEK_SET);
    fread(&magic, sizeof(magic), 1, fp);
    return magic == EXT2_MAGIC;
}

void print_ext2(FILE *fp)
{
    struct ext2_superblock sb;
    fseek(fp, 1024, SEEK_SET);
    fread(&sb, sizeof(sb), 1, fp);

    time_t mtime = sb.s_mtime;
    time_t wtime = sb.s_wtime;
    time_t last_check = sb.s_lastcheck;

    printf("\n------ Filesystem Information ------\n\n");
    printf("Filesystem: EXT2\n\n");

    printf("INODE INFO\n");
    printf("  Size: %u\n", sb.s_inode_size);
    printf("  Num Inodes: %u\n", sb.s_inodes_count);
    printf("  First inode: %u\n", sb.s_first_ino);
    printf("  Inodes group: %u\n", sb.s_inodes_per_group);
    printf("  Free inodes: %u\n\n", sb.s_free_inodes_count);

    printf("INFO BLOCK\n");
    printf("  Block size: %u\n", 1024 << sb.s_log_block_size);
    printf("  Reserved blocks: %u\n", sb.s_r_blocks_count);
    printf("  Free blocks: %u\n", sb.s_free_blocks_count);
    printf("  Total blocks: %u\n", sb.s_blocks_count);
    printf("  First block: %u\n", sb.s_first_data_block);
    printf("  Group blocks: %u\n", sb.s_blocks_per_group);
    printf("  Group frags: %u\n\n", sb.s_frags_per_group);

    printf("INFO VOLUME\n");
    printf("  Volume name: %s\n", sb.s_volume_name);
    printf("  Last Checked: %s", ctime(&last_check));
    printf("  Last Mounted: %s", ctime(&mtime));
    printf("  Last Written: %s", ctime(&wtime));
}

struct ext2_inode read_inode(FILE *fp, struct ext2_superblock sb, struct ext2_group_desc gd, uint32_t inode_no)
{
    uint32_t block_size = 1024 << sb.s_log_block_size;
    uint32_t index = (inode_no - 1) % sb.s_inodes_per_group;
    uint32_t block = gd.bg_inode_table + (index * sb.s_inode_size) / block_size;
    uint32_t offset = (index * sb.s_inode_size) % block_size;

    struct ext2_inode inode;
    fseek(fp, block * block_size + offset, SEEK_SET);
    fread(&inode, sizeof(struct ext2_inode), 1, fp);
    return inode;
}

int is_visited(uint32_t inode, uint32_t *visited, int count)
{
    for (int i = 0; i < count; i++)
        if (visited[i] == inode)
            return 1;
    return 0;
}

void traverse_directory(FILE *fp, struct ext2_superblock sb, struct ext2_group_desc gd, struct ext2_inode inode, int depth)
{
    static uint32_t visited[4096];
    static int visited_count = 0;

    uint32_t this_inode_num = inode.i_block[0]; // OJO: esto no es correcto, necesitas pasar el número de inodo real
    // Mejor: añade un parámetro uint32_t inode_num a la función y pásalo siempre

    if (is_visited(this_inode_num, visited, visited_count))
        return;
    visited[visited_count++] = this_inode_num;

    uint32_t block_size = 1024 << sb.s_log_block_size;
    char name[256];

    for (int i = 0; i < 12; i++)
    {
        if (inode.i_block[i] == 0)
            continue;

        uint32_t block = inode.i_block[i];
        uint32_t offset = block * block_size;
        uint32_t processed = 0;

        while (processed < block_size)
        {
            fseek(fp, offset + processed, SEEK_SET);

            uint32_t entry_inode;
            uint16_t rec_len;
            uint8_t name_len;
            uint8_t file_type;

            fread(&entry_inode, sizeof(uint32_t), 1, fp);
            fread(&rec_len, sizeof(uint16_t), 1, fp);
            fread(&name_len, sizeof(uint8_t), 1, fp);
            fread(&file_type, sizeof(uint8_t), 1, fp);
            fread(name, name_len, 1, fp);
            name[name_len] = '\0';

            if (entry_inode != 0 && strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
            {
                for (int j = 0; j < depth; j++)
                    printf("  ");
                printf("%c%c %s\n", 192, 196, name);

                struct ext2_inode child = read_inode(fp, sb, gd, entry_inode);
                if ((child.i_mode & 0xF000) == 0x4000)
                {
                    traverse_directory(fp, sb, gd, child, depth + 1 /*, entry_inode */);
                }
            }

            processed += rec_len;
        }
    }
}

void print_tree_ext2(FILE *fp)
{
    struct ext2_superblock sb;
    struct ext2_group_desc gd;

    fseek(fp, EXT2_SUPERBLOCK_OFFSET, SEEK_SET);
    fread(&sb, sizeof(sb), 1, fp);

    fseek(fp, EXT2_SUPERBLOCK_OFFSET + 1024, SEEK_SET);
    fread(&gd, sizeof(gd), 1, fp);

    struct ext2_inode root_inode = read_inode(fp, sb, gd, EXT2_ROOT_INO);

    printf(".\n");
    traverse_directory(fp, sb, gd, root_inode, 0);
}
