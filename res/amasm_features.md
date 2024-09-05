# Code sections
There are 4 main sections of asm:
- .code: there are code.
- .data: initialized (named) data like constants, file names, strings, buffer sizes, etc.
- .bss: uninitialized data like variables, pointers, etc.

---

# Memory types
- mframe includes registers, program and stack. They are placed in ascending order from 0 to max_stack_size.
- dframe includes pointers in regular memory and used for allocating arrays and some dynamic data.
- eframe includes external functions.
- sframe includes static data that can be shared with external programs.

---

# Variables, constants and registers
You can define names for memory regions from stack and heap.
This only affects compilation time.
- `var %a, $uint64, [%spx - 8]`

Also allocate data before main program as constants.
- `const %arr [$int32 * 3], [18, 24, 34]`
- `str %msg [$uint8 * 12], "Hello world\n"`

Registers are predefined as variables and can be accessed at any time using '%' operand
| Register name | Place | Definition | Function |
| - | - | - | - |
| %asx | 0x00 | Address | Contains address of last allocated pointer on heap; also can be used to create new struct objects |
| %rsx | 0x08 | Result | Contains value of last evaluated math expression; can be used as general register for results of binary and unary operations |
| %lox | 0x10 | Left operand | Can be used as general register for binary and unary operations |
| %rox | 0x18 | Left operand | Can be used as general register for binary and unary operations |
| %ipx | 0x20 | Instruction pointer | Contains pointer to current executing instruction |
| %bpx | 0x28 | Base pointer | Contains pointer to the last function entry |
| %spx | 0x30 | Stack pointer | Contains pointer to the top of the stack |
| %flags | 0x38 | Flags | Contains several flags for other instructions |
| %grx | 0x40 | General | Start of general purpose registers; size is defined as `registers_size - 64` bytes |

---

# Comments
```
mov32 %lox, %rox # can be read as "moves contents of %rox register to %lox register"
```

---

# Addressing modes
There are 4 types of addressing modes for several binary functions
## Direct operands
Works directly with given part of memories without changing it
```
mov32 %lox, %rox
```
This can be read as "moves contents of %rox register to %lox register"
## Simple operands
Doesn't require any operands other than instruction's name
```
mov32 %lox, 2
mov32 %rox, 3
add32u          # stores value '5' in %rsx 
```

---

# Functions
```
func @add6432(%l: $int64, %r: $int32):		# uses pascal-call
	add64i $l, $r
	sub %spx, 12
	push64 $asx
	ret
```
This function adds int64 and int32 from stack; removes these arguments from stack; pushes result on stack

---

# Struct
```
struct $some_t:					# you can define rules for struct align
	var %a, $int64, [%asx]		# and create struct members
	var %b, $int32, [%asx + 8]	# members will be placed in order of defining
	var %c, $int8, [%asx + 12]
	var %d, $int8, [%asx + 16]

var %s, $some_t, [%spx]
mov64 %s.a, 10
mov32 %s.b, 20
push64 %s.a
push32 %s.b
call @add6432(int64, int32)		# pascal-call automatically cleares stack from arguments
ncall @print64u					# pascal-call too, if you want to create custom functions from C++, make wraper for it
```
This program writes "30" in console

---

# Labels and branches
```
main__l0a:		#this branch is generated for loop purposes
```