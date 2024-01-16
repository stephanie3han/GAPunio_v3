# User Test
#------------------------------------
APP              = test
# App sources
APP_SRCS         = gapUART.c
# App includes
APP_INC	         +=
# Compiler flags
APP_CFLAGS       += -lm -Wall
# Linker flags
APP_LDFLAGS      =

CC= gcc

# Custom linker
APP_LINK_SCRIPT  =

gapUART.o: gapUART.c
	$(CC) $(APP_CFLAGS) -c gapUART.c -o gapUART.o $@ $^ $(APP_LDFLAGS)

include $(RULES_DIR)/pmsis_rules.mk
