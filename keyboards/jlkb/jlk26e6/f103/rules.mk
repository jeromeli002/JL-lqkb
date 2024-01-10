# Build Options
#   change yes to no to disable
#
# MCU name
MCU = STM32F103
# MCU_LDSCRIPT = STM32F103xB_uf2
MCU_LDSCRIPT = STM32F103xB_stm32duino_bootloader
# MCU_LDSCRIPT = STM32F103xB
OPT_DEFS += -DBOOTLOADER_STM32DUINO
# FIRMWARE_FORMAT = uf2  # 生成uf2格式固件
BOARD = STM32_F103_STM32DUINO
BOOTLOADER_TYPE = stm32duino
# Bootloader selection
# BOOTLOADER = stm32duino
# BOOTLOADER = custom

# Disable unsupported hardware
AUDIO_SUPPORTED = no
BACKLIGHT_SUPPORTED = no
BOOTMAGIC_ENABLE = yes     # Enable Bootmagic Lite
KEYBOARD_SHARED_EP = yes    # Free up some extra endpoints - needed if console+mouse+extra
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = yes         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes           # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
ENCODER_ENABLE = yes        # Enable rotary encoder support
AUDIO_ENABLE = no           # Audio output
#AUDIO_DRIVER = pwm_hardware
RGBLIGHT_ENABLE ?= yes
SPLIT_KEYBOARD = no
SERIAL_DRIVER = usart
# HAPTIC_ENABLE = yes //蜂鸣器/电磁阀
# HAPTIC_DRIVER += SOLENOID
# POINTING_DEVICE_ENABLE = yes               #摇杆模拟指点杆
# POINTING_DEVICE_DRIVER = analog_joystick   #摇杆模拟指点杆
# JOYSTICK_TRIGGER_ENABLE = yes   #摇杆映射按键
DYNAMIC_MACRO_ENABLE = yes  # 启用动态宏

OLED_ENABLE = yes
OLED_DRIVER = SSD1306    # Enable the OLED Driver
EXTRAFLAGS+=-flto  # 如果固件太大在rule.mk 中添加EXTRAFLAGS+=-flto 

OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
# SRC += uf2_boot.c  # UF2格式固件