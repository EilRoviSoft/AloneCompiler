#include "instruction_factory.hpp"

//compiler_info
#include "instruction_builder.hpp"

namespace amasm::compiler {
   InstInfoFactory& InstInfoFactory::generate_system() {
      using B = InstInfoBuilder;
      m_product->append_range(std::initializer_list {
         B().name("halt")
            .code(0x0)
            .build(),
         B().name("fcall")
            .code(0x2)
            .arguments_count(1)
            .build(),
         B().name("ret")
            .code(0x3)
            .arguments_count(0, 1)
            .build()
      });

      return *this;
   }
   template<typename T, size_t TOffset>
   InstInfoFactory& InstInfoFactory::generate_universal_size() {
      using B = InstInfoBuilder;
      const std::string suffix = std::to_string(sizeof(T) * 8);
      m_product->append_range(std::initializer_list {
         B().name("mov" + suffix)
            .code(TOffset + 0x0)
            .arguments_count(2)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("push" + suffix)
            .code(TOffset + 0x1)
            .arguments_count(0, 1)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("pop" + suffix)
            .code(TOffset + 0x2)
            .arguments_count(0, 1)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("not" + suffix)
            .code(TOffset + 0x3)
            .arguments_count(0, 1)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("and" + suffix)
            .code(TOffset + 0x4)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("or" + suffix)
            .code(TOffset + 0x5)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("xor" + suffix)
            .code(TOffset + 0x6)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("shl" + suffix)
            .code(TOffset + 0x7)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("shr" + suffix)
            .code(TOffset + 0x8)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build()
      });

      return *this;
   }
   template<typename T, char TPostfix, size_t TOffset>
   InstInfoFactory& InstInfoFactory::generate_universal_type() {
      using B = InstInfoBuilder;
      const std::string suffix = std::to_string(sizeof(T) * 8) + TPostfix;
      m_product->append_range(std::initializer_list {
         B().name("add" + suffix)
            .code(TOffset + 0x0)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("sub" + suffix)
            .code(TOffset + 0x1)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("mul" + suffix)
            .code(TOffset + 0x2)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("div" + suffix)
            .code(TOffset + 0x3)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("mod" + suffix)
            .code(TOffset + 0x4)
            .arguments_count(2, 3)
            .bit_depth(sizeof(T) * 8)
            .build(),
         B().name("cmp" + suffix)
            .code(TOffset + 0x5)
            .arguments_count(2)
            .bit_depth(sizeof(T) * 8)
            .build()
      });

      return *this;
   }
   template<typename T, char TPostfix, size_t TOffset>
   InstInfoFactory& InstInfoFactory::generate_signed_type() {
      using B = InstInfoBuilder;
      m_product->append_range(generate_universal_type<T, TPostfix, TOffset>());
      m_product->emplace_back(
         B().name("neg" + std::to_string(sizeof(T) * 8) + TPostfix)
            .code(TOffset + 0x8)
            .arguments_count(1, 2)
            .bit_depth(sizeof(T) * 8)
            .build()
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
