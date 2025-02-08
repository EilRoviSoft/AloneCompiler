#include "instruction_factory.hpp"

//compiler_info
#include "instruction_builder.hpp"

namespace amasm::compiler {
   InstInfoFactory& InstInfoFactory::generate_system() {
      using B = InstInfoBuilder;
      m_product->append_range(std::initializer_list {
         B().set_name("halt")
            .set_code(0x0)
            .get_product(),
         B().set_name("fcall")
            .set_code(0x2)
            .set_arguments_count(1)
            .get_product(),
         B().set_name("ret")
            .set_code(0x3)
            .set_arguments_count(0, 1)
            .get_product()
      });

      return *this;
   }
   template<typename T, size_t TOffset>
   InstInfoFactory& InstInfoFactory::generate_universal_size() {
      using B = InstInfoBuilder;
      const std::string suffix = std::to_string(sizeof(T) * 8);
      m_product->append_range(std::initializer_list {
         B().set_name("mov" + suffix)
            .set_code(TOffset + 0x0)
            .set_arguments_count(2)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("push" + suffix)
            .set_code(TOffset + 0x1)
            .set_arguments_count(0, 1)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("pop" + suffix)
            .set_code(TOffset + 0x2)
            .set_arguments_count(0, 1)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("not" + suffix)
            .set_code(TOffset + 0x3)
            .set_arguments_count(0, 1)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("and" + suffix)
            .set_code(TOffset + 0x4)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("or" + suffix)
            .set_code(TOffset + 0x5)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("xor" + suffix)
            .set_code(TOffset + 0x6)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("shl" + suffix)
            .set_code(TOffset + 0x7)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("shr" + suffix)
            .set_code(TOffset + 0x8)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product()
      });

      return *this;
   }
   template<typename T, char TPostfix, size_t TOffset>
   InstInfoFactory& InstInfoFactory::generate_universal_type() {
      using B = InstInfoBuilder;
      const std::string suffix = std::to_string(sizeof(T) * 8) + TPostfix;
      m_product->append_range(std::initializer_list {
         B().set_name("add" + suffix)
            .set_code(TOffset + 0x0)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("sub" + suffix)
            .set_code(TOffset + 0x1)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("mul" + suffix)
            .set_code(TOffset + 0x2)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("div" + suffix)
            .set_code(TOffset + 0x3)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("mod" + suffix)
            .set_code(TOffset + 0x4)
            .set_arguments_count(2, 3)
            .set_bit_depth(sizeof(T) * 8)
            .get_product(),
         B().set_name("cmp" + suffix)
            .set_code(TOffset + 0x5)
            .set_arguments_count(2)
            .set_bit_depth(sizeof(T) * 8)
            .get_product()
      });

      return *this;
   }
   template<typename T, char TPostfix, size_t TOffset>
   InstInfoFactory& InstInfoFactory::generate_signed_type() {
      using B = InstInfoBuilder;
      m_product->append_range(generate_universal_type<T, TPostfix, TOffset>());
      m_product->emplace_back(
         B().set_name("neg" + std::to_string(sizeof(T) * 8) + TPostfix)
            .set_code(TOffset + 0x8)
            .set_arguments_count(1, 2)
            .set_bit_depth(sizeof(T) * 8)
            .get_product()
      );

      return *this;
   }

   InstInfoFactory& InstInfoFactory::generate_info() {
      // [halt, fcall, ret]
      generate_system();

      // [mov push pop not and or xor shl shr] for [8 16 32 64]
      generate_universal_size<uint8_t, 0x100>();
      generate_universal_size<uint16_t, 0x110>();
      generate_universal_size<uint32_t, 0x120>();
      generate_universal_size<uint64_t, 0x130>();

      // [add sub mul div mod cmp] for [8u 16u 32u 64u]
      generate_universal_type<uint8_t, 'u', 0x140>();
      generate_universal_type<uint16_t, 'u', 0x150>();
      generate_universal_type<uint32_t, 'u', 0x160>();
      generate_universal_type<uint64_t, 'u', 0x170>();

      // [add sub mul div mod cmp neg] for [8i 16i 32i 64i]
      generate_universal_type<uint8_t, 'i', 0x180>();
      generate_universal_type<uint16_t, 'i', 0x190>();
      generate_universal_type<uint32_t, 'i', 0x1A0>();
      generate_universal_type<uint64_t, 'i', 0x1B0>();

      return *this;
   }
}
