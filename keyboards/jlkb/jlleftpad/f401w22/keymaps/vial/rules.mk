VIA_ENABLE = yes
VIAL_ENABLE = yes
QMK_SETTINGS = yes
VIAL_INSECURE = yes
LTO_ENABLE = yes
RADIAL_CONTROLLER_ENABLE = yes
VIALRGB_ENABLE = yes

RGB_MATRIX_CUSTOM_USER = yes  # 开启用户自定义特效支持

# 是否使能低功耗
KB_LPM_ENABLED = no
KB_LPM_DRIVER = lpm_stm32f4
# 是否使能QMK端读取电池电压
KB_CHECK_BATTERY_ENABLED = yes
# 开启键盘层DEBUG  这里是用RTT输出日记的
KB_DEBUG = no


include keyboards/jlkb/kb_common/kb_common.mk