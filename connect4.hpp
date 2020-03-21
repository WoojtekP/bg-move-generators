#include <cstdint>
#include <vector>

namespace reasoner {
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int MONOTONIC_CLASSES = 1;
    constexpr int KEEPER = 0;
    constexpr int RED = 1;
    constexpr int YELLOW = 2;

    class resettable_bitarray_stack {};

    typedef uint8_t move_representation;
    struct move {
        move_representation mr;
        move(const move_representation& mv) : mr(mv) {};
        move(void) = default;
    };
    class game_state {
        public:
            int get_current_player(void)const;
            int get_player_score(int player_id)const;
            void apply_move(const move& m);
            void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
            bool apply_any_move(resettable_bitarray_stack&);
            int get_monotonicity_class(void);
            bool is_legal(const move& m)const;
        private:
            uint64_t pieces[2] = {0};
            int current_player = RED;
            int variables[2] = {0, 0};
            int column_shift[7] = {0, 1, 2, 3, 4, 5, 6};
    };
}
