CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

main.exe: $(OBJ_FILES)
	g++ -std=c++11 -Wall -o $@ $^

obj/%.o: src/%.cpp
	g++ -std=c++11 $(CC_FLAGS) -c -o $@ $<