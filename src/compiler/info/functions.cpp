#include "functions.hpp"

namespace amasm::compiler {
    std::string generate_func_full_name(const func_info& func) {
        std::string result = '@' + func.name + '(';
        if (!func.args.empty()) {
            for (size_t i = 0; i < func.args.size() - 1; i++)
                result += func.args[i]->name + ',';
            result += func.args.back()->name;
        }
        result += ')';
        return result;
    }
}
