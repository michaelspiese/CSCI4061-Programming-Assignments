CC=gcc
CFLAGS=-g

SRCDIR=src
INCLDIR=include
LIBDIR=lib

master: $(SRCDIR)/master.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o childProgram
	$(CC) $(CFLAGS) -I$(INCLDIR) $(SRCDIR)/master.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o -o master

childProgram: $(SRCDIR)/childprogram.c $(LIBDIR)/utils.o
	$(CC) $(CFLAGS) -I$(INCLDIR) $(SRCDIR)/childprogram.c $(LIBDIR)/utils.o $(LIBDIR)/myutils.o -o childProgram

$(LIBDIR)/myutils.o: $(SRCDIR)/myutils.c
	$(CC) $(CFLAGS) -I$(INCLDIR) -c $(SRCDIR)/myutils.c -o $(LIBDIR)/myutils.o

.PHONY: clean run1 run2 run3 run4 run5 run6 run7 run8 run9

run1:
	./master input1.file

run2:
	./master input2.file

run3:
	./master input3.file

run4:
	./master input4.file

run5:
	./master input5.file

run6:
	./master input6.file

run7:
	./master input7.file

run8:
	./master input8.file

run9:
	./master input9.file

clean:
	rm lib/myutils.o master childProgram
	rm -rf output
	rm -rf *.dSYM