EXE_OUTPUT_FILE := main.exe

LIST_OF_INCLUDES = include/

LIST_OF_SRC += main.c
LIST_OF_SRC += src/stdmalloc.c

PHONIES += build_all;
build_all:
	gcc $(LIST_OF_SRC) -w -I $(LIST_OF_INCLUDES) -o $(EXE_OUTPUT_FILE)

PHONIES += clean_all
clean_all:
	rm -rf main.exe

.PHONY: $(PHONIES)