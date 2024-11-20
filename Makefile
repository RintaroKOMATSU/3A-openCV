CXX = g++
CXXFLAGS = `pkg-config opencv4 --cflags` -O2
LDLIBS = `pkg-config opencv4 --libs` -lglut -lGL -lGLU -lX11 -lSDL2 -lSDL2_mixer


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)


TARGET = main

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean tmpclean
clean: tmpclean
	rm -f $(OBJS)

tmpclean:
	rm -f *~
