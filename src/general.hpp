#pragma once

//std
#include <array>
#include <bitset>
#include <deque>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace alone::lib {
    //objects

    inline auto string_hasher = std::hash<std::string>();

    //data_types

    using inst_code_t = uint32_t;
    using address_t = uint64_t;
    using machine_word_t = uint64_t;
    using flags_t = std::bitset<64>;
    using mask_predicate = std::function<std::byte(const std::byte&, const std::byte&)>;

    constexpr size_t machine_word_size = sizeof(uint64_t);
    constexpr size_t inst_code_size = sizeof(inst_code_t);

    enum class argument_type : uint8_t {
        empty = 0,
        direct,
        immediate,
        indirect_with_displacement
    };
    using args_data_t = std::array<argument_type, 4>;

    enum class literal_type : uint8_t {
        none = 0,
        word,
        binary, decimal, hexadecimal, floating
    };

    enum class regs_set : uint8_t {
        asx   = 0 * machine_word_size, //Address System register
        rsx   = 1 * machine_word_size, //Result System register
        lox   = 2 * machine_word_size, //Left Operand register
        rox   = 3 * machine_word_size, //Right Operand register
        ipx   = 4 * machine_word_size, //Instruction Pointer
        bpx   = 5 * machine_word_size, //Stack Pointer
        spx   = 6 * machine_word_size, //Base Pointer
        cpx   = 7 * machine_word_size, //Context pointer
        flags = 8 * machine_word_size, //Flags register
        grx   = 9 * machine_word_size, //General Registers start
    };
    constexpr size_t registers_size = 256;
    constexpr size_t system_registers_size = registers_size - (uint64_t) regs_set::grx;
    constexpr size_t general_registers_size = registers_size - system_registers_size;

    enum class flags_set : uint8_t {
        rf = 0, //is program running flag
        zf = 1, //zero flag
        sf = 2, //sign flag
        cf = 3, //carry flag
        of = 4, //overflow flag
    };

    enum class memory_type : uint8_t {
        mframe, //registers + application + stack
        dframe, //pointers
        eframe, //external memory
        sframe, //static memory
    };
    constexpr size_t mframe_size = 1 << 16;
    constexpr size_t dframe_size = 16;

    template<typename... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    template<typename T>
    struct array_t {
        size_t size;
        T* ptr;
    };

    class Bytecode {
    public:
        std::byte& operator[](const size_t& idx) {
            return _container[idx];
        }
        const std::byte& operator[](const size_t& idx) const {
            return _container[idx];
        }

        template<typename T>
        T get(size_t offset) const {
            T result = 0;
            for (size_t i = 0; i < sizeof(T); i++)
                result |= ((T) _container[i + offset] & 0xff) << i * 8;
            return result;
        }
        template<typename T>
        void set(const T& val, const size_t& offset) {
            for (size_t i = 0; i < sizeof(T); i++)
                _container[i + offset] = std::byte((val & 0xff << i * 8) >> i * 8);
        }

        template<typename T>
        void apply_mask(const T& mask, const mask_predicate& pred, const size_t& offset = 0) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&mask);
            for (size_t i = 0; i < sizeof(T); i++)
                _container[i + offset] = pred(_container[i + offset], as_bytes[i]);
        }

        void prepend_bytecode(const Bytecode& another) {
            _container.prepend_range(another._container);
        }
        void append_bytecode(const Bytecode& another) {
            _container.append_range(another._container);
        }

        template<typename T>
        void prepend(const T& value, const size_t& size = sizeof(T)) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&value);
            _container.prepend_range(std::initializer_list(as_bytes, as_bytes + size));
        }
        template<typename T>
        void append(const T& value, const size_t& size = sizeof(T)) {
            const auto as_bytes = reinterpret_cast<const std::byte*>(&value);
            _container.append_range(std::initializer_list(as_bytes, as_bytes + size));
        }

        size_t size() const { return _container.size(); }

        std::vector<std::byte> as_vector() const { return std::ranges::to<std::vector>(_container); }

    protected:
        std::deque<std::byte> _container;
    };

    //functions

    constexpr bool is_alpha(const char& c);
    constexpr bool is_binary(const char& c);
    constexpr bool is_numeric(const char& c);
    constexpr bool is_hexadecimal(const char& c);
    constexpr bool is_alpha_numeric(const char& c);
    constexpr bool is_whitespace(const char& c);

    literal_type check_type(const std::string& l);

    std::string gen_str(const char& a, const char& b);

    std::tuple<address_t, memory_type> decompose_address(const address_t& address);
    std::tuple<inst_code_t, args_data_t> decompose_instruction(const inst_code_t& instruction);
}
