CXX = g++
CXXFLAGS = `pkg-config opencv4 --cflags` -O2 -Iinclude
LDLIBS = `pkg-config opencv4 --libs` -lglut -lGL -lGLU -lX11 -lSDL2 -lSDL2_mixer 


SRCS = $(wildcard ./src/*.cpp)
OBJS = $(SRCS:%.cpp=%.o)


TARGET = ./bin/main

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean tmpclean
clean: tmpclean
	rm -f $(OBJS) $(TARGET)

tmpclean:
	rm -f *~
