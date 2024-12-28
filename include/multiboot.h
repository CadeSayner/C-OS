#pragma once
#include "stdint.h"
struct multiboot_aout_symbol_table{
    uint32_t tabSize;
    uint32_t strSize;
    uint32_t address;
    uint32_t reserved;
};

struct multiboot_elf_section_header_table{
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
};

struct multiboot_info{
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;

    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;

    union{
        struct multiboot_aout_symbol_table aout_sym;
        struct multiboot_elf_section_header_table elf_sec;
    } u;

    uint32_t mmap_length;
    uint32_t mmap_addr;

    uint32_t drives_length;
    uint32_t drives_addr;

    uint32_t config_table;
    uint32_t boot_loader_name;

    uint32_t apm_table;

    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
};

struct multiboot_mmap_entry
{
  uint32_t size;
  uint32_t addr_low;
  uint32_t addr_high;
  uint32_t len_low;
  uint32_t len_high;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  uint32_t type;
} __attribute__((packed));


struct Elf32_Ehdr{
    unsigned char e_ident[16];  // Magic number and other info
    uint16_t e_type;            // Type of file (e.g., executable, relocatable)
    uint16_t e_machine;         // Machine architecture (e.g., x86, x86_64)
    uint32_t e_version;         // ELF version
    uint32_t e_entry;           // Entry point address
    uint32_t e_phoff;           // Program header offset
    uint32_t e_shoff;           // Section header offset
    uint32_t e_flags;           // Processor-specific flags
    uint16_t e_ehsize;          // ELF header size
    uint16_t e_phentsize;       // Size of program header entry
    uint16_t e_phnum;           // Number of program headers
    uint16_t e_shentsize;       // Size of section header entry
    uint16_t e_shnum;           // Number of section headers
    uint16_t e_shstrndx;        // Section name string table index
};

struct module_struct{
    uint32_t module_start;
    uint32_t module_end;
    uint32_t module_name;
    uint32_t reserved;
};
