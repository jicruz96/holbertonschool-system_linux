#include "hreadelf.h"

#define PH_SUMMARY "There are %d program headers, starting at offset %d\n\n"
#define PH_INTRO "Program Headers:"
#define PH_FIRST_ROW \
"  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align"
#define COLUMN_WIDTH 34

/**
 * main - replica of readelf -W -h <file>, where <file> is argv[1]
 * @argc: argument count
 * @argv: argument array
 * Return: 0 on success or no arguments passed | 1 on failure
 **/
int main(int argc, char *argv[])
{
	int elf_file;

	if (argc > 1)
	{
		if (open_ELF_file(&elf_file, argv[1], "readelf") == ELFCLASS64)
			print_elf64_header(elf_file);
		else
			print_elf32_header(elf_file);
	}

	return (EXIT_SUCCESS);
}

/**
 * print_elf64_header - prints ELF64 section header table
 * @elf_file: ELF file descriptor
 **/
void print_elf64_header(int elf_file)
{
	Elf64_Ehdr h;
	int i;

	read(elf_file, &h, sizeof(h));

	puts("ELF Header:");

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%02x ", h.e_ident[i]);
	putchar('\n');

	printf("  %-34s ELF%d\n", "Class:", 32 * h.e_ident[EI_CLASS]);
	printf("  %-34s 2's complement, ", "Data:");
	printf("%s endian\n", h.e_ident[EI_DATA] == ELFDATA2LSB ? "little" : "big");
	printf("  %-34s %d", "Version:", h.e_ident[EI_VERSION]);
	printf("%s\n", h.e_ident[EI_VERSION] == EV_CURRENT ? " (current)" : "");
	printf("  %-34s ", "OS/ABI:");
	print_os_abi(h.e_ident[EI_OSABI]);
	printf("  %-34s %d\n", "ABI Version:", h.e_ident[EI_ABIVERSION]);
	printf("  %-34s ", "Type:");
	print_ELF_file_type(h.e_type);
	printf("  %-34s ", "Machine:");
	print_machine(h.e_machine);
	printf("  %-34s 0x%x\n", "Version:", h.e_version);
	printf("  %-34s 0x%x\n", "Entry point address:", (unsigned int)h.e_entry);
	printf("  %-34s %u ", "Start of program headers:", (unsigned int)h.e_phoff);
	printf("(bytes into file)\n");
	printf("  %-34s %u ", "Start of section headers:", (unsigned int)h.e_shoff);
	printf("(bytes into file)\n");
	printf("  %-34s 0x%x\n", "Flags:", h.e_flags);
	printf("  %-34s %i (bytes)\n", "Size of this header:", h.e_ehsize);
	printf("  %-34s %i (bytes)\n", "Size of program headers:", h.e_phentsize);
	printf("  %-34s %u\n", "Number of program headers:", h.e_phnum);
	printf("  %-34s %u (bytes)\n", "Size of section headers:", h.e_shentsize);
	printf("  %-34s %u\n", "Number of section headers:", h.e_shnum);
	printf("  %-34s %u\n", "Section header string table index:", h.e_shstrndx);
}

/**
 * print_machine - prints machine information for ELF header
 * @e_machine: value
 **/
void print_machine(uint16_t e_machine)
{
	macro_matcher_t machines[] = {
		{EM_M32, "AT&T WE 32100"},
		{EM_SPARC, "Sun Microsystems SPARC"},
		{EM_386, "Intel 80386"},
		{EM_68K, "Motorola 68000"},
		{EM_88K, "Motorola 88000"},
		{EM_860, "Intel 80860"},
		{EM_MIPS, "MIPS RS3000"},
		{EM_PARISC, "HP/PA"},
		{EM_SPARC32PLUS, "SPARC with enhanced instruction set"},
		{EM_PPC, "PowerPC"},
		{EM_PPC64, "PowerPC 64-bit"},
		{EM_S390, "IBM S/390"},
		{EM_ARM, "Advanced RISC Machines"},
		{EM_SH, "Renesas SuperH"},
		{EM_SPARCV9, "SPARC v9 64-bit"},
		{EM_IA_64, "Intel Itanium"},
		{EM_X86_64, "Advanced Micro Devices X86-64"},
		{EM_VAX, "DEC Vax"}
	};
	unsigned int i;

	for (i = 0; i < sizeof(machines) / sizeof(macro_matcher_t); i++)
		if (e_machine == machines[i].macro)
		{
			puts(machines[i].macro_string);
			return;
		}

	printf("<unknown: %x>\n", e_machine);
}

/**
 * print_os_abi - prints OS/ABI info for ELF header
 * @os_abi: value
 **/
void print_os_abi(unsigned char os_abi)
{
	macro_matcher_t oses[] = {
		{ELFOSABI_NONE, "UNIX - System V"},
		{ELFOSABI_SYSV, "UNIX - System V"},
		{ELFOSABI_HPUX, "HP-UX"},
		{ELFOSABI_NETBSD, "UNIX - NetBSD"},
		{ELFOSABI_LINUX, "Linux"},
		{ELFOSABI_SOLARIS, "UNIX - Solaris"},
		{ELFOSABI_IRIX, "IRIX"},
		{ELFOSABI_FREEBSD, "FreeBSD"},
		{ELFOSABI_TRU64, "TRU64 UNIX"},
		{ELFOSABI_ARM, "ARM architecture"},
		{ELFOSABI_STANDALONE, "Stand-alone (embedded) ABI"}
	};
	unsigned int i;

	for (i = 0; i < sizeof(oses) / sizeof(macro_matcher_t); i++)
	{
		if (os_abi == oses[i].macro)
		{
			puts(oses[i].macro_string);
			return;
		}
	}
	printf("<unknown: %x>\n", os_abi);
}

/**
 * print_elf32_header - prints ELF32 section header table
 * @elf_file: ELF file descriptor
 **/
void print_elf32_header(int elf_file)
{
	Elf32_Ehdr h;
	int i;

	read(elf_file, &h, sizeof(h));

	puts("ELF Header:");

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%02x ", h.e_ident[i]);
	putchar('\n');

	printf("  %-34s ELF%d\n", "Class:", 32 * h.e_ident[EI_CLASS]);
	printf("  %-34s 2's complement, ", "Data:");
	printf("%s endian\n", h.e_ident[EI_DATA] == ELFDATA2LSB ? "little" : "big");
	printf("  %-34s %d", "Version:", h.e_ident[EI_VERSION]);
	printf("%s\n", h.e_ident[EI_VERSION] == EV_CURRENT ? " (current)" : "");
	printf("  %-34s ", "OS/ABI:");
	print_os_abi(h.e_ident[EI_OSABI]);
	printf("  %-34s %d\n", "ABI Version:", h.e_ident[EI_ABIVERSION]);
	printf("  %-34s ", "Type:");
	print_ELF_file_type(h.e_type);
	printf("  %-34s ", "Machine:");
	print_machine(h.e_machine);
	printf("  %-34s 0x%x\n", "Version:", h.e_version);
	printf("  %-34s 0x%x\n", "Entry point address:", (unsigned int)h.e_entry);
	printf("  %-34s %u ", "Start of program headers:", (unsigned int)h.e_phoff);
	printf("(bytes into file)\n");
	printf("  %-34s %u ", "Start of section headers:", (unsigned int)h.e_shoff);
	printf("(bytes into file)\n");
	printf("  %-34s 0x%x\n", "Flags:", h.e_flags);
	printf("  %-34s %i (bytes)\n", "Size of this header:", h.e_ehsize);
	printf("  %-34s %i (bytes)\n", "Size of program headers:", h.e_phentsize);
	printf("  %-34s %u\n", "Number of program headers:", h.e_phnum);
	printf("  %-34s %u (bytes)\n", "Size of section headers:", h.e_shentsize);
	printf("  %-34s %u\n", "Number of section headers:", h.e_shnum);
	printf("  %-34s %u\n", "Section header string table index:", h.e_shstrndx);
}
