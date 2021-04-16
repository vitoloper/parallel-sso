# Parallel SSO MPI application Makefile

CC = mpicc
OPT_CC = cc
CFLAGS = -O3 -Wall
LDLIBS = -lm
OBJFILES = utils.o init_positions.o of.o tc.o sso.o
TARGET = sso

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET)