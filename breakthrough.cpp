#include "breakthrough.hpp"

namespace reasoner {
    int game_state::get_current_player(void) const {
        return current_player;
    }

    int game_state::get_player_score(int player_id) const {
        return variables[player_id-1];
    }

    void game_state::apply_move(const move& mv) {
        if (current_player == WHITE) {
            white ^= mv.mr;
            black &= ~mv.mr;
            if (white & 0xFF00000000000000) {
                winner = WHITE;
            }
            current_player = BLACK;
        }
        else {
            black ^= mv.mr;
            white &= ~mv.mr;
            if (black & 0xFF) {
                winner = BLACK;
            }
            current_player = WHITE;
        }
        empty = ~(white | black);
    }

    std::vector<move> game_state::get_all_moves(resettable_bitarray_stack& cache) {
        std::vector<move> result;
        result.reserve(100);
        get_all_moves(cache, result);
        return result;
    }

    void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves) {
        moves.clear();
        if (winner) {
            variables[winner-1] = 100;
            return;
        }
        if (current_player == WHITE) {
            uint64_t not_white = ~white;
            uint64_t movers = white & (empty >> 8);
            while (movers) {
                auto piece = msb(movers);
                moves.push_back(piece | piece << 8);
                movers ^= piece;
            }
            movers = white & maskLD & (not_white >> 7);
            while (movers) {
                auto piece = msb(movers);
                moves.push_back(piece | piece << 7);
                movers ^= piece;
            }
            movers = white & maskRD & (not_white >> 9);
            while (movers) {
                auto piece = msb(movers);
                moves.push_back(piece | piece << 9);
                movers ^= piece;
            }
            if (moves.empty()) {
                variables[0] = 0;
                variables[1] = 100;
            }
        }
        else {
            uint64_t not_black = ~black;
            uint64_t movers = black & (empty << 8);
            while (movers) {
                auto piece = msb(movers);
                moves.push_back(piece | piece >> 8);
                movers ^= piece;
            }
            movers = black & maskLD & (not_black << 9);
            while (movers) {
                auto piece = msb(movers);
                moves.push_back(piece | piece >> 9);
                movers ^= piece;
            }
            movers = black & maskRD & (not_black << 7);
            while (movers) {
                auto piece = msb(movers);
                moves.push_back(piece | piece >> 7);
                movers ^= piece;
            }
            if (moves.empty()) {
                variables[0] = 100;
                variables[1] = 0;
            }
        }
    }

    bool game_state::apply_any_move(resettable_bitarray_stack&) {
        return false;
    }

    int game_state::get_monotonicity_class(void) {
        return -1;
    }

    bool game_state::is_legal([[maybe_unused]] const move& m) const {
        return false;
    }

    inline uint64_t game_state::msb(const uint64_t& pieces) const {
        auto k = 63 - __builtin_clzll(pieces);
        return 1ull << k;
    }
}
