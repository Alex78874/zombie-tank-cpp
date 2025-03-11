COMPILER = clang++
SOURCE_LIBS = -Ilib/
OSX_OPT = -std=c++17 -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
OSX_RAYLIB = $(shell pkg-config --libs --cflags raylib)
OSX_OUT = -o "bin/build_osx"
CFILES = src/*.cpp src/scenes/*.cpp

build_osx:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(OSX_OUT) $(OSX_OPT) $(OSX_RAYLIB)
	./bin/build_osx