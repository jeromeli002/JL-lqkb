# MCU name
MCU = STM32F103
BOOTLOADER = stm32duino

# MCU_LDSCRIPT = STM32F103xB_uf2
# FIRMWARE_FORMAT = uf2
# BOARD = STM32_F103_STM32DUINO

# Bootloader selection
# BOOTLOADER = custom


# Build Options
#   comment out to disable the options.
#
# Disable unsupported hardware
AUDIO_SUPPORTED = no
BACKLIGHT_SUPPORTED = no
BOOTMAGIC_ENABLE = yes     # Enable Bootmagic Lite
KEYBOARD_SHARED_EP = yes    # Free up some extra endpoints - needed if console+mouse+extra
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = yes           # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
ENCODER_ENABLE = yes        # 启用旋钮
# ENCODER_MAP_ENABLE = yes    # 旋钮映射
AUDIO_ENABLE = no           # Audio output
# RGBLIGHT_ENABLE ?= yes
SPLIT_KEYBOARD = no
# SERIAL_DRIVER = no
# RGB_MATRIX_ENABLE = yes
# RGB_MATRIX_DRIVER = WS2812
# WS2812_DRIVER = pwm
# WPM_ENABLE = yes		 # Show APM.
# OPENRGB_ENABLE = yes
# MIDI_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes  # 启用动态宏
KEY_LOCK_ENABLE = yes   #启用锁定键

# POINTING_DEVICE_ENABLE = yes               #摇杆模拟指点杆
# POINTING_DEVICE_DRIVER = analog_joystick   #摇杆模拟指点杆
JOYSTICK_TRIGGER_ENABLE = yes   #摇杆映射按键
# JOYSTICK_ENABLE = yes

# Enter lower-power sleep mode when on the ChibiOS idle thread
# OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
# PROGRAMMABLE_BUTTON_ENABLE = yes

# OLED屏幕开关
#OLED_ENABLE = yes
#OLED_DRIVER = ssd1306
#OLED_TRANSPORT = i2c

EXTRAFLAGS+=-flto  # 如果固件太大在rule.mk 中添加EXTRAFLAGS+=-flto 
# SRC += uf2_boot.c
