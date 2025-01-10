ifeq ($(strip $(JOYSTICK_KEY_ENABLED)), yes)
 	OPT_DEFS += -DJOYSTICK_KEY_ENABLED
	SRC+= jllab/joystick_key.c
endif 
