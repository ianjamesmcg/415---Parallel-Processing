
CC := gcc

CFLAGS := -std=gnu99 -Wall -Wextra -O3  -fopenmp

PI_SRCDIR := src/
PI_CFILES := $(filter-out ./$(PI_SRCDIR)/main.c,$(wildcard ./$(PI_SRCDIR)/*.c))
PI_OFILES := $(PI_CFILES:.c=.o)



LIB := -lm


all: pi
	@echo "Compilation complete."


pi: $(PI_OFILES)
	$(CC) -o pi.x ./$(PI_SRCDIR)/main.c $(PI_OFILES) $(CFLAGS) $(LIB)

$(PI_SRCDIR)/%.o: $(PI_SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LIB)






clean:
	-rm *.x $(PI_OFILES)
