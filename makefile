CXX = g++
FLAGS = -Wall -Wextra -Wpedantic -Ofast -march=native -std=c++17 -flto

ifdef fastrand
	CPPFLAGS = -DFASTRAND $(FLAGS)
else
	CPPFLAGS = $(FLAGS)
endif

GAMES = breakthrough connect4 english_draughts reversi

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

# reversi
reversi: reversi.o benchmark_flatmc.cpp
	$(CXX) -DREVERSI $(CPPFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(GAMES) *.o