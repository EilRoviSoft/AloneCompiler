# Table of contents

<!-- TOC -->
- [Table of contents](#table-of-contents)
- [Code sections](#code-sections)
- [Memory types](#memory-types)
- [Working with data in memory](#working-with-data-in-memory)
  - [Named addresses aka Variables](#named-addresses-aka-variables)
  - [Predefined data aka Constants](#predefined-data-aka-constants)
  - [Registers set](#registers-set)
  - [Different bit modes](#different-bit-modes)
- [Addressing modes](#addressing-modes)
  - [Direct operands](#direct-operands)
- [Labels and branches](#labels-and-branches)
- [Functions](#functions)
- [Struct](#struct)
<!-- TOC -->

---

# Code sections

There are 4 main sections of asm:

- `.code`: there are code.
- `.data`: initialized (named) data like constants, file names, strings, buffer sizes, etc.
- `.bss`: uninitialized data like variables, pointers, etc.

---

# Memory types

- `mframe` includes registers, program and stack. They are placed in ascending order from 0 to max_stack_size.
- `dframe` includes pointers in regular memory and used for allocating arrays and some dynamic data.
- `eframe` includes external functions.
- `sframe` includes static data that can be shared with external programs.

---

# Working with data in memory

## Named addresses aka Variables

You can define names for memory regions from stack and heap.
This only affects compilation time.

```asm
var %a: uint64, [%rsp - 8];   # will not be replaced with anything
```

If you write variable definition without indicating her adress, 
line will be automatically replaced with `push`.
It affects compile time.

```asm
var %b: uint64;               # in bytecode will be replaced with `push64`
```

## Predefined data aka Constants

Also allocate data before main program as constants.
Elements inside const can't be larger than 8 bytes.

```asm
const %val: int64, 69420;
const %arr: [int32 * 3], [18, 24, 34];
str %msg: "Hello world\n";
```

## Registers set

Registers are predefined as variables and can be accessed at any time using `%` operand.

| 16-bit basic register name | Place | Definition        | Function                                                                                 |
| -------------------------- | ----- | ----------------- | ---------------------------------------------------------------------------------------- |
| `%ax`                      | 0x00  | Accumulator       | It is used for I/O port access, arithmetic, interrupt calls                              |
| `%bx`                      | 0x08  | Base              | It is used as a base pointer for memory access                                           |
| `%cx`                      | 0x10  | Counter           | It is used as a loop counter and for shifts                                              |
| `%dx`                      | 0x18  | Data              | It is used for I/O port access, arithmetic, some interrupt calls                         |
| `%si`                      | 0x20  | Source Index      | Used for string, memory array copying and setting and for far pointer addressing with ES |
| `%di`                      | 0x28  | Destination Index | Used for string and memory array copying                                                 |
| `%ip`                      | 0x30  | Index Pointer     | Holds the offset of the next instruction                                                 |
| `%bp`                      | 0x38  | Base Pointer      | Holds the base address of the stack                                                      |
| `%sp`                      | 0x40  | Stack Pointer     | Holds the top address of the stack                                                       |
| `%flags`                   | 0x48  | Flags Pointer     | Contains several flags for other instructions                                            |
| `%gp`                      | 0x50  | General Purpose   | Start of general purpose registers; size is defined as `registers_size - 80` bytes       |

## Different bit modes

If you need **32-bit** register, add `e` (shortcut from `extended`) at the start (`%ax` -> `%eax`).

If you need **64-bit** register, add `r` (shortcut from `register`) at the start (`%ax` -> `%rax`).

If you need other parts of registers set, just use absolute adresses or make your own shortcuts inside functions.

---

# Addressing modes

There are 4 types of addressing modes for several binary functions.

## Direct operands

Works directly with given part of memories without changing it.

```asm
mov32 %ecx, %edx
```

This can be read as "moves contents of %ecx register to edx register".

# Labels and branches

```asm
label main__l0a:  # this branch is generated for loop purposes
```

---

# Functions

This function adds int64 and int32 from stack; removes these arguments from stack; pushes result on stack.
Also you can call labels only inside current function.

```asm
func @add6432(int64, int32): int64 { # arguments pushes right-to-left
    mov64 %rcx, [%rsp - 12];
    mov32 %rdx, [%rsp - 4];
    add64i %rbx, %rcx, %rdx;
    ret;
}
```

---

# Struct

```asm
struct some_t {                    # members will be placed in order of defining
    var %a: int64;
    var %b: int32;
    var %c: int8;
    var %d: int8;
}

func @main() {
    var %sizes, some_t, [%rsp];
    mov64 %sizes.a, 10;
    mov32 %sizes.b, 20;
    push64 %sizes.a;
    push32 %sizes.b;
    fcall @add6432(int64, int32);  # pascal-call automatically cleares stack from arguments
    ncall @print64u; 	             # if you want to create custom functions from C++, make wraper for it
}
```

This program adds 10 and 20, after that writes "30" in console.

---