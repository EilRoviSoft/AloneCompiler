namespace alone::util {
	template<typename T> T add(const T& lhs, const T& rhs) { return lhs + rhs; }
	template<typename T> T sub(const T& lhs, const T& rhs) { return lhs - rhs; }
	template<typename T> T mul(const T& lhs, const T& rhs) { return lhs * rhs; }
	template<typename T> T div(const T& lhs, const T& rhs) { return lhs / rhs; }
	template<typename T> T mod(const T& lhs, const T& rhs) { return lhs % rhs; }
}
