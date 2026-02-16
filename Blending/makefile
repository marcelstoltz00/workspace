#add all the files that you need to the files makefile variable below as a space seperated list


main: main.cpp glad.c
	g++ -g -I/opt/homebrew/include -I../../glad/include shader.cpp $(files) main.cpp glad.c -L/opt/homebrew/lib -lglfw3 -pthread -ldl -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreGraphics -o main

clean:
	rm -f *.o main

run:
	./main

all:
	make clean
	make
	make run