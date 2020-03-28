#include <cstdint>
#include <vector>

namespace reasoner {
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int MONOTONIC_CLASSES = 0;

    class resettable_bitarray_stack {};

    typedef uint32_t move_representation;
    struct move {
        move_representation mr;
        move(const move_representation& mv) : mr(mv) {};
        move(void) = default;
    };
    struct jumpMask
    {
        uint32_t mask;
        uint8_t empty;
        uint8_t enemy;
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
            inline uint32_t get_jumpers_down(const uint32_t&, const uint32_t&) const;
            inline uint32_t get_jumpers_up(const uint32_t&, const uint32_t&) const;
            bool get_jump_list_black(std::vector<move>&) const;
            bool get_jump_list_white(std::vector<move>&) const;
            void get_jump_list_down(uint32_t, uint32_t, uint32_t, std::vector<move>&) const;
            void get_jump_list_up(uint32_t, uint32_t, uint32_t, std::vector<move>&) const;
            void get_jump_list_two_sides(uint32_t, uint32_t, uint32_t, std::vector<move>&) const;
            inline uint32_t msb(const uint32_t&) const;
            std::pair<uint32_t, const uint32_t> pieces[2] = {
                { 0xFFF00000, 0x0000000F },
                { 0x00000FFF, 0xF0000000 }
            };
            uint32_t empty = 0x000FF000;
            uint32_t kings = 0;
            int kings_moves = 0;
            int current_player = 1;
            int variables[2] = {50, 50};
    };
}
