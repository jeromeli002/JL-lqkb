# MCU name
MCU = STM32F103

# Bootloader selection
BOOTLOADER = stm32duino

SERIAL_DRIVER = usart
SPLIT_KEYBOARD = yes

LTO_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
NKRO_ENABLE = yes
VIA_ENABLE = yes
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
RGBLIGHT_ENABLE ?= yes


BOOTMAGIC_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
SLEEP_LED_ENABLE = no
BACKLIGHT_ENABLE = no
AUDIO_ENABLE = no
RGBLIGHT_ENABLE = no
EXTRAFLAGS+=-flto  # 如果固件太大在rule.mk 中添加EXTRAFLAGS+=-flto 