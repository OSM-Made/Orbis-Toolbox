#pragma once

#define MAJOR_VERSION 1
#define MINOR_VERSION 0

//Which software version we want to compile for
#define SOFTWARE_VERSION_NA
#ifdef VERSION_505
#define VERSION_NUM 505
#define SOFTWARE_VERSION_505
#undef SOFTWARE_VERSION_NA
#endif
#ifdef VERSION_672
#define VERSION_NUM 672
#define SOFTWARE_VERSION_672
#undef SOFTWARE_VERSION_NA
#endif
#ifdef VERSION_702
#define VERSION_NUM 702
#define SOFTWARE_VERSION_702
#undef SOFTWARE_VERSION_NA
#endif
#ifdef VERSION_755
#define VERSION_NUM 755
#define SOFTWARE_VERSION_755
#undef SOFTWARE_VERSION_NA
#endif