CXX = g++
CXXFLAGS = -IC:\Users\parsa\scoop\apps\sdl2\2.30.10\include
LDFLAGS = -LC:\Users\parsa\scoop\apps\sdl2\2.30.10\lib -lSDL2main -lSDL2

TARGET = laputa

SRC = src/main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	nu -c "rm -f laputa.exe"
