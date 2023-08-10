LIST_OF_INCLUDES = include/


build_all:
	gcc main.c -I $(LIST_OF_INCLUDES) -o main.exe

.PHONY: test