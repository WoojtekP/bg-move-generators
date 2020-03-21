CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Ofast -march=native -std=c++17 -flto

GAMES = breakthrough connect4 english_draughts

all: $(GAMES)

# breakthrough
breakthrough: breakthrough.o benchmark_flatmc.cpp
	$(CC) -DBREAKTHROUGH $(CFLAGS) -o $@ $^

# connect4
connect4: connect4.o benchmark_flatmc.cpp
	$(CC) -DCONNECT4 $(CFLAGS) -o $@ $^

# english_draughts
english_draughts: english_draughts.o benchmark_flatmc.cpp
	$(CC) -DENGDRAUGHTS $(CFLAGS) -o $@ $^


%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMES) *.o