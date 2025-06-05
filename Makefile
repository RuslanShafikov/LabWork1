CXX := g++
CXXFLAGS := -Iheader -Wall -Wextra -pedantic -std=c++17 -fopenmp
SRCDIR := src

MAIN_OBJS := $(SRCDIR)/main.o $(SRCDIR)/BMP.o
TEST_OBJS := $(SRCDIR)/test.o $(SRCDIR)/BMP.o

TEST_LIBS := -lgtest -lpthread -fopenmp

EXECUTABLES := main test

.PHONY: all clean

all: $(EXECUTABLES)

main: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -fopenmp

test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(TEST_LIBS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR)/main.o: header/BMP.h
$(SRCDIR)/test.o: header/BMP.h
$(SRCDIR)/BMP.o: header/BMP.h

clean:
	rm -f $(EXECUTABLES) $(SRCDIR)/*.o