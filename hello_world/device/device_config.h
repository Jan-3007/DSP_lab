/*
 * Author: Jan Eberhardt
 */

#pragma once


/**
 * @brief Stack size
 */
#define STACK_SIZE 0x00001000   /* 0x1000 => 4,096 bytes */


/**
 * @brief Heap size
 */
#define HEAP_SIZE 0x00000400    /* 0x0400 => 1,024 bytes */


/**
 * @brief libraries for the linker
 * 
 * crt0.o crti.o crtn.o crtbegin.o crtend.o     mandatory 
 * libgcc.a                                     compiler stubs 
 * libc.a libnosys.a                            C runtime 
 * libstdc++.a                                  C++ runtime - complete 
 * libsupc++.a                                  C++ runtime - subset (alternative to complete version) 
 * libm.a                                       math lib (optional) 
 */                
#define LINKER_LIBS crt0.o crti.o crtn.o crtbegin.o crtend.o libgcc.a libc.a libnosys.a libsupc++.a libm.a


/**
 * @brief Flash specs
 */
#define ROM_BASE 0x00000000
#define ROM_SIZE 0x00200000     /* 2048 Kbytes => 2,097,152 bytes */

#define ROM1_BASE 0x00406000    /* unused */
#define ROM1_SIZE 0x0000A000    /* 40 Kbytes => 40,960 bytes */


/**
 * @brief RAM specs
 */
#define RAM_BASE 0x1FFD0000     
#define RAM_SIZE 0x00030000     /* 192 Kbytes => 196,608 bytes*/

#define RAM1_BASE 0x20038000    /* unused */
#define RAM1_SIZE 0x00008000    /* 32 Kbytes => 32,768 bytes */

#define RAM2_BASE 0x20040000    /* unused */
#define RAM2_SIZE 0x00008000    /* 32 Kbytes => 32,768 bytes */



