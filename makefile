RANDGEN := 0

CXX = g++
CPPFLAGS = -DRBG_RANDOM_GENERATOR=$(RANDGEN) -Wall -Wextra -Wpedantic -Ofast -march=native -std=c++17 -flto

GAMES = breakthrough connect4 english_draughts english_draughts_split reversi
BENCHMARK = src/tests/benchmark_flatmc.cpp

all: $(GAMES)

# breakthrough
breakthrough: breakthrough.o $(BENCHMARK)
	$(CXX) -DBREAKTHROUGH $(CPPFLAGS) -o $@ $^

# connect4
connect4: connect4.o $(BENCHMARK)
	$(CXX) -DCONNECT4 $(CPPFLAGS) -o $@ $^

# english_draughts
english_draughts: english_draughts.o $(BENCHMARK)
	$(CXX) -DENGDRAUGHTS $(CPPFLAGS) -o $@ $^

# english_draughts (split)
english_draughts_split: english_draughts_split.o $(BENCHMARK)
	$(CXX) -DENGDRAUGHTSSPLIT $(CPPFLAGS) -o $@ $^

# reversi
reversi: reversi.o $(BENCHMARK)
	$(CXX) -DREVERSI $(CPPFLAGS) -o $@ $^

%.o: src/%.cpp src/%.hpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMES) *.o