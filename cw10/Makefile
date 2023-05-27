
# rozszerzenia plików
.SUFFIXES: .c .cpp .o .x .h

# nazwa obecnego katalogu
DIR = $(notdir $(CURDIR))

# nazwa systemu operacyjnego
SYSNAME = $(shell uname -s)

#rdzenie nazw plików
NAME1 = thread
NAME2 = program10
THREADS = 3
SECTIONS = 3

# Programy wykonawcze
EXEC1  = $(NAME2).x


# object files
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o


# Pliki naglowkowe
HEADS1 = $(NAME1).h

# Nazwa biblioteki
LIB_NAME = $(NAME1)

# Katalog dla bibliotek
LIB_DIR = ./lib

# Biblioteka statyczna
LIB_STAT = lib$(LIB_NAME).a

# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS = $(OBJS1)


# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC -pthread
LFLAGS = -Wall -std=c99 -pedantic -O -pthread

# Kompilator i linker
CO = gcc
LD = $(CO)
# Program-archiwizer do tworzenia bibliotek statycznych
AR = ar
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow 
ARFLAGS = rsv


# reguła wzorcowa
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
%.o: %.c
	$(CO) $(CFLAGS) -c $<


# biblioteka statyczna
$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)


# programy wykonawcze
$(EXEC1): $(OBJS2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME)

.PHONY: run

# uruchomienie programu
run: $(EXEC1)
	clear
	./$(EXEC1) $(THREADS) $(SECTIONS)

.PHONY: clean tar

# czyszczenie katalogu
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )