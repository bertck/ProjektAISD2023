CC = g++
CFLAGS = -Wall -std=c++11

NAME1 = main

EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o

%.o : %.cpp 
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all
all: $(EXEC1)

$(EXEC1): $(OBJS1)
	$(CC) -o $@.out $(CFLAGS) $^

.PHONY : clean
clean :
	rm -r $(TARGET) *.o

run:
	./main.out