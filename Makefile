# Define the compiler
CXX = g++

# Define compiler flags
CXXFLAGS = -Iinclude `pkg-config --cflags sdl2 SDL2_image`

# Define libraries
LIBS = -lSDL2 -lSDL2_image `pkg-config --libs sdl2 SDL2_image`

# Define source files and target executable
SRCS = src/main.cpp
TARGET = game

# Rule to build the project
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Rule to run the game with the correct working directory
run: $(TARGET)
	ASSETS_PATH=assets ./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET) src/*.o

