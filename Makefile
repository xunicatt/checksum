PROJECT=main
CC=gcc -std=c11
FLAGS=-Wall -Werror -Wextra -pedantic
SRCDIR=checksum
INCDIR=checksum

SRC=$(SRCDIR)/checksum.c main.c

all: $(PROJECT) run
.PHONY: run clean

$(PROJECT): $(SRC)
	$(CC) $(FALGS) -o $@ $^ -I $(INCDIR)

run:
	./$(PROJECT)

clean:
	rm -rf $(PROJECT)
