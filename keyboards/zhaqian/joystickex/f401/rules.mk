# MCU name
MCU = STM32F401

# Bootloader selection
BOOTLOADER = stm32-dfu
BOARD = BLACKPILL_STM32_F401

# Bootloader selection
BOOTLOADER = custom

EEPROM_DRIVER = wear_leveling
WEAR_LEVELING_DRIVER = embedded_flash

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
# SRC += uf2_boot.c


