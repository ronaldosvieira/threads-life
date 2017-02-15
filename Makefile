SRCF = src/
BINF = bin/
FLAGS = -O3 -g3 -std=c++11

SEQS = $(SRCF)sequential
PARS = $(SRCF)parallel

SEQB = $(BINF)sequential
PARB = $(BINF)parallel

all:
	g++ $(FLAGS) $(SEQS)/life.cc -o $(SEQB)/life
	g++ $(FLAGS) $(PARS)/life.cc -o $(PARB)/life -lpthread