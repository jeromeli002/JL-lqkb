# MCU name
MCU = STM32F401

# Bootloader selection
BOOTLOADER = stm32-dfu
BOARD = BLACKPILL_STM32_F401

# BOOTLOADER = tinyuf2

# Build Options
#   comment out to disable the options.
#
# Disable unsupported hardware
AUDIO_SUPPORTED = no
BACKLIGHT_SUPPORTED = no
BOOTMAGIC_ENABLE = yes     # Enable Bootmagic Lite
KEYBOARD_SHARED_EP = yes    # Free up some extra endpoints - needed if console+mouse+extra
#MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes           # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
ENCODER_ENABLE = yes        # Enable rotary encoder support
AUDIO_ENABLE = no           # Audio output
# RGBLIGHT_ENABLE ?= yes
SPLIT_KEYBOARD = no
SEND_STRING_ENABLE = yes
VELOCIKEY_ENABLE = yes
LEADER_ENABLE = yes
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

# POINTING_DEVICE_ENABLE = yes
# POINTING_DEVICE_DRIVER = analog_joystick
JOYSTICK_TRIGGER_ENABLE = yes

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
# 可编程键
PROGRAMMABLE_BUTTON_ENABLE = yes