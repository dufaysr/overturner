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

EXEC=overturner.exe

SRC= $(wildcard $(SRCDIR)/*.cpp)
OBJ= $(SRC:$(SRCDIR)/%.cpp=$(LIBDIR)/%.o)

all: $(BINDIR)/$(EXEC)

run: all
	./$(BINDIR)/$(EXEC) $(MODEL)

$(BINDIR)/$(EXEC): $(OBJ)
	$(CPP) -o $@ $^ $(LDFLAGS) $(GLLIBS)

$(LIBDIR)/Particles2D.o : $(SRCDIR)/Particles2D.hpp
$(LIBDIR)/Field.o: $(SRCDIR)/Field.hpp
$(LIBDIR)/Utilities.o:  $(SRCDIR)/Utilities.hpp
$(LIBDIR)/Solvers.o: $(SRCDIR)/Solvers.hpp $(LIBDIR)/Particles2D.o $(LIBDIR)/Utilities.o
$(LIBDIR)/Estimator.o: $(SRCDIR)/Estimator.hpp $(LIBDIR)/Field.o $(LIBDIR)/Particles2D.o $(LIBDIR)/Utilities.o 
$(LIBDIR)/studyCases.o: $(SRCDIR)/studyCases.hpp $(LIBDIR)/Solvers.o $(LIBDIR)/Estimator.o
$(LIBDIR)/main.o:  $(LIBDIR)/studyCases.o


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