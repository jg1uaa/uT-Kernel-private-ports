/*
 * hardware configuration
 */

#ifndef _HWCONFIG_H_
#define _HWCONFIG_H_

#define USE_PROTECT_MODE 1
#define USE_TMONITOR 1

#define ENABLE_16MHz_CRYSTAL    0x033f0002

#define INTERNAL_RAM_SIZE       0x00002000
#define INTERNAL_RAM_START      0x00000000
#define INTERNAL_RAM_END        0x00002000
#define INTERNAL_RAM_START_BEFORE_REMAP 0x00300000
#define INTERNAL_RAM_END_BEFORE_REMAP   0x00302000

#define FLASH_BASE      0x10000000
#define SRAM_BASE       0x20000000
#define ETH_BASE        0x40000000

#define FLASH_SIZE      0x00400000
#define FLASH_START     FLASH_BASE
#define FLASH_END       (FLASH_START+FLASH_SIZE)
#define SRAM_SIZE       0x00200000
#define SRAM_START      SRAM_BASE
#define SRAM_END        (SRAM_START+SRAM_SIZE)

#define EBI_CSR0        (FLASH_BASE | 0x20b1)
#define EBI_CSR1        (SRAM_BASE | 0x30a9)
#define EBI_CSR2        0x00000000
#define EBI_CSR3        0x00000000
#define EBI_CSR4        0x00000000
#define EBI_CSR5        0x00000000
#define EBI_CSR6        0x00000000
#define EBI_CSR7        (ETH_BASE | 0x2035)

#endif /* _CONFIG_H_ */
