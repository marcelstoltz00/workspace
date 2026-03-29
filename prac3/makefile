#add all the files that you need to the files makefile variable below as a space seperated list
files = Matrix.cpp Vector.cpp Square.cpp Triangle.cpp

# Homebrew paths for macOS
CFLAGS = -I/opt/homebrew/include -I/usr/local/include
LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib

main: main.cpp
	g++ -g $(CFLAGS) shader.cpp $(files) main.cpp $(LDFLAGS) -lglfw -lGLEW -framework OpenGL -o main

clean:
	rm -f *.o main

run:
	./main

all:
	make clean
	make
	make run