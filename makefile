CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Ofast -march=native -std=c++17 -flto

GAMES = breakthrough

all: breakthrough

breakthrough: breakthrough.o benchmark_flatmc.cpp
	$(CC) -DBREAKTHROUGH $(CFLAGS) -o $@ $^

breakthrough.o: breakthrough.cpp breakthrough.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMES) *.o