#include "../lib/fat16.h"

int is_fat16(FILE *fp) {
    struct fat16_reserved_area bs;
    fseek(fp, 0, SEEK_SET);
    fread(&bs, sizeof(bs), 1, fp);
    return strncmp(bs.BS_OEMName, "mkfs.fat", 8) == 0;
}

void print_fat16(FILE *fp) {
    struct fat16_reserved_area bs;
    fseek(fp, 0, SEEK_SET);
    fread(&bs, sizeof(bs), 1, fp);

    printf("\n------ Filesystem Information ------\n\n");
    printf("Filesystem: FAT16\n\n");
    printf("System name: %.8s\n", bs.BS_OEMName);
    printf("Sector size: %u\n", bs.BPB_BytsPerSec);
    printf("Sectors per Cluster: %u\n", bs.BPB_SecPerClus);
    printf("Reserved sectors: %u\n", bs.BPB_RsvdSecCnt);
    printf("# of FATs: %u\n", bs.BPB_NumFATs);
    printf("Max root entries: %u\n", bs.BPB_RootEntCnt);
    printf("Sectors per FAT: %u\n", bs.BPB_FATSz16);
    printf("Label: %.11s\n", bs.BS_VolLab);
}

