# Define the compiler
CXX = g++

# Define compiler flags
CXXFLAGS = -Iinclude `pkg-config --cflags sdl2 SDL2_image`

# Define libraries
LIBS = -lSDL2 -lSDL2_image `pkg-config --libs sdl2 SDL2_image`

# Define source files and object files
SRCS = src/game.cpp src/main.cpp src/player.cpp src/texture_manager.cpp
OBJS = $(SRCS:src/%.cpp=src/%.o)

# Define target executable
TARGET = laputa

# Default rule to build the project
all: $(TARGET)

# Rule to link the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile source files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the game with the correct working directory
run: $(TARGET)
	ASSETS_PATH=assets ./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET) src/*.o
