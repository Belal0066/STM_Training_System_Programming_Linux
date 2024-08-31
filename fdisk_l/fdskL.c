#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <uuid/uuid.h>
#include <endian.h>
#include <dirent.h>
#include <errno.h>

#define SECTOR_SIZE 512
#define MBR_SIGN 0xAA55
#define GPT_SIGN 0x5452415020494645ULL

typedef struct {
    uint8_t boot_indicator;
    uint8_t start_head;
    uint16_t start_sector_cylinder;
    uint8_t system_id;
    uint8_t end_head;
    uint16_t end_sector_cylinder;
    uint32_t start_sector;
    uint32_t total_sectors;
} __attribute__((packed)) MBRPartitionEntry;

typedef struct {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t header_crc32;
    uint32_t reserved;
    uint64_t current_lba;
    uint64_t backup_lba;
    uint64_t first_usable_lba;
    uint64_t last_usable_lba;
    uuid_t disk_guid;
    uint64_t partition_entry_lba;
    uint32_t num_partition_entries;
    uint32_t partition_entry_size;
    uint32_t partition_array_crc32;
} __attribute__((packed)) GPTHeader;

typedef struct {
    uuid_t type_guid;
    uuid_t unique_guid;
    uint64_t starting_lba;
    uint64_t ending_lba;
    uint64_t attributes;
    uint16_t name[36];
} __attribute__((packed)) GPTPartitionEntry;

enum PartitionTableType {
    UNKNOWN,
    MBR,
    GPT
};

const char *get_mbr_partition_type(uint8_t system_id) {
    switch (system_id) {
        case 0x5:
        case 0xF:
            return "Extended";
        case 0x83:
            return "Linux";
        default:
            return "Unknown";
    }
}

const char *get_gpt_partition_type(char guid_str[24]) {
    
    const char *LINUX_FILESYSTEM_GUID = "af3dc60f-8384-7247-8e79-3d69d8477de4";
    const char *EFI_SYSTEM_GUID = "28732ac1-1ff8-d211-ba4b-00a0c93ec93b";
    const char *BIOS_BOOT_GUID = "48616821-4964-6f6e-744e-656564454649";

    
    
    

    if (strcmp(guid_str, LINUX_FILESYSTEM_GUID) == 0) {
        return "Linux filesystem";
    } else if (strcmp(guid_str, EFI_SYSTEM_GUID) == 0) {
        return "EFI System";
    } else if (strcmp(guid_str, BIOS_BOOT_GUID) == 0) {
        return "BIOS boot";
    } else {
        return guid_str;
    }
}


void print_mbr_partition_info(MBRPartitionEntry *entry, const char *device) {
    printf("%-10s %10u %10u %10u %3uG %2X %s\n",
           device,
           entry->start_sector,
           entry->start_sector + entry->total_sectors - 1,
           entry->total_sectors,
           (unsigned int)((uint64_t)entry->total_sectors * SECTOR_SIZE / (1024 * 1024 * 1024)),
           entry->system_id,
           get_mbr_partition_type(entry->system_id));
}


void print_gpt_partition_info(GPTPartitionEntry *entry, int partition_number, const char *device) {
    char uuid_str[37];
    uuid_unparse(entry->type_guid, uuid_str);
    
    printf("%-11s %10llu %10llu %10llu %5.1fG %s\n",
           device,
           (unsigned long long)entry->starting_lba,
           (unsigned long long)entry->ending_lba,
           (unsigned long long)(entry->ending_lba - entry->starting_lba + 1),
           (double)(entry->ending_lba - entry->starting_lba + 1) * SECTOR_SIZE / (1024.0 * 1024 * 1024),
           get_gpt_partition_type(uuid_str));  
}




void print_disk_info(const char *device, uint64_t size_in_bytes, struct hd_driveid *hd, int logical_sector_size, int physical_sector_size) {
    printf("\n\033[1mDisk %s: %.1f GiB, %llu bytes, %llu sectors\033[0m\n", 
           device, 
           (double)size_in_bytes / (1024 * 1024 * 1024), 
           (unsigned long long)size_in_bytes, 
           (unsigned long long)size_in_bytes / SECTOR_SIZE);
    printf("Disk model: %.40s\n", hd->model);
    printf("Units: sectors of 1 * %d = %d bytes\n", SECTOR_SIZE, SECTOR_SIZE);
    printf("Sector size (logical/physical): %d bytes / %d bytes\n", logical_sector_size, physical_sector_size);
    printf("I/O size (minimum/optimal): %d bytes / %d bytes\n", logical_sector_size, logical_sector_size);
}

enum PartitionTableType detect_partition_table_type(int fd) {
    unsigned char buffer[SECTOR_SIZE];
    if (read(fd, buffer, SECTOR_SIZE) != SECTOR_SIZE) {
        perror("Error reading first sector");
        return UNKNOWN;
    }

    if (*(uint16_t *)(buffer + 510) == MBR_SIGN) {
        lseek(fd, SECTOR_SIZE, SEEK_SET);
        if (read(fd, buffer, SECTOR_SIZE) != SECTOR_SIZE) {
            perror("Error reading second sector");
            return UNKNOWN;
        }
        if (*(uint64_t *)buffer == GPT_SIGN) {
            return GPT;
        }
        return MBR;
    }

    return UNKNOWN;
}

void handle_mbr(int fd, const char *device) {
    unsigned char buffer[SECTOR_SIZE];
    lseek(fd, 0, SEEK_SET);
    if (read(fd, buffer, SECTOR_SIZE) != SECTOR_SIZE) {
        perror("Error reading MBR");
        return;
    }

    printf("Disklabel type: dos\n");
    printf("Disk identifier: 0x%08x\n", *(uint32_t *)(buffer + 440));

    printf("\n\033[1mDevice     Boot   Start      End  Sectors Size Id Type\033[0m\n");

    for (int i = 0; i < 4; i++) {
        MBRPartitionEntry *entry = (MBRPartitionEntry *)(buffer + 446 + i * 16);
        if (entry->total_sectors != 0) {
            char part_device[20];
            snprintf(part_device, sizeof(part_device), "%s%d", device, i + 1);
            print_mbr_partition_info(entry, part_device);
        }
    }

    for (int i = 0; i < 4; i++) {
        MBRPartitionEntry *entry = (MBRPartitionEntry *)(buffer + 446 + i * 16);
        if (entry->system_id == 0x5 || entry->system_id == 0xF) {
            uint32_t ebr_sector = entry->start_sector;
            int logical_count = 5;
            while (ebr_sector != 0) {
                lseek(fd, (off_t)ebr_sector * SECTOR_SIZE, SEEK_SET);
                if (read(fd, buffer, SECTOR_SIZE) != SECTOR_SIZE) {
                    perror("Error reading EBR");
                    break;
                }
                MBRPartitionEntry *logical_entry = (MBRPartitionEntry *)(buffer + 446);
                if (logical_entry->total_sectors != 0) {
                    char part_device[20];
                    snprintf(part_device, sizeof(part_device), "%s%d", device, logical_count);
                    logical_entry->start_sector += ebr_sector;
                    print_mbr_partition_info(logical_entry, part_device);
                    logical_count++;
                }
                ebr_sector = *(uint32_t *)(buffer + 446 + 16);
                if (ebr_sector != 0)
                    ebr_sector += entry->start_sector;
            }
        }
    }
}

void handle_gpt(int fd, const char *device) {
    GPTHeader gpt_header;
    lseek(fd, SECTOR_SIZE, SEEK_SET);
    if (read(fd, &gpt_header, sizeof(GPTHeader)) != sizeof(GPTHeader)) {
        perror("Error reading GPT header");
        return;
    }

    printf("Disklabel type: gpt\n");
    
    char uuid_str[37];
    uuid_unparse(gpt_header.disk_guid, uuid_str);
    printf("Disk identifier: %s\n", uuid_str);

    printf("\n\033[1mDevice       Start       End   Sectors  Size Type\033[0m\n");

    lseek(fd, le64toh(gpt_header.partition_entry_lba) * SECTOR_SIZE, SEEK_SET);
    for (int i = 0; i < le32toh(gpt_header.num_partition_entries); i++) {
        GPTPartitionEntry entry;
        if (read(fd, &entry, sizeof(GPTPartitionEntry)) != sizeof(GPTPartitionEntry)) {
            perror("Error reading partition entry");
            break;
        }
        
        if (entry.starting_lba != 0 || entry.ending_lba != 0) {
            char part_device[20];
            snprintf(part_device, sizeof(part_device), "%s%d", device, i + 1);
            print_gpt_partition_info(&entry, i + 1, part_device);
        }
    }
}

void process_device(const char *device) {
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        return;
    }

    uint64_t size_in_bytes;
    if (ioctl(fd, BLKGETSIZE64, &size_in_bytes) == -1) {
        perror("Error getting device size");
        close(fd);
        return;
    }

    int logical_sector_size, physical_sector_size;
    if (ioctl(fd, BLKSSZGET, &logical_sector_size) == -1 || ioctl(fd, BLKPBSZGET, &physical_sector_size) == -1) {
        perror("Error getting sector sizes");
        close(fd);
        return;
    }

    struct hd_driveid hd;
    if (ioctl(fd, HDIO_GET_IDENTITY, &hd) == -1) {
        perror("Error getting disk identity");
    }

    print_disk_info(device, size_in_bytes, &hd, logical_sector_size, physical_sector_size);

    enum PartitionTableType table_type = detect_partition_table_type(fd);

    switch (table_type) {
        case MBR:
            handle_mbr(fd, device);
            break;
        case GPT:
            handle_gpt(fd, device);
            break;
        case UNKNOWN:
            printf("Unknown partition table type\n");
            break;
    }

    close(fd);
}

void iterate_devices() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir("/dev")) == NULL) {
        perror("opendir(/dev)");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "sd", 2) == 0 && strlen(entry->d_name) == 3) {
            char device_path[300];
            snprintf(device_path, sizeof(device_path), "/dev/%s", entry->d_name);
            process_device(device_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        iterate_devices();
    } else if (argc == 2) {
        process_device(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s [device]\n", argv[0]);
        return 1;
    }

    return 0;
}
