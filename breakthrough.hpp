#include <cstdint>
#include <vector>

namespace reasoner {
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int MONOTONIC_CLASSES = 0;
    constexpr int KEEPER = 0;
    constexpr int WHITE = 1;
    constexpr int BLACK = 2;

    class resettable_bitarray_stack {};

    typedef uint64_t move_representation;
    struct move {
        move_representation mr;
        move(const move_representation& mv) : mr(mv) {};
        move(void) = default;
    };
    class game_state {
        public:
            int get_current_player(void) const;
            int get_player_score(int player_id) const;
            void apply_move(const move& m);
            void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
            bool apply_any_move(resettable_bitarray_stack&);
            int get_monotonicity_class(void);
            bool is_legal(const move& m) const;
        private:
            inline uint64_t msb(const uint64_t&) const;
            uint64_t empty = 0xFFFFFFFF0000;
            uint64_t pieces[2] = {0xFFFF, 0xFFFF000000000000};
            uint64_t last_row[2] = {0xFF00000000000000, 0xFF};
            const uint64_t maskLD = 0xFEFEFEFEFEFEFEFE;
            const uint64_t maskRD = 0x7F7F7F7F7F7F7F7F;
            int current_player = WHITE;
            int variables[2] = {0, 0};
    };
}
