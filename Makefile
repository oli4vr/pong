all:
	gcc errnostr.c -c -o errnostr.o -O3
	gcc pong.c -o pong -O3 errnostr.o
clean:
	rm -rf pong *.o
install:
	mkdir -p ~/bin
	cp pong ~/bin/
