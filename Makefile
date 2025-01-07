CXX = g++
CXXFLAGS = -IC:\Users\parsa\scoop\apps\sdl2\2.30.11\include -IC:\Users\parsa\scoop\apps\sdl2-image\2.8.4\include
LDFLAGS = -LC:\Users\parsa\scoop\apps\sdl2\2.30.11\lib -lSDL2main -lSDL2 -LC:\Users\parsa\scoop\apps\sdl2-image\2.8.4\lib -lSDL2_image

TARGET = laputa

SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

run:
	./laputa.exe

clean:
	nu -c "rm -f laputa.exe"
