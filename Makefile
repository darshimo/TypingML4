CC = gcc
CFLAGS = -Wall
SRC = $(shell ls src)
SRCC = $(filter %.c, $(SRC))
OBJ = $(addprefix obj/, $(SRCC:%.c=%.o))
PROBLEM = $(shell ls problem)
ANSWER = $(addprefix answer/, $(PROBLEM))
TARGET = ./typingML4

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(OBJ) : obj/%.o : src/%.c src/param.h
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

$(ANSWER) : answer/% : problem/%
	mkdir -p answer
	$(TARGET) $< > $@

test : all $(ANSWER)

clean :
	rm -rf $(TARGET) obj answer
