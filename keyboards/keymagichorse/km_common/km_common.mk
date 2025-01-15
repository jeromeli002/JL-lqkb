MAKEFLAGS= -j 10

ifeq ($(strip $(KM_DEBUG)), yes)
	OPT_DEFS += -DKM_DEBUG
	SRC+= km_common/rtt/SEGGER_RTT.c
	SRC+= km_common/rtt/SEGGER_RTT_printf.c
	VPATH += keyboards/keymagichorse/km_common/rtt
endif   

ifeq ($(strip $(KB_LPM_ENABLED)), yes)
    OPT_DEFS += -DKB_LPM_ENABLED
	SRC+= km_common/lpm_stm32f4.c
endif


VPATH += keyboards/keymagichorse/km_common/

