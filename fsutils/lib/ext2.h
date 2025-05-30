#ifndef EXT2_H
#define EXT2_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define EXT2_SUPERBLOCK_OFFSET 1024
#define EXT2_MAGIC 0xEF53
#define EXT2_ROOT_INO 2

#pragma pack(push, 1)
struct ext2_superblock {
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_r_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
    int32_t  s_log_frag_size;
	uint32_t s_blocks_per_group; 
    uint32_t s_frags_per_group;
    uint32_t s_inodes_per_group;
    uint32_t s_mtime;
    uint32_t s_wtime; 
    uint16_t s_mnt_count;
    int16_t  s_max_mnt_count;
    uint16_t s_magic;
    uint16_t s_state;
    uint16_t s_errors;
    uint16_t s_minor_rev_level;
    uint32_t s_lastcheck;
    uint32_t s_checkinterval;
	uint32_t s_creator_os;
    uint32_t s_rev_level;
    uint16_t s_def_resuid;
    uint16_t s_def_resgid;

    // --------- EXTENDED FIELDS ---------
    uint32_t s_first_ino;
    uint16_t s_inode_size;
    uint16_t s_block_group_nr;
    uint32_t s_feature_compat;
    uint32_t s_feature_incompat;
    uint32_t s_feature_ro_compat;
    uint8_t  s_uuid[16];
    char     s_volume_name[16];
    char     s_last_mounted[64];
    uint32_t s_algorithm_usage_bitmap;
};

struct ext2_group_desc {
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;
    uint32_t bg_reserved[3];
};

struct ext2_inode {
    uint16_t i_mode;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    uint32_t i_flags;
    union {
        struct {
            uint32_t l_i_reserved1;
        } linux1;
        struct {
            uint32_t h_i_translator;
        } hurd1;
        struct {
            uint32_t m_i_reserved1;
        } masix1;
    } osd1;

    uint32_t i_block[15];
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_dir_acl;
    uint32_t i_faddr;

    union {
        struct {
            uint8_t l_i_frag; 
            uint8_t l_i_fsize; 
            uint16_t l_i_reserved2; 
        } linux2;

        struct {
            uint8_t h_i_frag; 
            uint8_t h_i_fsize; 
            uint16_t h_i_mode_high; 
        } hurd2;

        struct {
            uint8_t m_i_frag; 
            uint8_t m_i_fsize; 
            uint16_t m_i_mode_high; 
        } masix2;

        struct {
            char l_i_reserved3[12];
        } reserved3;
    } osd2;
};

struct ext2_data_block {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t  name_len;
    uint8_t  file_type;
    char     name[];
};
#pragma pack(pop)

int is_ext2(FILE *fp);
void print_ext2(FILE *fp);
void print_tree_ext2(FILE *fp);

#endif

