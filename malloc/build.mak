#######################################################################################################################
# VARIABLES
#######################################################################################################################
RELEASE ?= n
EMPTY :=

EXE_OUTPUT_FILE  := main.exe

LIST_OF_INCLUDES = include/

LIST_OF_SRC += main.c
LIST_OF_SRC += src/stdmalloc.c

ifneq ($(RELEASE),y)
LIST_OF_SRC      += src/debug/logger.c
LIST_OF_INCLUDES += include/debug
endif

#######################################################################################################################
# FUNCTIONS
#######################################################################################################################
str_eq = $(if $(subst $(1),$(EMPTY),$(2)),,true)

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