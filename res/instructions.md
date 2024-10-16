# General

---

# Lexical

| Name | Semantics                        | Notes                                                                      |
| ---- | -------------------------------- | -------------------------------------------------------------------------- |
| var  | `var %name, data_type`           | adds variable to the struct                                                |
| var  | `var %name, data_type, [offset]` | adds variable to the scope with certain offset, have to push to alloc size |

---

# System

| Name  | Size | ID     | Semantics                     | Notes                                                                  |
| ----- | ---- | ------ | ----------------------------- | ---------------------------------------------------------------------- |
| halt  | 4    | `0x00` | `halt`                        | Stops program                                                          |
| ncall | 12   | `0x01` | `ncall @native_func_id(args)` | Calls native function                                                  |
| fcall | 12   | `0x02` | `fcall @func_id(args)`        | Calls in-code defined function, works similar to `enter`               |
| ret   | 4    | `0x03` | `ret`                         | Leaves function, works similar to to `leave`                           |
| enter | 12   | `0x04` | `enter`                       | Moves `%bpx` to new address and saves previous position on heap        |
| leave | 4    | `0x05` | `leave`                       | Moves `%bpx` to previous position and clears all memory between places |

---

# Jumps and conditions

If you want to use standard `jmp`, use `add64u` or `add64u` instead

| Name    | Size | ID   | Semantics                    | Notes                                                   |
| ------- | ---- | ---- | ---------------------------- | ------------------------------------------------------- |
| jif_eql | 12   | 0x80 | `jif_eql address`            | Jumps to address if `ZF`                                |
| jif_eql | 20   | 0x81 | `jif_eql address0, address1` | Jumps to `address0` if `ZF` else to `address1`          |
| jif_neq | 12   | 0x82 | `jif_neq address`            | Jumps to address if not `ZF`                            |
| jif_neq | 20   | 0x83 | `jif_neq address0, address1` | Jumps to `address0` if not `ZF` else to `address1`      |
| jif_grt | 12   | 0x84 | `jif_grt address`            | Jumps to address if `SF`                                |
| jif_grt | 20   | 0x85 | `jif_grt address0, address1` | Jumps to `address0` if `SF` else to `address1`          |
| jif_goq | 12   | 0x86 | `jif_goq address`            | Jumps to address if `SF` or `ZF`                        |
| jif_goq | 20   | 0x87 | `jif_goq address0, address1` | Jumps to `address0` if `SF` or `ZF` else to `address1`  |
| jif_lwr | 12   | 0x88 | `jif_lwr address`            | Jumps to address if !`SF`                               |
| jif_lwr | 20   | 0x89 | `jif_lwr address0, address1` | Jumps to `address0` if !`SF` else to `address1`         |
| jif_loq | 12   | 0x8A | `jif_loq address`            | Jumps to address if !`SF` or `ZF`                       |
| jif_loq | 20   | 0x8B | `jif_loq address0, address1` | Jumps to `address0` if !`SF` or `ZF` else to `address1` |

---

# Size related

Offset ranges

| Size | Range           |
| ---- | --------------- |
| 8    | `0x100 - 0x10F` |
| 16   | `0x110 - 0x11F` |
| 32   | `0x120 - 0x12F` |
| 64   | `0x130 - 0x13F` |

| Name | Size             | Offset + ID     | Semantics        | Equals to       | Notes                        |
| ---- | ---------------- | --------------- | ---------------- | --------------- | ---------------------------- |
| mov  | 12 + `args.size` | `Offset + 0x00` | `mov a0, a1`     | `a0 = a1`       |                              |
| push | 4 + `arg.size`   | `Offset + 0x01` | `push a0`        |                 | pushes `a0` on stack         |
| pop  | 4                | `Offset + 0x02` | `pop`            |                 | pops last n bytes from stack |
| not  | 4 + `args.size`  | `Offset + 0x03` | `not a0, a1`     | `a0 = ~a1`      |                              |
| and  | 4 + `args.size`  | `Offset + 0x04` | `and a0, a1, a2` | `a0 = a1 & a2`  |                              |
| or   | 4 + `args.size`  | `Offset + 0x05` | `or a0, a1, a2`  | `a0 = a1 \| a2` |                              |
| xor  | 4 + `args.size`  | `Offset + 0x06` | `xor a0, a1, a2` | `a0 = a1 ^ a2`  |                              |
| shl  | 4 + `args.size`  | `Offset + 0x07` | `shl a0, a1, a2` | `a0 = a1 << a2` |                              |
| shr  | 4 + `args.size`  | `Offset + 0x08` | `shr a0, a1, a2` | `a0 = a1 >> a2` |                              |

---

# Data type related

| Primitive type | Range           |
| -------------- | --------------- |
| uint8          | `0x140 - 0x14F` |
| uint16         | `0x150 - 0x15F` |
| uint32         | `0x160 - 0x16F` |
| uint64         | `0x170 - 0x17F` |
| int8           | `0x180 - 0x18F` |
| int16          | `0x190 - 0x19F` |
| int32          | `0x1A0 - 0x1AF` |
| int64          | `0x1B0 - 0x1BF` |
| float32        | `0x1C0 - 0x1CF` |
| float64        | `0x1D0 - 0x1DF` |

| Name | Size            | Offset + ID     | Semantics        | Equals to      | Notes                        |
| ---- | --------------- | --------------- | ---------------- | -------------- | ---------------------------- |
| add  | 4 + `args.size` | `Offset + 0x00` | `add a0, a1, a2` | `a0 = a1 + a2` |                              |
| sub  | 4 + `args.size` | `Offset + 0x01` | `sub a0, a1, a2` | `a0 = a1 - a2` |                              |
| mul  | 4 + `args.size` | `Offset + 0x02` | `mul a0, a1, a2` | `a0 = a1 * a2` |                              |
| div  | 4 + `args.size` | `Offset + 0x03` | `div a0, a1, a2` | `a0 = a1 / a2` |                              |
| mod  | 4 + `args.size` | `Offset + 0x04` | `mod a0, a1, a2` | `a0 = a1 % a2` |                              |
| inc  | 4 + `arg.size`  | `Offset + 0x05` | `inc a0`         | `++a0`         |                              |
| dec  | 4 + `arg.size`  | `Offset + 0x06` | `dec a0`         | `--a0`         |                              |
| neg  | 4 + `args.size` | `Offset + 0x07` | `neg a0, a1`     | `a0 = -a1`     | only exists for signed types |
| cmp  | 4 + `args.size` | `Offset + 0x08` | `cmp a0, a1`     | `a0 <=> a1`    | writes result to flags       |

---
