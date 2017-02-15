SRCF = src/
BINF = bin/
FLAGS = -O3 -g3 -std=c++11 -Wno-unused-result

SEQS = $(SRCF)sequential
PARS = $(SRCF)parallel

SEQB = $(BINF)sequential
PARB = $(BINF)parallel

all:
	g++ $(FLAGS) $(SEQS)/life.cc -o $(SEQB)/life.o
	g++ $(FLAGS) $(PARS)/life.cc -o $(PARB)/life.o -lpthread