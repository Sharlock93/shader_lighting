HDR_DIR = header C:\dev\lib\include C:\dev\lib\include\Shar\headers
HEADER = $(addprefix -I,$(HDR_DIR))
LIB_DIR = C:\dev\lib\lib-glew\Release\x64 C:\dev\lib\lib-mingw C:\dev\lib\lib-sharo
LIB = $(addprefix -L, $(LIB_DIR))
LIB_NAMES = glfw3 glew32s opengl32 sharo gdi32
LIBRARY = $(addprefix -l,$(LIB_NAMES))
CCOPTION = Wall std=gnu++11 D__USE_MINGW_ANSI_STDIO=1 static-libgcc static-libstdc++ g
FLAGS = $(addprefix -,$(CCOPTION))
CC = g++
OBJ_DIR = obj
OBJ = grid.o main.o sh_shpere.o sh_cube.o sh_cone.o
BUILD_DIR = build
SRC_DIR = src

$(BUILD_DIR)\sphere.exe: $(addprefix $(OBJ_DIR)\,$(OBJ)) 
	 $(CC) $(FLAGS) $(addprefix $(OBJ_DIR)\,$(OBJ)) -o $(BUILD_DIR)\sphere.exe $(LIB) $(LIBRARY)

$(OBJ_DIR)\grid.o: $(SRC_DIR)\grid.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\grid.cpp -o $(OBJ_DIR)\grid.o $(HEADER)

$(OBJ_DIR)\main.o: $(SRC_DIR)\main.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\main.cpp -o $(OBJ_DIR)\main.o $(HEADER)

$(OBJ_DIR)\sh_shpere.o: $(SRC_DIR)\sh_shpere.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\sh_shpere.cpp -o $(OBJ_DIR)\sh_shpere.o $(HEADER)

$(OBJ_DIR)\sh_cube.o: $(SRC_DIR)\sh_cube.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\sh_cube.cpp -o $(OBJ_DIR)\sh_cube.o $(HEADER)

$(OBJ_DIR)\sh_cone.o: $(SRC_DIR)\sh_cone.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\sh_cone.cpp -o $(OBJ_DIR)\sh_cone.o $(HEADER)

clean: 
	del /Q $(OBJ_DIR)\*.o
	del /Q $(BUILD_DIR)\*.exe
