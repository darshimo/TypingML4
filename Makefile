typingML4: obj/main.o obj/read.o obj/write.o obj/derivation.o obj/free.o obj/copy.o obj/error.o obj/debug.o obj/cmp.o obj/sub.o src/param.h
	gcc -o $@ obj/main.o obj/read.o obj/write.o obj/derivation.o obj/free.o obj/copy.o obj/error.o obj/debug.o obj/cmp.o obj/sub.o

obj/main.o obj/read.o obj/write.o obj/derivation.o obj/free.o obj/copy.o obj/error.o obj/debug.o obj/cmp.o obj/sub.o : obj/%.o : src/%.c
	mkdir -p obj
	gcc -c $< -o $@

clean :
	rm -r obj typingML4
