CC = cc
CFLAGS = -Wall -std=c99
TARGET = zeno
SRCDIR = .
BUILDDIR = ../../build
SOURCES = $(SRCDIR)/lexer.c $(SRCDIR)/ast.c $(SRCDIR)/parser.c $(SRCDIR)/codegen.c $(SRCDIR)/zenoscript.c $(SRCDIR)/cli.c

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(SOURCES) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$(TARGET) $(SOURCES)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -f $(BUILDDIR)/$(TARGET)

install: $(BUILDDIR)/$(TARGET)
	cp $(BUILDDIR)/$(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean install uninstall