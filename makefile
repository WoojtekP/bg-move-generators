CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Ofast -march=native -std=c++17 -flto

GAMES = breakthrough connect4 english_draughts

all: $(GAMES)

# breakthrough
breakthrough: breakthrough.o benchmark_flatmc.cpp
	$(CC) -DBREAKTHROUGH $(CFLAGS) -o $@ $^

breakthrough.o: breakthrough.cpp breakthrough.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

# connect4
connect4: connect4.o benchmark_flatmc.cpp
	$(CC) -DCONNECT4 $(CFLAGS) -o $@ $^

connect4.o: connect4.cpp connect4.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

# english_draughts
english_draughts: english_draughts.o benchmark_flatmc.cpp
	$(CC) -DENGDRAUGHTS $(CFLAGS) -o $@ $^

english_draughts.o: english_draughts.cpp english_draughts.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMES) *.o