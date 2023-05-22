
# rozszerzenia plików
.SUFFIXES: .c .o .x .h

# nazwa obecnego katalogu
DIR = $(notdir $(CURDIR))

# nazwa systemu operacyjnego
SYSNAME = $(shell uname -s)

#rdzenie nazw plików
SERWER = server
BIBLIOTEKA = queue
KLIENT = client

# Programy wykonawcze
EXEC1 = $(SERWER).x
EXEC2 = $(KLIENT).x

# object files
OBJS1  = $(BIBLIOTEKA).o
OBJS2  = $(SERWER).o
OBJS3  = $(KLIENT).o

# Pliki naglowkowe
HEADS1 = $(BIBLIOTEKA).h

# Nazwy bibliotek
LIB_NAME1 = $(BIBLIOTEKA)

# Katalog dla bibliotek
LIB_DIR = ./lib

# Biblioteki statyczne
LIB_STAT1 = lib$(LIB_NAME1).a

# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS1 = $(OBJS1)


# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

# Kompilator i linker
CO = gcc
LD = $(CO)
# Program-archiwizer do tworzenia bibliotek statycznych
AR = ar
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow
ARFLAGS = rsv


# pliki obiektowe
$(OBJS1): $(BIBLIOTEKA).c
	$(CO) $(CFLAGS) -c $(BIBLIOTEKA).c

$(OBJS2): $(SERWER).c
	$(CO) $(CFLAGS) -c $(SERWER).c $(HEADS1)

$(OBJS3): $(KLIENT).c
	$(CO) $(CFLAGS) -c $(KLIENT).c $(HEADS1)

# biblioteki statyczne
$(LIB_STAT1): $(LIB_OBJS1)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT1) $(LIB_DIR)


# programy wykonawcze
$(EXEC1): $(OBJS2) $(LIB_STAT1)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME1)

$(EXEC2): $(OBJS3) $(LIB_STAT1)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME1)

.PHONY: run

# uruchomienie programu
run: $(EXEC1) $(EXEC2)
	xterm -hold -title $(SERWER) -bg red -e ./$(EXEC1) &
	xterm -hold -title $(KLIENT)1 -bg green -e ./$(EXEC2) &
	xterm -hold -title $(KLIENT)2 -bg green -e ./$(EXEC2)

.PHONY: clean tar

# czyszczenie katalogu
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out *.gch; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
