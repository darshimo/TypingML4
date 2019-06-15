SRC = $(shell ls src)
SRCC = $(filter %.c, $(SRC))
OBJ = $(addprefix obj/, $(SRCC:%.c=%.o))
PROBLEM = $(shell ls problem)
ANSWER = $(addprefix answer/, $(PROBLEM))
TARGET = ./typingML4

$(TARGET): $(OBJ)
	gcc -o $@ $(OBJ)

$(OBJ) : obj/%.o : src/%.c src/param.h
	mkdir -p obj
	gcc -c $< -o $@

$(ANSWER) : answer/% : problem/%
	mkdir -p answer
	$(TARGET) $< > $@

test : $(ANSWER)

clean :
	rm -rf obj typingML4 answer
