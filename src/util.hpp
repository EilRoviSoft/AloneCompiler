#pragma once

namespace alone::util {
    template<typename T>
    T call_add(const T& lhs, const T& rhs) { return lhs + rhs; }
    template<typename T>
    T call_sub(const T& lhs, const T& rhs) { return lhs - rhs; }
    template<typename T>
    T call_mul(const T& lhs, const T& rhs) { return lhs * rhs; }
    template<typename T>
    T call_div(const T& lhs, const T& rhs) { return lhs / rhs; }
    template<typename T>
    T call_mod(const T& lhs, const T& rhs) { return lhs % rhs; }
    template<typename T>
    T call_inc(const T& val) { return val + 1; }
    template<typename T>
    T call_dec(const T& val) { return val - 1; }
    template<typename T>
    T call_neg(const T& val) { return -val; }
    template<typename T>
    T call_not(const T& val) { return ~val; }
    template<typename T>
    T call_and(const T& lhs, const T& rhs) { return lhs & rhs; }
    template<typename T>
    T call_or(const T& lhs, const T& rhs) { return lhs | rhs; }
    template<typename T>
    T call_xor(const T& lhs, const T& rhs) { return lhs ^ rhs; }
    template<typename T>
    T call_shl(const T& lhs, const T& rhs) { return lhs << rhs; }
    template<typename T>
    T call_shr(const T& lhs, const T& rhs) { return lhs >> rhs; }
}
