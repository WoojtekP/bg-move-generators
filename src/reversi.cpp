#include "reversi.hpp"

namespace reasoner {
    int game_state::get_piece(int cell_id) const {
        // 0 -black, 1 - empty, 2 - white
        const uint64_t cell_mask = 1ull << (cell_id - 1);
        if (pieces[0] & cell_mask) {
            return 2;
        }
        if (pieces[1] & cell_mask) {
            return 0;
        }
        return 1;
    }

    int game_state::get_variable_value(int variable_id) const {
        return variables[variable_id];
    }

    int game_state::get_current_player(void) const {
        return current_player;
    }

    int game_state::get_player_score(int player_id) const {
        return variables[player_id - 1];
    }

    void game_state::apply_move(const move& mv) {
        if (mv.mr == 64) {
            current_player ^= 0b11;
            return;
        }
        uint64_t move_mask = 1ull << mv.mr;
        auto curr_id = current_player - 1;
        auto opp_id = current_player & 1;
        pieces[curr_id] ^= move_mask;
        empty ^= move_mask;
        uint64_t flipped = 0;
        uint64_t &current = pieces[curr_id];
        uint64_t &opponent = pieces[opp_id];

        flipped = up(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= up(flipped) & opponent;
        if (up(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = down(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= down(flipped) & opponent;
        if (down(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = left(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= left(flipped) & opponent;
        if (left(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = right(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= right(flipped) & opponent;
        if (right(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = up_left(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= up_left(flipped) & opponent;
        if (up_left(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = up_right(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= up_right(flipped) & opponent;
        if (up_right(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = down_left(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= down_left(flipped) & opponent;
        if (down_left(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        flipped = down_right(move_mask) & opponent;
        for (int j = 0; j < 5; ++j)
            flipped |= down_right(flipped) & opponent;
        if (down_right(flipped) & current) {
            current ^= flipped;
            opponent ^= flipped;
        }

        current_player ^= 0b11;
    }

    void game_state::apply(const move& mv) {
        apply_move(mv);
    }

    void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves) {
        moves.clear();
        auto curr_id = current_player - 1;
        auto opp_id = current_player & 1;
        uint64_t current = pieces[curr_id];
        uint64_t opponent = pieces[opp_id];
        uint64_t result = get_movers(current, opponent);

        while (result) {
            auto piece = msb(result);
            moves.push_back(piece);
            result ^= 1ull << piece;
        }

        if (moves.empty()) {
            result = get_movers(opponent, current);
            if (result == 0) {
                auto black_pieces = __builtin_popcountll(pieces[0]);
                auto white_pieces = __builtin_popcountll(pieces[1]);
                if (black_pieces > white_pieces) {
                    variables[0] = 100;
                }
                else if(white_pieces > black_pieces) {
                    variables[1] = 100;
                }
                else {
                    variables[0] = 50;
                    variables[1] = 50;
                }
            }
            else {
                moves.push_back(64);
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

}
