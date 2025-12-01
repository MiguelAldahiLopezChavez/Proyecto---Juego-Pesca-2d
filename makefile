# Directorios de origen y destino
SRC_DIR := src
BIN_DIR := bin

SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d


# Archivos fuente y ejecutable principal
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
MAIN := $(SRC_DIR)/01_Mundo.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := $(BIN_DIR)/01_Mundo.exe

all: $(TARGET)

$(TARGET): $(SRCS) | $(BIN_DIR)
	g++ $(SRCS) -o $(TARGET) $(SFML) -Iinclude

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: $(TARGET)
	@powershell -NoProfile -Command "& '$(TARGET)'"

clean:
	rm -f $(BIN_DIR)/*.exe

.PHONY: all clean run