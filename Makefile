all:
	gcc -o piano main.c -lasound -lm

clean:
	rm -rf piano