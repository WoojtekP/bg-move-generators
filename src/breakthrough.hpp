#include <cstdint>
#include <vector>

#include <boost/container/static_vector.hpp>


union signed_unsigned {
    int_fast32_t signed_int;
    uint_fast32_t unsigned_int;
};

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
        action_representation(uint_fast64_t move) {
            signed_unsigned converter;
            converter.unsigned_int = move >> 32;
            index = converter.signed_int;
            converter.unsigned_int = move & 0xFFFFFFFF;
            cell = converter.signed_int;
        }
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
        move(const uint_fast64_t mv) : mr({action_representation(mv)}) {};
        move(const move_representation& mv) : mr(mv) {};
        move(void) = default;
        bool operator==(const move& rhs) const {
            return mr == rhs.mr;
        }
    };
    class game_state {
        public:
            int get_piece(int cell_id) const;
            int get_variable_value(int variable_id) const;
            int get_current_player(void) const;
            int get_player_score(int player_id) const;
            void apply_move(const move& m);
            void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
            bool apply_any_move(resettable_bitarray_stack&);
            int get_monotonicity_class(void);
            bool is_legal(const move& m) const;
        private:
            inline uint_fast64_t convert_to_uint(const action_representation action);
            inline uint_fast64_t msb(const uint_fast64_t&) const;
            static constexpr uint_fast64_t maskLD = 0xFEFEFEFEFEFEFEFE;
            static constexpr uint_fast64_t maskRD = 0x7F7F7F7F7F7F7F7F;
            uint_fast64_t empty = 0xFFFFFFFF0000;
            uint_fast64_t pieces[2] = {0xFFFF, 0xFFFF000000000000};
            uint_fast64_t last_row[2] = {0xFF00000000000000, 0xFF};
            int current_player = 1;
            int variables[2] = {0, 0};
    };
}
