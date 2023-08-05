PROJ = qwe
CC = gcc
FILE = ./src/main.c
EXE = ./dist/$(PROJ)
CFLAGS = -Wall

all: $(PROJ)
	$(EXE)

$(PROJ): $(FILE)
	 $(CC) -o $(EXE) $(FILE) $(CFLAGS)

clean:
	rm ./dist/*

