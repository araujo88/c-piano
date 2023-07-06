all:
	gcc -Wall -Wpedantic -Wextra -o piano main.c -lasound -lm -pthread

clean:
	rm -rf piano