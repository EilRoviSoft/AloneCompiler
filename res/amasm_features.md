# Table of contents

[//]: # (@formatter:off)
<!-- TOC -->
* [Table of contents](#table-of-contents)
* [Code sections](#code-sections)
* [Memory types](#memory-types)
* [Variables, constants and registers](#variables-constants-and-registers)
* [Comments](#comments)
* [Addressing modes](#addressing-modes)
  * [Direct operands](#direct-operands)
  * [Simple operands](#simple-operands)
* [Labels and branches](#labels-and-branches)
* [Functions](#functions)
* [Struct](#struct)
<!-- TOC -->
[//]: # (@formatter:on)

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

# Variables, constants and registers

You can define names for memory regions from stack and heap.
This only affects compilation time.

```asm
var %a, uint64, [%spx - 8]
```

Also allocate data before main program as constants.

```asm
const %arr [int32 * 3], [18, 24, 34]
str %msg [uint8 * 12], "Hello world\n"
```

Registers are predefined as variables and can be accessed at any time using '%' operand

| Register name | Place | Definition          | Function                                                                                                                     |
|---------------|-------|---------------------|------------------------------------------------------------------------------------------------------------------------------|
| %asx          | 0x00  | Address             | Contains address of last allocated pointer on heap; also can be used to create new struct objects                            |
| %rsx          | 0x08  | Result              | Contains value of last evaluated math expression; can be used as general register for results of binary and unary operations |
| %lox          | 0x10  | Left operand        | Can be used as general register for binary and unary operations                                                              |
| %rox          | 0x18  | Right operand       | Can be used as general register for binary and unary operations                                                              |
| %ipx          | 0x20  | Instruction pointer | Contains pointer to current executing instruction                                                                            |
| %bpx          | 0x28  | Base pointer        | Contains pointer to the last function entry                                                                                  |
| %spx          | 0x30  | Stack pointer       | Contains pointer to the top of the stack                                                                                     |
| %flags        | 0x38  | Flags pointer       | Contains several flags for other instructions                                                                                |
| %grx          | 0x40  | General             | Start of general purpose registers; size is defined as `registers_size - 64` bytes                                           |

---

# Comments

```asm
mov32 %lox, %rox # can be read as "moves contents of %rox register to %lox register"
```

---

# Addressing modes

There are 4 types of addressing modes for several binary functions

## Direct operands

Works directly with given part of memories without changing it

```asm
mov32 %lox, %rox
```

This can be read as "moves contents of %rox register to %lox register"

## Simple operands

Doesn't require any operands other than instruction'sizes name

```asm
mov32 %lox, 2
mov32 %rox, 3
add32u         # stores value '5' in %rsx 
```

---

# Labels and branches

```asm
label main__l0a:  # this branch is generated for loop purposes
```

---

# Functions

```asm
func @add6432(int64, int32): int64 { # uses pascal-call
    mov %rox, [%spx - 4]
    add64i [%spx - 12], %rox
    sub64u %spx, 12
    push64 %asx
    ret
}
```

This function adds int64 and int32 from stack; removes these arguments from stack; pushes result on stack

---

# Struct

```asm
struct some_t {	                # you can define rules_collection for struct align
    var %a, int64, [%this + 8]  # and create struct members
    var %b, int32, [%this + 8]  # members will be placed in order of defining
    var %c, int8, [%this + 12]
    var %d, int8, [%this + 16]
}

func @main() {
    var %sizes, some_t, [%spx]
    mov64 %sizes.a, 10
    mov32 %sizes.b, 20
    push64 %sizes.a
    push32 %sizes.b
    call @add6432(int64, int32)  # pascal-call automatically cleares stack from arguments
    ncall @print64u 	         # if you want to create custom functions from C++, make wraper for it
}
```

This program adds 10 and 20, after that writes "30" in console

---