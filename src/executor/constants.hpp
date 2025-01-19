#pragma once

//std
#include <cstdint>

namespace amasm::executor {
	enum class MemoryType : uint8_t {
		MainFrame,    // registers + application + stack
		DynamicFrame, // pointers
		EternalFrame, // external data
		StaticFrame   // bss data
	};

	constexpr size_t mframe_size = 1 << 16;
	constexpr size_t dframe_size = 16;
}
