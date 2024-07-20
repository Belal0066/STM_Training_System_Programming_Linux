#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define EI_NIDENT 16


typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;


void print_elf_header(Elf64_Ehdr *header) {
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%2.2x ", header->e_ident[i]);
    }
    printf("\n");

    printf("  Class:                             ");
    if (header->e_ident[4] == 1) {
        printf("ELF32\n");
    } else if (header->e_ident[4] == 2) {
        printf("ELF64\n");
    } else {
        printf("Unknown\n");
    }

    printf("  Data:                              ");
    if (header->e_ident[5] == 1) {
        printf("2's complement, little endian\n");
    } else if (header->e_ident[5] == 2) {
        printf("2's complement, big endian\n");
    } else {
        printf("Unknown\n");
    }

    printf("  Version:                           %d (current)\n", header->e_ident[6]);
    printf("  OS/ABI:                            ");
    switch (header->e_ident[7]) {
        case 0: printf("UNIX System V\n"); break;
        case 1: printf("HP-UX\n"); break;
        case 2: printf("NetBSD\n"); break;
        case 3: printf("Linux\n"); break;
        default: printf("Unknown\n"); break;
    }

    printf("  ABI Version:                       %d\n", header->e_ident[8]);
    printf("  Type:                              ");
    switch (header->e_type) {
        case 1: printf("REL (Relocatable file)\n"); break;
        case 2: printf("EXEC (Executable file)\n"); break;
        case 3: printf("DYN (Shared object file)\n"); break;
        case 4: printf("CORE (Core file)\n"); break;
        default: printf("Unknown\n"); break;
    }

    printf("  Machine:                           ");
    switch (header->e_machine) {
        case 62: printf("Advanced Micro Devices X86-64\n"); break;
        default: printf("Unknown\n"); break;
    }

    printf("  Version:                           0x%x\n", header->e_version);
    printf("  Entry point address:               0x%lx\n", header->e_entry);
    printf("  Start of program headers:          %lu (bytes into file)\n", header->e_phoff);
    printf("  Start of section headers:          %lu (bytes into file)\n", header->e_shoff);
    printf("  Flags:                             0x%x\n", header->e_flags);
    printf("  Size of this header:               %u (bytes)\n", header->e_ehsize);
    printf("  Size of program headers:           %u (bytes)\n", header->e_phentsize);
    printf("  Number of program headers:         %u\n", header->e_phnum);
    printf("  Size of section headers:           %u (bytes)\n", header->e_shentsize);
    printf("  Number of section headers:         %u\n", header->e_shnum);
    printf("  Section header string table index: %u\n", header->e_shstrndx);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ELF_FILE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    Elf64_Ehdr header;
    fread(&header, 1, sizeof(header), file);

    print_elf_header(&header);

    fclose(file);
    return EXIT_SUCCESS;
}
