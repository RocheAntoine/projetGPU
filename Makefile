EXT = c
CXX = gcc
EXEC = main

CXXFLAGS = -O3 -fopenmp -pg
LDFLAGS = -fopenmp -lpng -pg


OBJDIR = ./
SRC = $(wildcard *.$(EXT))
OBJ = $(SRC:.$(EXT)=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))
 
all: $(EXEC)
 
$(EXEC): $(OBJ)
	@$(CXX) -o $@ $^ $(LDFLAGS)
 
$(OBJDIR)/%.o: %.$(EXT)
	@$(CXX) -o $@ -c $< $(CXXFLAGS)
 
clean:
	@rm -rf *.o
	@rm -f $(EXEC)