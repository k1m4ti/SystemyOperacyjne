
# rozszerzenia plików
.SUFFIXES: .c .cpp .o .x .h

# nazwa obecnego katalogu
DIR = $(notdir $(CURDIR))

# nazwa systemu operacyjnego
SYSNAME = $(shell uname -s)

#rdzenie nazw plików
NAME1 = program5
NAME2 = producer
NAME3 = consumer
FIFO = myFifo
SOURCE_FILE = source.txt
DESTINATION_FILE = consumer.txt
UP = 20
DOWN = 20

# Programy wykonawcze
EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x
EXEC3 = $(NAME3).x

# object files
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o

# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

# Kompilator i linker
CO = gcc
LD = $(CO)

# pliki obiektowe
$(OBJS1): $(NAME1).c
	$(CO) $(CFLAGS) -c $(NAME1).c

$(OBJS2): $(NAME2).c
	$(CO) $(CFLAGS) -c $(NAME2).c

$(OBJS3): $(NAME3).c
	$(CO) $(CFLAGS) -c $(NAME3).c

# programy wykonawcze
$(EXEC1): $(OBJS1)
	$(LD) -o $@ $(LFLAGS) $(OBJS1)

$(EXEC2): $(OBJS2)
	$(LD) -o $@ $(LFLAGS) $(OBJS2)

$(EXEC3): $(OBJS3)
	$(LD) -o $@ $(LFLAGS) $(OBJS3)

.PHONY: runA runB

# uruchomienie programu
runA: $(EXEC1) $(EXEC2) $(EXEC3)
	./$(EXEC1) ./$(EXEC2) ./$(EXEC3) $(EXEC2) $(EXEC3) $(FIFO) $(SOURCE_FILE) $(DESTINATION_FILE) $(UP) $(DOWN)
	diff -s $(SOURCE_FILE) $(DESTINATION_FILE)

runB: $(EXEC1) $(EXEC2) $(EXEC3)
	mkfifo -m 0666 $(FIFO)
	xterm -title $(NAME2) -e ./$(EXEC2) $(FIFO) $(SOURCE_FILE) $(UP) &
	xterm -title $(NAME3) -e ./$(EXEC3) $(FIFO) $(DESTINATION_FILE) $(DOWN)
	rm $(FIFO)
	diff -s $(SOURCE_FILE) $(DESTINATION_FILE)


.PHONY: clean tar

# czyszczenie katalogu
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out *.gch $(DESTINATION_FILE);

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )

