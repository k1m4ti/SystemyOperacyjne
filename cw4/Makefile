
# rozszerzenia plików
.SUFFIXES: .c .cpp .o .x .h

# nazwa obecnego katalogu
DIR = $(notdir $(CURDIR))

# nazwa systemu operacyjnego
SYSNAME = $(shell uname -s)

#rdzenie nazw plików
NAME1 = program4
SOURCE_FILE = source.txt
DESTINATION_FILE = consumer.txt
UPLOAD = 20
DOWNLOAD = 20

# Programy wykonawcze
EXEC1 = $(NAME1).x

# object files
OBJS1  = $(NAME1).o

# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

# Kompilator i linker
CO = gcc
LD = $(CO)

# pliki obiektowe
$(OBJS1): $(NAME1).c
	$(CO) $(CFLAGS) -c $(NAME1).c

# programy wykonawcze
$(EXEC1): $(OBJS1)
	$(LD) -o $@ $(LFLAGS) $(OBJS1)

.PHONY: run

# uruchomienie programu
run: $(EXEC1)
	./$(EXEC1) $(SOURCE_FILE) $(DESTINATION_FILE) $(DOWNLOAD) $(UPLOAD)
	diff -s $(SOURCE_FILE) $(DESTINATION_FILE)

.PHONY: clean tar

# czyszczenie katalogu
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out *.gch $(DESTINATION_FILE);

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
