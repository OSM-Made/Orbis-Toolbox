#include "Common.h"
#include "ELF.h"
#include "Resolver/Resolver.h"

// thank you osdev for some help
static inline struct Elf64_Phdr *elf_pheader(struct Elf64_Ehdr *hdr) {
	if (!hdr->e_phoff) {
		return NULL;
	}

	return (struct Elf64_Phdr *)((uint64_t)hdr + hdr->e_phoff);
}
static inline struct Elf64_Phdr *elf_segment(struct Elf64_Ehdr *hdr, int idx) {
	uint64_t addr = (uint64_t)elf_pheader(hdr);
	if (!addr) {
		return NULL;
	}

	return (struct Elf64_Phdr *)(addr + (hdr->e_phentsize * idx));
}
static inline struct Elf64_Shdr *elf_sheader(struct Elf64_Ehdr *hdr) {
	if (!hdr->e_shoff) {
		return NULL;
	}

	return (struct Elf64_Shdr *)((uint64_t)hdr + hdr->e_shoff);
}
static inline struct Elf64_Shdr *elf_section(struct Elf64_Ehdr *hdr, int idx) {
	uint64_t addr = (uint64_t)elf_sheader(hdr);
	if (!addr) {
		return NULL;
	}

	return (struct Elf64_Shdr *)(addr + (hdr->e_shentsize * idx));
}

int elf_mapped_size(void *elf, size_t *msize) {
	struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)elf;

	// check magic
	if (memcmp(ehdr->e_ident, ElfMagic, 4)) {
		return LDR_INVALID_ELF;
	}

	size_t s = 0;

	struct Elf64_Phdr *phdr = elf_pheader(ehdr);
	if (phdr) {
		// use segments
		for (int i = 0; i < ehdr->e_phnum; i++) {
			struct Elf64_Phdr *phdr = elf_segment(ehdr, i);

			uint64_t delta = phdr->p_paddr + phdr->p_memsz;
			if (delta > s) {
				s = delta;
			}
		}
	} else {
		// use sections
		for (int i = 0; i < ehdr->e_shnum; i++) {
			struct Elf64_Shdr *shdr = elf_section(ehdr, i);

			uint64_t delta = shdr->sh_addr + shdr->sh_size;
			if (delta > s) {
				s = delta;
			}
		}
	}

	if (msize) {
		*msize = s;
	}

	return LDR_SUCCESS;
}

int map_elf(void *elf, void *exec) {
	struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)elf;

	struct Elf64_Phdr *phdr = elf_pheader(ehdr);
	if (phdr) {
		// use segments
		for (int i = 0; i < ehdr->e_phnum; i++) {
			struct Elf64_Phdr *phdr = elf_segment(ehdr, i);

			if (phdr->p_filesz) {
				memcpy((uint8_t *)exec + phdr->p_paddr, (uint8_t *)elf + phdr->p_offset, phdr->p_filesz);
			}

			if (phdr->p_memsz - phdr->p_filesz) {
				memset((uint8_t *)exec + phdr->p_paddr + phdr->p_filesz, NULL, phdr->p_memsz - phdr->p_filesz);
			}
		}
	} else {
		// use sections
		for (int i = 0; i < ehdr->e_shnum; i++) {
			struct Elf64_Shdr *shdr = elf_section(ehdr, i);

			if (!(shdr->sh_flags & SHF_ALLOC)) {
				continue;
			}

			if (shdr->sh_size) {
				memcpy((uint8_t *)exec + shdr->sh_addr, (uint8_t *)elf + shdr->sh_offset, shdr->sh_size);
			}
		}
	}

	return LDR_SUCCESS;
}

int relocate_elf(void *elf, void *exec) {
	struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)elf;

	for (int i = 0; i < ehdr->e_shnum; i++) {
		struct Elf64_Shdr *shdr = elf_section(ehdr, i);

		// check table
		if (shdr->sh_type == SHT_REL) {
			// process each entry in the table
			for (int j = 0; j < shdr->sh_size / shdr->sh_entsize; j++) {
				struct Elf64_Rela *reltab = &((struct Elf64_Rela *)((uint64_t)ehdr + shdr->sh_offset))[j];
				uint8_t **ref = (uint8_t **)((uint8_t *)exec + reltab->r_offset);

				switch (ELF64_R_TYPE(reltab->r_info)) {
				case R_X86_64_RELATIVE:
					*ref = (uint8_t *)exec + reltab->r_addend;
					break;
				case R_X86_64_64:
				case R_X86_64_JUMP_SLOT:
				case R_X86_64_GLOB_DAT:
					// TODO: relocations
					break;
				}
			}
		}
	}

	return LDR_SUCCESS;
}

int load_elf(void *elf, size_t size, void *exec, size_t msize, void **entry) {
	size_t s = 0;

	if (!elf || !exec || !size || !msize)
		return LDR_INVALID_ELF;

	struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)elf;

	if (memcmp(ehdr->e_ident, ElfMagic, 4))
		return LDR_INVALID_ELF;

	if (elf_mapped_size(elf, &s))
		return LDR_SIZE_ERROR;

	if (s > msize)
		return LDR_SIZE_ERROR;

	if (map_elf(elf, exec))
		return LDR_MAP_ERROR;

	if (relocate_elf(elf, exec))
		return LDR_RELOC_ERROR;

	*entry = (void *)((uint64_t)exec + ehdr->e_entry);

	return LDR_SUCCESS;
}
