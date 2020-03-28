CXX = g++
CPPFLAGS = -DRBG_RANDOM_GENERATOR=$(RANDGEN) -Wall -Wextra -Wpedantic -Ofast -march=native -std=c++17 -flto

RANDGEN := 0

GAMES = breakthrough connect4 english_draughts english_draughts_split reversi

all: $(GAMES)

# breakthrough
breakthrough: breakthrough.o benchmark_flatmc.cpp
	$(CXX) -DBREAKTHROUGH $(CPPFLAGS) -o $@ $^

# connect4
connect4: connect4.o benchmark_flatmc.cpp
	$(CXX) -DCONNECT4 $(CPPFLAGS) -o $@ $^

# english_draughts
english_draughts: english_draughts.o benchmark_flatmc.cpp
	$(CXX) -DENGDRAUGHTS $(CPPFLAGS) -o $@ $^

# english_draughts (split)
english_draughts_split: english_draughts_split.o benchmark_flatmc.cpp
	$(CXX) -DENGDRAUGHTSSPLIT $(CPPFLAGS) -o $@ $^

# reversi
reversi: reversi.o benchmark_flatmc.cpp
	$(CXX) -DREVERSI $(CPPFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMES) *.o