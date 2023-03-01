all:
	gcc pong.c -o pong -O3
clean:
	rm -rf pong
install:
	mkdir -p ~/bin
	cp pong ~/bin/
