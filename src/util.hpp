#pragma once

namespace alone::util {
	template<typename T> T add(const T& lhs, const T& rhs) { return lhs + rhs; }
	template<typename T> T sub(const T& lhs, const T& rhs) { return lhs - rhs; }
	template<typename T> T mul(const T& lhs, const T& rhs) { return lhs * rhs; }
	template<typename T> T div(const T& lhs, const T& rhs) { return lhs / rhs; }
	template<typename T> T mod(const T& lhs, const T& rhs) { return lhs % rhs; }
	template<typename T> T inc(const T& val) { return val + 1; }
	template<typename T> T dec(const T& val) { return val - 1; }
	template<typename T> T neg(const T& val) { return -val; }
}
