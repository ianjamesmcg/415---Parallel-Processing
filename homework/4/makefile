
CC := gcc

CFLAGS := -std=gnu99 -Wall -Wextra -Wpedantic -O3 -fopenmp



CS_SRCDIR := src/summercamp
CS_CFILES := $(filter-out ./$(CS_SRCDIR)/main.c,$(wildcard ./$(CS_SRCDIR)/*.c))
CS_OFILES := $(CS_CFILES:.c=.o)

LIB := -lm


all: summercamp
	@echo "Compilation complete."




summercamp: $(CS_OFILES)
	$(CC) -o summercamp.x ./$(CS_SRCDIR)/main.c $(CS_OFILES) $(CFLAGS) $(LIB)

$(CS_SRCDIR)/%.o: $(CS_SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LIB)


clean:
	-rm *.x  $(CS_OFILES)
