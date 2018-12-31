SRC = $(shell ls src)
SRCC = $(filter %.c, $(SRC))
OBJ = $(addprefix obj/, $(SRCC:%.c=%.o))
PROBLEM = $(shell ls problem)
ANSWER = $(addprefix answer/, $(PROBLEM))
TARGET = ./typingML4

$(TARGET): $(OBJ)
	gcc -o $@ obj/main.o obj/read.o obj/write.o obj/derivation.o obj/free.o obj/copy.o obj/error.o obj/debug.o obj/cmp.o obj/sub.o

$(OBJ) : obj/%.o : src/%.c src/param.h
	mkdir -p obj
	gcc -c $< -o $@

$(ANSWER) : answer/% : problem/%
	$(TARGET) $< > $@

test : $(ANSWER)

tclean :
	rm answer/*

clean :
	rm -r obj typingML4
