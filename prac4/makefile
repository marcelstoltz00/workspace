files = math/Matrix.cpp math/Vector.cpp geo/Square.cpp geo/Triangle.cpp geo/ShapeFactory.cpp scene/SceneManager.cpp scene/MeshNode.cpp

CFLAGS = -std=c++11 -I/opt/homebrew/include -I/usr/local/include -I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include -I/opt/homebrew/opt/libpng/include/libpng16
LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -L/opt/homebrew/opt/glew/lib -L/opt/homebrew/opt/glfw/lib -L/opt/homebrew/opt/libpng/lib

main: main.cpp
	clang++ -g $(CFLAGS) -Imath -Igeo shader.cpp $(files) main.cpp $(LDFLAGS) -lglfw -lGLEW -lpng16 -framework OpenGL -o main

clean:
	rm -f *.o main

run:
	./main

all:
	make clean
	make
	make run
