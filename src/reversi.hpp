#include <cstdint>
#include <vector>

#include <boost/container/static_vector.hpp>


namespace reasoner {
    constexpr int BOARD_SIZE = 64;
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int NUMBER_OF_PIECES = 3;
    constexpr int NUMBER_OF_VARIABLES = 2;
    constexpr int MONOTONIC_CLASSES = 0;

    class resettable_bitarray_stack {};
    struct action_representation {
        int index;
        int cell;
        action_representation(void) = default;
        action_representation(int index)
        : index(index) {}
        action_representation(int index, int cell)
        : index(index),
          cell(cell) {}
        bool operator==(const action_representation& rhs) const {
            return index == rhs.index && cell == rhs.cell;
        }
    };

    using move_representation = boost::container::static_vector<action_representation, 1>;
    struct move {
        move_representation mr;
        move(const int mv) : mr({action_representation(mv)}) {};
        move(const move_representation& mv) : mr(mv) {};
        move(void) = default;
    };
    class game_state {
        public:
            int get_piece(int cell_id) const;
            int get_variable_value(int variable_id) const;
            int get_current_player(void) const;
            int get_player_score(int player_id) const;
            void apply_move(const move& m);
            void apply(const move& m);
            void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
            bool apply_any_move(resettable_bitarray_stack&);
            int get_monotonicity_class(void);
            bool is_legal(const move& m) const;
        private:
            inline uint_fast64_t get_movers(const uint_fast64_t& current, const uint_fast64_t& opponent) {
                uint_fast64_t flipped = up(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= up(flipped) & opponent;
                uint_fast64_t result = up(flipped) & empty;

                flipped = down(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= down(flipped) & opponent;
                result |= down(flipped) & empty;

                flipped = left(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= left(flipped) & opponent;
                result |= left(flipped) & empty;

                flipped = right(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= right(flipped) & opponent;
                result |= right(flipped) & empty;

                flipped = up_left(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= up_left(flipped) & opponent;
                result |= up_left(flipped) & empty;

                flipped = up_right(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= up_right(flipped) & opponent;
                result |= up_right(flipped) & empty;

                flipped = down_left(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= down_left(flipped) & opponent;
                result |= down_left(flipped) & empty;

                flipped = down_right(current) & opponent;
                for (int j = 0; j < 5; ++j)
                    flipped |= down_right(flipped) & opponent;
                result |= down_right(flipped) & empty;

                return result;
            }
            inline uint_fast64_t up(const uint_fast64_t& pieces) const {
                return pieces << 8;
            }
            inline uint_fast64_t down(const uint_fast64_t& pieces) const {
                return pieces >> 8;
            }
            inline uint_fast64_t left(const uint_fast64_t& pieces) const {
                return (pieces & 0x7F7F7F7F7F7F7F7Full) << 1;
            }
            inline uint_fast64_t right(const uint_fast64_t& pieces) const {
                return (pieces & 0xFEFEFEFEFEFEFEFEull) >> 1; 
            }
            inline uint_fast64_t up_left(const uint_fast64_t& pieces) const {
                return (pieces & 0x7F7F7F7F7F7F7F7Full) << 9;
            }
            inline uint_fast64_t up_right(const uint_fast64_t& pieces) const {
                return (pieces & 0xFEFEFEFEFEFEFEFEull) << 7;
            }
            inline uint_fast64_t down_left(const uint_fast64_t& pieces) const {
                return (pieces & 0x7F7F7F7F7F7F7F7Full) >> 7;
            }
            inline uint_fast64_t down_right(const uint_fast64_t& pieces) const {
                return (pieces & 0xFEFEFEFEFEFEFEFEull) >> 9;
            }
            inline int msb(const uint_fast64_t& pieces) const {
                return 63 - __builtin_clzll(pieces);
            }
            uint_fast64_t pieces[2] = {
                0x1008000000ull,
                0x0810000000ull
            };
            uint_fast64_t empty = 0xFFFFFFE7E7FFFFFFull;
            int current_player = 1;
            int variables[2] = {0, 0};
    };
}
