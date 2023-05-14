.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)

NAME1 = programA
NAME2 = producer
NAME3 = consumer

NAME4 = producer.txt
NAME5 = consumer.txt

NAME6 = myfifo

EXEC1 = $(NAME1).x
EXEC2 = $(NAME2).x
EXEC3 = $(NAME3).x

OBJS1 = $(NAME1).o
OBJS2 = $(NAME2).o
OBJS3 = $(NAME3).o

CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

LDLIBS = -lm
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

$(EXEC1): $(OBJS1)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC2): $(OBJS2)
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC3): $(OBJS3)
	$(LD) -o $@ $(LFLAGS) $^

.PHONY: runA runB

runA: $(EXEC1) $(EXEC2) $(EXEC3)
	./$(EXEC1) $(NAME4) $(NAME5) $(NAME6) $(EXEC2) $(EXEC3)
	rm -f $(NAME6)
	diff $(NAME4) $(NAME5)

runB: $(EXEC2) $(EXEC3)
	mkfifo -m 0666 $(NAME6)
	xterm -hold -title $(NAME2) -e ./$(EXEC2) $(NAME6) $(NAME4) &
	xterm -hold -title $(NAME3) -e ./$(EXEC3) $(NAME6) $(NAME5)
	rm $(NAME6)
	diff $(NAME4) $(NAME5)

.PHONY: clean tar
clean:
	rm -f *.o *~ *.a *.so *.x core core* a.out $(NAME5) $(NAME6)

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
