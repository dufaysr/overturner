CPP=g++

CPPFLAGS=-Wall -O3
LDFLAGS=
GLLIBS = -std=gnu++11

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    GLLIBS += -lm
endif

SRCDIR = src
HEADDIR = src
LIBDIR = lib
BINDIR = bin

EXEC=overturner

SRC= $(wildcard $(SRCDIR)/*.cpp)
OBJ= $(SRC:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)

all: $(BINDIR)/$(EXEC)

run: all
	./$(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CPP) -o $@ $^ $(LDFLAGS) $(GLLIBS)

$(LIBDIR)/Particles2D.o : $(SRCDIR)/Particles2D.hpp 
$(LIBDIR)/Solvers.o: $(SRCDIR)/Particles2D.hpp $(SRCDIR)/Particles2D.cpp $(SRCDIR)/Solvers.hpp $(SRCDIR)/Utilities.hpp $(SRCDIR)/Utilities.cpp
$(LIBDIR)/Utilities.o:  $(SRCDIR)/Utilities.hpp
$(LIBDIR)/Field.o: $(SRCDIR)/Field.hpp
$(LIBDIR)/Estimator.o: $(SRCDIR)/Estimator.hpp $(SRCDIR)/Field.hpp $(SRCDIR)/Field.cpp $(SRCDIR)/Particles2D.hpp $(SRCDIR)/Particles2D.cpp 
$(LIBDIR)/main.o:  $(SRCDIR)/Utilities.hpp

$(LIBDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS) $(GLLIBS)

print-%  : ; @echo $* = $($*)

open :
	subl out/out.txt

.PHONY: clean mrproper

clean:
	rm -rf $(LIBDIR)/*.o
	rm -rf out/*

mrproper: clean
	rm -rf $(BINDIR)/$(EXEC)