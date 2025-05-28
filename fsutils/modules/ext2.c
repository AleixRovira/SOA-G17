#include "../lib/ext2.h"

int is_ext2(FILE *fp) {
    uint16_t magic;
    fseek(fp, EXT2_SUPERBLOCK_OFFSET + 56, SEEK_SET);
    fread(&magic, sizeof(magic), 1, fp);
    return magic == EXT2_MAGIC;
}

void print_ext2(FILE *fp) {
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
