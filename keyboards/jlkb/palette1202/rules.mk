# MCU name
MCU = STM32F401

# Bootloader selection
BOOTLOADER = stm32-dfu
BOARD = BLACKPILL_STM32_F401

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE  = yes      # Mouse keys
EXTRAKEY_ENABLE  = yes      # Audio control and System control
CONSOLE_ENABLE   = yes      # Console for debug
COMMAND_ENABLE   = yes      # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE   = no     # Enable keyboard backlight functionality
RGBLIGHT_ENABLE    = no     # Enable keyboard RGB underglow
AUDIO_ENABLE       = no     # Audio output
ENCODER_ENABLE     = yes    # Enable support for rotary encoders
