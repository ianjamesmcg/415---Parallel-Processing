
CXX := g++

CPPFLAGS := -std=c++11 -O3 -fopenmp

CC := mpicc

CFLAGS := -std=gnu99 -O3

CODEBRK_SRCDIR := src/codebreaker
MPI_SRCDIR := src/mpi_sqrt

all:  mpi_sqrt encrypter codebreaker 
	@echo "Compilation complete."

mpi_sqrt:
	$(CC) -o mpi_sqrt.x ./$(MPI_SRCDIR)/mpi_sqrt.c $(CFLAGS)

encrypter:
	$(CC) -o encrypter.x ./$(CODEBRK_SRCDIR)/encrypter.c $(CFLAGS)


codebreaker:
	$(CC) -o codebreaker.x ./$(CODEBRK_SRCDIR)/codebreaker.c $(CFLAGS)

clean:
	-rm *.x $(CS_OFILES)
