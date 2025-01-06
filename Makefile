CXX = g++
CXXFLAGS = -Iinclude
LDFLAGS = -Llib -lSDL2main -lSDL2 -lSDL2_image

TARGET = laputa

SRC = src/main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

run:
	./laputa

