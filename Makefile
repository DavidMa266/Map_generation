CC              = g++ -std=c++11
CPP_FILES       = $(wildcard src/*.cpp)
OBJ_FILES       = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
GLEW_LOC        = lib\Release\Win32
GLEW_FLAGS      = -L$(GLEW_LOC) -lglew32s
FREEGLUT_STATIC = -D FREEGLUT_STATIC -Iinclude\GL
FREEGLUT_FLAGS  = -Llib -lfreeglut_static

main.exe: $(OBJ_FILES)
	$(CC) -Wall -o $@ $^ $(GLEW_FLAGS) $(FREEGLUT_FLAGS) -lopengl32 -lwinmm -lgdi32  
obj/%.o: src/%.cpp
	$(CC) $(FREEGLUT_STATIC) -c -o $@ $<