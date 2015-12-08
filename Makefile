CP=g++
CPFLAGS=-std=c++11 -pthread -pedantic -Wall -O3 -g
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

all: simulation

simulation: $(OBJ_FILES)
	$(CP) $(CPFLAGS) $^ -o $@ -lsimlib -lm

%.o:%.cpp
	$(CP) $(CPFLAGS) -c $< -o $@ -I/usr/local/include -L/usr/local/lib64 -lsimlib -lm
	
run:	
	make && ./simulation

zip:
	zip 06_xprofa00_xricht21.zip *.cpp *.h Makefile README schedule.conf add_40_trains.conf dokumentace.pdf
	
clean:
	$(RM) simulation $(OBJ_FILES) *.h.gch
