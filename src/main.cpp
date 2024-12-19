//std
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    auto slice = [](const std::vector<int>& c, const size_t& begin, const size_t& end) {
        return c | std::views::drop(begin) | std::views::take(end - begin);
    };

    const auto vec = std::vector { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (const auto& it : slice(vec, 2, 4))
        std::cout << it;

    return 0;
}
