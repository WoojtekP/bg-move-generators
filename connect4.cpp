#include "connect4.hpp"

namespace reasoner {
    int game_state::get_current_player(void) const {
        return current_player;
    }

    int game_state::get_player_score(int player_id) const {
        return variables[player_id-1];
    }

    void game_state::apply_move(const move& m) {
        uint64_t &tokens = pieces[current_player-1];
        tokens |= 1ull << column_shift[m.mr];
        column_shift[m.mr] += 8;

        static constexpr int shift[] = {1, 7, 8, 9};

        uint64_t result = 0;
        for (int i = 0; i < 4; ++i) {
            uint64_t tokenstmp = tokens & (tokens >> 2*shift[i]);
            result |= tokenstmp & (tokenstmp >> shift[i]);
        }
        if (result) {
            variables[current_player - 1] = 100;
            variables[current_player & 1] = 0;
            current_player = 0;
            return;
        }
        current_player ^= 0b11;
    }

    void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves) {
        moves.clear();
        for (uint8_t i = 0; i < 7; ++i) {
            if (column_shift[i] <= 46) {
                moves.push_back(i);
            }
        }
    }

    bool game_state::apply_any_move(resettable_bitarray_stack&) {
        return false;
    }

    int game_state::get_monotonicity_class(void) {
        return 0;
    }

    bool game_state::is_legal([[maybe_unused]] const move& m) const {
        return column_shift[m.mr] <= 46;
    }

}
