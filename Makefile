CXX:=g++
TARGETS:=test
SOURCE:=$(wildcard *.cpp)
OBJECTS:=$(patsubst %.cpp,%.o,$(SOURCE))

%.o : %.cpp
	$(CXX) -c $(SOURCE)

$(TARGETS) : $(OBJECTS)
	$(CXX) -o $(TARGETS) $(OBJECTS)
