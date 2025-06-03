CXX = g++
CXXFLAGS = -Wall -g -Iheader -fopenmp
SRC_DIR = src

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/BMP.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,%.o,$(SRCS))  # Changed: Objects in root
EXEC = main  # Changed: Executable in root

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) *.o

.PHONY: all clean