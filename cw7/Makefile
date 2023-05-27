
# rozszerzenia plików
.SUFFIXES: .c .cpp .o .x .h

# nazwa obecnego katalogu
DIR = $(notdir $(CURDIR))

# nazwa systemu operacyjnego
SYSNAME = $(shell uname -s)

#rdzenie nazw plików
NAME1 = sem
NAME2 = smem
NAME3 = program7
NAME4 = producer
NAME5 = consumer
SHARED_MEMORY = smem
SEMAPHORE_PRODUCER = sem1
SEMAPHORE_CONSUMER = sem2
PRODUCER_FILE = producer.txt
CONSUMER_FILE = consumer.txt

# Programy wykonawcze
EXEC1s = $(NAME3)S.x
EXEC2s = $(NAME4)S.x
EXEC3s = $(NAME5)S.x

EXEC1d = $(NAME3)D.x
EXEC2d = $(NAME4)D.x
EXEC3d = $(NAME5)D.x

# object files
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o
OBJS4  = $(NAME4).o
OBJS5  = $(NAME5).o

# Pliki naglowkowe
HEADS1 = $(NAME1).h
HEADS2 = $(NAME2).h

# Nazwy bibliotek
LIB_NAME1 = $(NAME1)
LIB_NAME2 = $(NAME2)

# Katalog dla bibliotek
LIB_DIR = ./lib

# Biblioteki statyczne
LIB_STAT1 = lib$(LIB_NAME1).a
LIB_STAT2 = lib$(LIB_NAME2).a

# Biblioteki dynamiczne
ifeq ($(SYSNAME), Linux)
  LIB_SHAR1 = lib$(LIB_NAME1).so
  LIB_SHAR2 = lib$(LIB_NAME2).so
endif
# macOS - nazwa jadra to Darwin
ifeq ($(SYSNAME), Darwin)
  LIB_SHAR1 = lib$(LIB_NAME1).dylib
  LIB_SHAR2 = lib$(LIB_NAME2).dylib
endif


# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS1 = $(OBJS1)
LIB_OBJS2 = $(OBJS2)


# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=gnu99 -pedantic -O -fPIC -pthread -lrt
LFLAGS = -Wall -std=gnu99 -pedantic -O -pthread -lrt

# Kompilator i linker
CO = gcc
LD = $(CO)
# Program-archiwizer do tworzenia bibliotek statycznych
AR = ar
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow 
ARFLAGS = rsv


# pliki obiektowe
$(OBJS1): $(NAME1).c
	$(CO) $(CFLAGS) -c $(NAME1).c

$(OBJS2): $(NAME2).c
	$(CO) $(CFLAGS) -c $(NAME2).c

$(OBJS3): $(NAME3).c
	$(CO) $(CFLAGS) -c $(NAME3).c $(HEADS1) $(HEADS2)

$(OBJS4): $(NAME4).c
	$(CO) $(CFLAGS) -c $(NAME4).c $(HEADS1) $(HEADS2)

$(OBJS5): $(NAME5).c
	$(CO) $(CFLAGS) -c $(NAME5).c $(HEADS1) $(HEADS2)


# biblioteki statyczne
$(LIB_STAT1): $(LIB_OBJS1)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT1) $(LIB_DIR)

$(LIB_STAT2): $(LIB_OBJS2)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT2) $(LIB_DIR)


# Tworzenie biblioteki dzielonej (shared library)
$(LIB_SHAR1): $(LIB_OBJS1)
# Linux - opcja linkera: -shared
ifeq ($(SYSNAME), Linux)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS1)
endif
# macOS - opcje linkera: -dynamiclib,  -install_name @rpath/$(LIB_DIR)/$@
ifeq ($(SYSNAME), Darwin)
	$(LD) -dynamiclib -o $@ $(LFLAGS) $(LIB_OBJS1) -install_name @rpath/$(LIB_DIR)/$@
endif

$(LIB_SHAR2): $(LIB_OBJS2)
# Linux - opcja linkera: -shared
ifeq ($(SYSNAME), Linux)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS2)
endif
# macOS - opcje linkera: -dynamiclib,  -install_name @rpath/$(LIB_DIR)/$@
ifeq ($(SYSNAME), Darwin)
	$(LD) -dynamiclib -o $@ $(LFLAGS) $(LIB_OBJS2) -install_name @rpath/$(LIB_DIR)/$@
endif
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR1) $(LIB_SHAR2) $(LIB_DIR)


# programy wykonawcze
$(EXEC1s): $(OBJS3) $(LIB_STAT1) $(LIB_STAT2)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2)

$(EXEC1d): $(OBJS3) $(LIB_SHAR1) $(LIB_SHAR2)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2) $(LDLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2) $(LDLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif

$(EXEC2s): $(OBJS4) $(LIB_STAT1) $(LIB_STAT2)
	$(LD) -o $@ $(LFLAGS) $(OBJS4) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2)

$(EXEC2d): $(OBJS4) $(LIB_SHAR1) $(LIB_SHAR2)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(LFLAGS) $(OBJS4) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2) $(LDLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(LFLAGS) $(OBJS4) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2) $(LDLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif

$(EXEC3s): $(OBJS5) $(LIB_STAT1) $(LIB_STAT2)
	$(LD) -o $@ $(LFLAGS) $(OBJS5) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2)

$(EXEC3d): $(OBJS5) $(LIB_SHAR1) $(LIB_SHAR2)
ifeq ($(SYSNAME), Linux)
	$(LD) -o $@ $(LFLAGS) $(OBJS5) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2) $(LDLIBS) -Wl,-R $(LIB_DIR)
endif
# macOS: opcje -Xlinker -rpath -Xlinker $(CURDIR)
ifeq ($(SYSNAME), Darwin)
	$(LD) -o $@ $(LFLAGS) $(OBJS5) -L$(LIB_DIR) -l$(LIB_NAME1) -l$(LIB_NAME2) $(LDLIBS) -Xlinker -rpath -Xlinker $(CURDIR)
endif


.PHONY: runS runD

# uruchomienie programu
runS: $(EXEC1s) $(EXEC2s) $(EXEC3s)
	#rm -rf $(LIB_DIR)
	./$(EXEC1s) ./$(EXEC2s) ./$(EXEC3s) $(EXEC2s) $(EXEC3s) $(SHARED_MEMORY) $(SEMAPHORE_PRODUCER) $(SEMAPHORE_CONSUMER) $(PRODUCER_FILE) $(CONSUMER_FILE)
	diff -s $(PRODUCER_FILE) $(CONSUMER_FILE)

runD: $(EXEC1d) $(EXEC2d) $(EXEC3d)
	./$(EXEC1d) ./$(EXEC2d) ./$(EXEC3d) $(EXEC2d) $(EXEC3d) $(SHARED_MEMORY) $(SEMAPHORE_PRODUCER) $(SEMAPHORE_CONSUMER) $(PRODUCER_FILE) $(CONSUMER_FILE)
	diff -s $(PRODUCER_FILE) $(CONSUMER_FILE)


.PHONY: clean tar

# czyszczenie katalogu
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out *.gch; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )


