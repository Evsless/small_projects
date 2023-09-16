#######################################################################################################################
# FUNCTIONS
#######################################################################################################################
str_eq = $(if $(subst $(1),$(EMPTY),$(2)),,true)

#######################################################################################################################
# VARIABLES
#######################################################################################################################
RELEASE ?= n
PLATFORM ?=
EMPTY :=

EXE_OUTPUT_FILE := $(if $(call str_eq,$(PLATFORM),linux),main,main.exe)

LIST_OF_INCLUDES = include/

LIST_OF_SRC += main.c
LIST_OF_SRC += src/stdmalloc.c
LIST_OF_SRC += src/stdstring.c

ifneq ($(RELEASE),y)
LIST_OF_SRC      += src/debug/logger.c
LIST_OF_INCLUDES += include/debug
endif

#######################################################################################################################
# TARGETS
#######################################################################################################################

PHONIES += build_all;
build_all:
	gcc $(LIST_OF_SRC) -w $(foreach INCLUDE_FOLDER,$(LIST_OF_INCLUDES),-I$(INCLUDE_FOLDER))\
	$(if $(call str_eq,$(RELEASE),y),-DRELEASE=y,$(EMPTY)) -o $(EXE_OUTPUT_FILE)

PHONIES += clean_all
clean_all:
	rm -rf main.exe

.PHONY: $(PHONIES)