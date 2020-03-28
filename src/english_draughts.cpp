#include "english_draughts.hpp"

namespace reasoner {
    int game_state::get_current_player(void) const {
        return current_player;
    }

    int game_state::get_player_score(int player_id) const {
        return variables[player_id-1];
    }

    void game_state::apply_move(const move& mv) {
        uint32_t &current = pieces[current_player - 1].first;
        uint32_t &opponent = pieces[current_player & 1].first;
        if ((mv.mr & current & kings) && ((mv.mr & opponent) == 0)) {
            kings_moves++;
        }
        else {
            kings_moves = 0;
        }
        uint32_t old = current;
        current ^= mv.mr & ~opponent;
        opponent &= ~mv.mr;
        if (mv.mr & kings & old)
            kings ^= mv.mr & current;
        kings |= mv.mr & current & pieces[current_player - 1].second;
        kings &= current | opponent;
        empty = ~(current | opponent);
        current_player ^= 0b11;
    }

    void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves) {
        moves.clear();
        if (kings_moves >= 20) {
            return;
        }
        if (current_player == 1) {
            if (get_jump_list_black(moves)) {
                return;
            }
        }
        else if (get_jump_list_white(moves)) {
            return;
        }

        int curr_id = current_player - 1;
        uint32_t piecesU = pieces[curr_id].first;
        uint32_t piecesD = piecesU & kings;
        if (current_player == 1) {  // black
            piecesD = pieces[curr_id].first;
            piecesU = piecesD & kings;
        }

        // moves down
        uint32_t movers = (empty << 4) & piecesD;
        movers |= ((empty & 0x0E0E0E0E) << 3) & piecesD;
        movers |= ((empty & 0x00707070) << 5) & piecesD;
        while(movers) {
            uint32_t piece = msb(movers);
            uint32_t mv = (empty << 4) & piece;
            if (mv)
                moves.push_back(piece | (piece >> 4));
            
            mv = ((empty & 0x0E0E0E0E) << 3) & piece;
            if (mv)
                moves.push_back(piece | (piece >> 3));
            
            mv = ((empty & 0x00707070) << 5) & piece;
            if (mv)
                moves.push_back(piece | (piece >> 5));
            
            movers ^= piece;
        }

        // moves up
        movers = (empty >> 4) & piecesU;
        movers |= ((empty & 0x70707070) >> 3) & piecesU;
        movers |= ((empty & 0x0E0E0E00) >> 5) & piecesU;
        while(movers) {
            uint32_t piece = msb(movers);
            uint32_t mv = (empty >> 4) & piece;
            if (mv)
                moves.push_back(piece | (piece << 4));
            
            mv = ((empty & 0x70707070) >> 3) & piece;
            if (mv)
                moves.push_back(piece | (piece << 3));
            
            mv = ((empty & 0x0E0E0E00) >> 5) & piece;
            if (mv)
                moves.push_back(piece | (piece << 5));
            
            movers ^= piece;
        }

        if (moves.empty()) {
            variables[current_player - 1] = 0;
            variables[current_player & 1] = 100;
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

    inline uint32_t game_state::get_jumpers_down(const uint32_t& jumpers, const uint32_t& pieces) const {
        uint32_t jumps = jumpers & 0x07070700 & (empty << 7) & (pieces << 4);  // odd -> right
        jumps |= jumpers & 0x0E0E0E00 & (empty << 9) & (pieces << 5);  // odd -> left
        jumps |= jumpers & 0x70707000 & (empty << 7) & (pieces << 3);  // even -> right
        jumps |= jumpers & 0xE0E0E000 & (empty << 9) & (pieces << 4);  // even -> left
        return jumps;
    }

    inline uint32_t game_state::get_jumpers_up(const uint32_t& jumpers, const uint32_t& pieces) const {
        uint32_t jumps = jumpers & 0x00070707 & (empty >> 9) & (pieces >> 4);  // odd -> right
        jumps |= jumpers & 0x000E0E0E & (empty >> 7) & (pieces >> 3);  // odd -> left
        jumps |= jumpers & 0x00707070 & (empty >> 9) & (pieces >> 5);  // even -> right
        jumps |= jumpers & 0x00E0E0E0 & (empty >> 7) & (pieces >> 4);  // even -> left
        return jumps;
    }

    bool game_state::get_jump_list_black(std::vector<move>& moves) const {
        uint32_t current = pieces[0].first;
        uint32_t opponent = pieces[1].first;
        uint32_t piecesDown = get_jumpers_down(current, opponent);
        uint32_t piecesUp = get_jumpers_up(current & kings, opponent);
        get_jump_list_down(piecesDown & ~kings, opponent, empty, moves);
        get_jump_list_two_sides(piecesUp | (piecesDown & kings), opponent, empty, moves);
        return !moves.empty();
    }

    bool game_state::get_jump_list_white(std::vector<move>& moves) const {
        uint32_t current = pieces[1].first;
        uint32_t opponent = pieces[0].first;
        uint32_t piecesUp = get_jumpers_up(current, opponent);
        uint32_t piecesDown = get_jumpers_down(current & kings, opponent);
        get_jump_list_up(piecesUp & ~kings, opponent, empty, moves);
        get_jump_list_two_sides(piecesDown | (piecesUp & kings), opponent, empty, moves);
        return !moves.empty();
    }

    void game_state::get_jump_list_down(uint32_t jumpers, uint32_t pieces, uint32_t empty, std::vector<move>& jumps) const {
        static constexpr jumpMask bits[4] = {
            {0x70707000, 7, 3}, // even rows
            {0xE0E0E000, 9, 4}, // even rows
            {0x07070700, 7, 4}, // odd rows
            {0x0E0E0E00, 9, 5}  // odd rows
        };
        while(jumpers) {
            uint32_t jumper = msb(jumpers);
            int offset = (jumper & 0x0F0F0F00) ? 2 : 0;  // offset for odd rows
            for (int i = offset; i < 2 + offset; ++i) {
                bool isJump = bits[i].mask & (empty << bits[i].empty) & (pieces << bits[i].enemy) & jumper;
                if (isJump) {
                    uint32_t jump = jumper | (jumper >> bits[i].empty) | (jumper >> bits[i].enemy);
                    std::vector<move> nextJumps;
                    nextJumps.reserve(4);
                    get_jump_list_down(jumper >> bits[i].empty, pieces, empty ^ (jumper >> bits[i].enemy), nextJumps);
                    if (!nextJumps.empty()) {
                        for (const auto& next : nextJumps) {
                            jumps.push_back(jump ^ next.mr);
                        }
                    }
                    else {
                        jumps.push_back(jump);
                    }
                }
            }
            jumpers ^= jumper;
        }
    }

    void game_state::get_jump_list_up(uint32_t jumpers, uint32_t pieces, uint32_t empty, std::vector<move>& jumps) const {
        static constexpr jumpMask bits[4] = {
            {0x00707070, 9, 5}, // even rows
            {0x00E0E0E0, 7, 4}, // even rows
            {0x00070707, 9, 4}, // odd rows
            {0x000E0E0E, 7, 3}  // odd rows
        };
        while(jumpers) {
            uint32_t jumper = msb(jumpers);
            int offset = (jumper & 0x00F0F0F0) ? 0 : 2;  // offset for odd rows
            for (int i = offset; i < 2 + offset; ++i) {
                bool isJump = bits[i].mask & (empty >> bits[i].empty) & (pieces >> bits[i].enemy) & jumper;
                if (isJump) {
                    uint32_t jump = jumper | (jumper << bits[i].empty) | (jumper << bits[i].enemy);
                    std::vector<move> nextJumps;
                    nextJumps.reserve(4);
                    get_jump_list_up(jumper << bits[i].empty, pieces, empty ^ (jumper << bits[i].enemy), nextJumps);
                    if (!nextJumps.empty()) {
                        for (const auto& next : nextJumps) {
                            jumps.push_back(jump ^ next.mr);
                        }
                    }
                    else {
                        jumps.push_back(jump);
                    }
                }
            }
            jumpers ^= jumper;
        }
    }

    void game_state::get_jump_list_two_sides(uint32_t jumpers, uint32_t pieces, uint32_t empty, std::vector<move>& jumps) const {
        static constexpr jumpMask bits[8] = {
            {0x00707070, 9, 5}, // even up
            {0x00E0E0E0, 7, 4}, // even up
            {0x70707000, 7, 3}, // even down
            {0xE0E0E000, 9, 4}, // even down
            {0x00070707, 9, 4}, // odd up
            {0x000E0E0E, 7, 3}, // odd up
            {0x07070700, 7, 4}, // odd down
            {0x0E0E0E00, 9, 5}  // odd down
        };
        while(jumpers) {
            uint32_t jumper = msb(jumpers);
            int offset = (jumper & 0x0F0F0F0F) ? 4 : 0;  // offset for odd rows
            for (int i = offset; i < 2 + offset; ++i)
            {
                // jumps up
                bool isJump = bits[i].mask & (empty >> bits[i].empty) & (pieces >> bits[i].enemy) & jumper;
                if (isJump)
                {
                    uint32_t jump = jumper | (jumper << bits[i].empty) | (jumper << bits[i].enemy);
                    std::vector<move> nextJumps;
                    nextJumps.reserve(8);
                    get_jump_list_two_sides(jumper << bits[i].empty, pieces ^ (jumper << bits[i].enemy), empty ^ jumper, nextJumps);
                    if (!nextJumps.empty()) {
                        for (const auto& next : nextJumps) {
                            jumps.push_back(jump ^ next.mr);
                        }
                    }
                    else {
                        jumps.push_back(jump);
                    }
                }
                // jumps down
                isJump = bits[i+2].mask & (empty << bits[i+2].empty) & (pieces << bits[i+2].enemy) & jumper;
                if (isJump) {
                    uint32_t jump = jumper | (jumper >> bits[i+2].empty) | (jumper >> bits[i+2].enemy);
                    std::vector<move> nextJumps;
                    nextJumps.reserve(8);
                    get_jump_list_two_sides(jumper >> bits[i+2].empty, pieces ^ (jumper >> bits[i+2].enemy), empty ^ jumper, nextJumps);
                    if (!nextJumps.empty()) {
                        for (const auto& next : nextJumps) {
                            jumps.push_back(jump ^ next.mr);
                        }
                    }
                    else {
                        jumps.push_back(jump);
                    }
                }
            }
            jumpers ^= jumper;
        }
    }

    inline uint32_t game_state::msb(const uint32_t& pieces) const {
        auto k = 31 - __builtin_clz(pieces);
        return 1u << k;
    }
}
