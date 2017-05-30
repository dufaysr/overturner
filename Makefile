CPP=g++
# CPP=mpicxx

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
$(LIBDIR)/Matrix.o: $(SRCDIR)/Matrix.hpp
$(LIBDIR)/AdvDiffProblem.o: $(SRCDIR)/AdvDiffProblem.hpp $(LIBDIR)/Utilities.o
$(LIBDIR)/Utilities.o:  $(SRCDIR)/Utilities.hpp $(SRCDIR)/workingdirectory.hpp
$(LIBDIR)/Solvers.o: $(SRCDIR)/Solvers.hpp $(LIBDIR)/AdvDiffProblem.o $(LIBDIR)/Particles2D.o $(LIBDIR)/Utilities.o
$(LIBDIR)/Estimator.o: $(SRCDIR)/Estimator.hpp $(LIBDIR)/Matrix.o $(LIBDIR)/Particles2D.o $(LIBDIR)/Utilities.o 
$(LIBDIR)/studyCases.o: $(SRCDIR)/studyCases.hpp $(LIBDIR)/Solvers.o $(LIBDIR)/Estimator.o
$(LIBDIR)/main.o:  $(LIBDIR)/studyCases.o $(SRCDIR)/workingdirectory.hpp


$(LIBDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS) $(GLLIBS)

print-%  : ; @echo $* = $($*)

open :
	subl out/out.txt

.PHONY: clean mrproper

clean:
	rm -rf $(LIBDIR)/*.o

mrproper: clean
	rm -rf $(BINDIR)/$(EXEC)
