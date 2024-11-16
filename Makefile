CXX = g++
CXXFLAGS = `pkg-config opencv4 --cflags`
LDLIBS = `pkg-config opencv4 --libs` -lglut -lGL -lGLU


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)


TARGET = main

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean tmpclean
clean: tmpclean
	rm -f $(OBJS) main
tmpclean:
	rm -f *~
