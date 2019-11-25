#
# Compiler flags
#
CC     = gcc
CFLAGS = 
LIBS	= 
#
# Project files
#
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXE  = cLox

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -ftrapv -Wall -Wextra -Wfloat-equal -Wundef -Wunreachable-code -Wcast-qual -DDEBUG -DDEBUG_PRINT_CODE -DDEBUG_TRACE_EXECUTION

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -Ofast -DNDEBUG -Wall -Wextra -Wfloat-equal -Wundef -Wunreachable-code -Wcast-qual

.PHONY: all clean debug prep release remake run rund test

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^ $(LIBS)

$(DBGDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^ $(LIBS)

$(RELDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS) $(RELDIR)/*.o $(DBGDIR)/*.o
run:
	$(RELEXE)

rund:
	$(DBGEXE)

test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -v $(DBGEXE)
