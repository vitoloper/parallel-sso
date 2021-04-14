# Parallel SSO MPI application Makefile

CC = mpicc
OPT_CC = cc
CFLAGS = -O3 -Wall
LDLIBS = -lm
OBJFILES = print_matrix.o init_positions.o sso.o
TARGET = sso

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET)