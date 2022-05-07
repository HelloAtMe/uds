#==============================================================================
#	Makefile for C in platform x86_64 Windows
#
#	Created : wcy
#   Version	: V1.1
#	Date   	: 08/17/2021
#	log		: 
#		
#==============================================================================

ifndef VERBOSE
#   @echo "Running quiet. Use VERBOSE=1 for noise."
  export MAKEFLAGS+=--no-print-directory -s
  export SILENCE=>NUL
endif


#------------------------------  PROJECT INFO  ------------------------------#
TARGET = main
PROJ_DIR = ./
TARGET_PATH	= $(PROJ_DIR)out
OBJECT_PATH = $(PROJ_DIR)obj
				

SOURCE_DIR = 	$(PROJ_DIR)src \
				$(PROJ_DIR)test


#------------------------------  COMPILER INFO  ------------------------------#
ifdef WIN32
COMPILER_PATH = C:/Users/jxyun/AppData/Local/Nuitka/Nuitka/gcc/x86_64/mingw64
CC = $(COMPILER_PATH)/bin/gcc.exe
LD = $(COMPILER_PATH)/bin/gcc.exe
GCC_INC = $(COMPILER_PATH)/include
TARGET_SUFFIX = .exe
else 
CC = /usr/bin/gcc
LD = /usr/bin/gcc
GCC_INC = /usr/bin/include
TARGET_SUFFIX = 
endif


#------------------------------ HEADER FILES  ------------------------------#
PROJ_INC = -I./src \
			-I./test \
			-I$(GCC_INC)


#------------------------------ COMPILE OPTIONS  ------------------------------#
CFLAGS	= $(PROJ_INC) \
		  -O0 \
		  -g \
		  -std=gnu11


ASFLAGS	= $(PROJ_INC) \
		  -O0 \
		  -g \
		  -std=gnu11


#------------------------------ LINK OPTIONS  ------------------------------#
ifdef (WIN32)
LDFLAGS = -Wl,-Map, $(TARGET_PATH)/$(TARGET).map
else
LDFLAGS = 
endif

#------------------------------ SOUREC FILES  ------------------------------#
vpath %.c  $(SOURCE_DIR)
vpath %.s  $(SOURCE_DIR)
vpath %.o  $(SOURCE_DIR)

OBJ_FILES = $(foreach gf, $(SOURCE_DIR), $(patsubst $(gf)/%.c,$(OBJECT_PATH)/%.o,$(wildcard $(gf)/*.c))) \
          $(foreach gf, $(SOURCE_DIR), $(patsubst $(gf)/%.s,$(OBJECT_PATH)/%.o,$(wildcard $(gf)/*.s)))


#------------------------------ Compiling  ------------------------------#
$(OBJECT_PATH)/%.o: %.c
	@echo 'Building file: $<'
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJECT_PATH)/%.o: %.s
	@echo 'Building file: $<'
	$(CC) $(ASFLAGS) -c -o $@ $<

$(TARGET): $(OBJ_FILES)
	@echo 'Linking files ...'
	$(LD) -o $(TARGET_PATH)/$(TARGET)$(TARGET_SUFFIX) $(LDFLAGS) $(OBJ_FILES)

.PHONY: all	
all:
	$(MAKE) c
	$(MAKE) b

b:
	$(MAKE) $(TARGET)
	@echo 'Finish to compile.'

c:
	@$(RM) $(OBJ_FILES) $(TARGET_PATH)/$(TARGET)$(TARGET_SUFFIX) $(TARGET_PATH)/$(TARGET).map