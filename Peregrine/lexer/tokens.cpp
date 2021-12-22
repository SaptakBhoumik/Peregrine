#include "tokens.hpp"

namespace TokenUtils {

bool isArithmeticToken(Token tok) {
    switch (tok.tkType) {
        case tk_plus:
        case tk_minus:
        case tk_divide:
        case tk_multiply:
        case tk_modulo:
        case tk_exponent:
            return true;

        default:
            return false;
    }
}

bool isComparisonToken(Token tok) {
    switch (tok.tkType) {
        case tk_greater:
        case tk_less:
        case tk_equal:
        case tk_not_equal:
        case tk_gr_or_equ:
        case tk_less_or_equ:
            return true;

        default:
            return false;
    }
}

bool isBitwiseToken(Token tok) {
    switch (tok.tkType) {
        case tk_xor:
        case tk_ampersand:
        case tk_bit_or:
        case tk_bit_not:
        case tk_shift_left:
        case tk_shift_right:
            return true;

        default:
            return false;
    }
}

} // namespace TokenUtils