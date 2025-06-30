CC = cc
CFLAGS = -Wall -std=c99 -g
TARGET = zenovm
BUILDDIR = ../../build

# Include directories
INCLUDES = -I../transpiler -I.

# Source files
ZENOVM_SOURCES = zenovm.c zir.c cli.c
BACKEND_SOURCES = backends/backend.c backends/c_backend.c
TRANSPILER_SOURCES = ../transpiler/lexer.c ../transpiler/ast.c ../transpiler/parser.c

SOURCES = $(ZENOVM_SOURCES) $(BACKEND_SOURCES) $(TRANSPILER_SOURCES)

# Object files
OBJECTS = $(SOURCES:.c=.o)

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -f $(OBJECTS)
	rm -f $(BUILDDIR)/$(TARGET)

install: $(BUILDDIR)/$(TARGET)
	cp $(BUILDDIR)/$(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Development targets
test: $(BUILDDIR)/$(TARGET)
	./$(BUILDDIR)/$(TARGET) --help

debug: CFLAGS += -DDEBUG -O0
debug: $(BUILDDIR)/$(TARGET)

release: CFLAGS += -O2 -DNDEBUG
release: $(BUILDDIR)/$(TARGET)

.PHONY: all clean install uninstall test debug release