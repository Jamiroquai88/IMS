CP=g++
CPFLAGS=-std=c++11 -pthread -pedantic -Wall -O3 -g
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

all: simulation

simulation: $(OBJ_FILES)
	$(CP) $(CPFLAGS) $^ -o $@ -lsimlib -lm

%.o:%.cpp
	$(CP) $(CPFLAGS) -c $< -o $@ -I/usr/local/include -L/usr/local/lib64 -lsimlib -lm
	
tar:
	tar -cvzf archive.tgz *
	
clean:
	$(RM) simulation $(OBJ_FILES) *.h.gch
