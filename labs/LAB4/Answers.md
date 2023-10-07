# Lab 4 - Processor Design

## Assignment 1: Arithmetic/Logical Unit (ALU)
### Explain how each of the ALU functions are defined. In particular, you need to be able to explain how subtraction works, including the use of two's complement.
The `AND` operation is performed simply by redirecting the inputs A and B to the AND gate and choosing the correct paths with the multiplexers. The `OR` operation is performed in the same way but by choosing the inputs A and B to be inputs to the OR gate in the ALU. The addition function of the ALU is performed by passing in A and B as inputs to the adder and then choosing that output with the final multiplexer. The subtraction operation is performed by passing in A and the inverse of B and a carry-in value of 1 to the adder. This will perform the subtraction A-B and if B ends up being bigger than A the value will be negative and represented in two's complement form. The `SLT` (set less than) function is performed by subtracting B from A (A-B) and then selecting the most significant bit from the result (this bit will determine if the result is negative or positive because of two's complements form) and then zero extending it so that it is 32 bits long (which is the required input length for the multiplexer). If the value is a 1, that means that the result is negative and that B is bigger than A. Otherwise, when A is bigger than B and the result is positive, the output of the ALU (when F = 111) is 0.

### How did you implement the logic for the Zero output port? Did you consider any alternatives? Be prepared to explain your design choices.
The logic was implemented by using a NOR gate with 32 1-bit inputs (a 32-bit splitter was also required for this implementation). Since a NOR gate only outputs 1 if all of the inputs are 0, this is a very simple way to implement the logic for the zero flag (albeit not very elegant). The first thing that we thought of was to use a NOR gate and we thought for some time for a more elegant solution...

### What is the purpose of the ALU? Why are several functions grouped together into one component?
Combining different arithmetic and logic operations with hardware that supports these operations into one unit/element is very helpful. It makes it clear where the arithmetic is being carried out and all of the operands can be inputs into a single unit. An ALU also saves hardware by combining all of the operations into one unit.


## Assignment 2: Register File
### Explain if the read operation or the write operation, or both operations are clocked (updated at the clock edge). Why is it implemented this way?
The write operation is clocked (the register specified by A3 is only written the value of WD3 when WE3 is set to 1 and the clock goes from 0 to 1) while the read operation is not (the values of the registers specified by A1 and A2 are propagated to RD1 and RD2 constantly; meaning that the output of the multiplexer that picks register values changes directly in response to the changes in the two inputs A1 and A2).

Since reading does not alter any memory states, we allow it to be asynchronous which helps prevent timing conflicts. If both operations are clocked and we are trying to read and write to a register at the same time (on the same rising edge of the clock), there is an uncertainty regarding what will be read from that register (the old value or the new one we want to write to it).

### Explain the semantics of reading from and writing to $0, and how you implemented this behavior.
The $0 register is currently a 32-bit register that is always zero. Since the value of the zero register is never meant to change from 0, there is currently no way to write to the register (it has no data or clock input). We still want to be able to read from the register however, which is still possible. The register is wired to the 0th input to both of the multiplexers connected to RD1 and RD2. This means that whenever A1 and/or A2 are 000, they will propagate the value 0 from the zero register to RD1 and/or RD2 respectively.

### How many bits of data can this register file store? If the address width was the same size as for a complete 32-bits MIPS processor, how many bits would such a register file store?
The register file has 8 registers which stores 32 bits of data each. This means that the register file can store 8x32 = 256 bits of data, or 256 - 32 = 224 bits of data if we are not counting the zero register. If the register file supported all of the 32 registers of a complete MIPS processor, it could store 32x32 = 1024 bits of data (or 1024 - 32 = 992 bits not counting the zero register).


## Assignment 3: Control Unit
### Explain how you have implemented the control signals for the beq instruction. Why is this a correct solution?
There are certain control signals that need to be 1 and some others that need to be 0 if the `beq` instruction is to be executed properly. These are the different control signals and the values that they need to be for the instruction to execute correctly.
**Control signal values for `beq` instruction:**

| RegWrite | RegDst | ALUSrc | ALUControl | Branch | MemWrite | MemToReg |
|----------|--------|--------|------------|--------|----------|----------|
| 0        | ?      | 0      | 110        | 1      | 0        | ?        |

The implementation is therefore correct when if the opcode for the given instruction is 4 (the opcode for the `beq` instruction), all of the control signals have the above values. In our implementation, we first added a fourth comparator that checks if the opcode is 4 and we want to execute a `beq` instruction. If it is 4, the comparator will output 1 and the ALUControl will be set to 6 with the use of a multiplexer and two constants (all of the supported instructions either subtract or add so this is enough). The wire from the comparator is also connected to the branch output signal, which will then be activated if an opcode of 4 is detected. The MemWrite control signal was also added, and since it needs to be 0 for all of the supported instructions, is connected to a constant with the value 0. When now inputting an instruction with an opcode of 4, we can see that all of the control signals have the correct value for a `beq` instruction.

### Be prepared to explain why the RegDst control signal or the ALUSrc signal is hooked up to certain signals. You should be prepared to explain this using the figure provided.
The RegDst control signal is connected to the multiplexer that chooses between bits 20:16 or 15:11. This because this multiplexer determines what register to write to (the register specified by A3), or the register destination if you will. R-type instructions write to the register specified by bits 15:11, so the RegDst signal needs to be 1 for all R-type instructions. The ALUSrc control signal is connected to the multiplexer that determines the input to the ALU. It determines if we want to do an operation on an immediate and a register value or two register values. Some I-type instructions add a register value with an immediate (`addi` for example) and that control signal needs to be 1.


## Assignment 4: Data Path
### Explain how the bit selection works for the alternatives that are controlled by the RegDst control signal. Which instructions are using what logic and why?
RegDst controls what register the result of the ALU should be written to (if a result should be written to a register at all). This register is determined by the target register field of the instruction machine code. Since the rd field is stored in bits 20:16 for some instructions and 15:11 (18:16 and 13:11 respectively for our 8 available registers) for others. Specifically, R-type instructions store the target register in the rd field and other instructions store it in the rt field.

### Explain how the beq instruction is implemented, how the address is calculated and how the signals are controlled by the control unit.
The branch control signal is 1 and is connected as one of the inputs to an AND gate. The other input to the gate is connected to the zero flag of the ALU. If the zero flag is raised, that means that the contents of the two registers given in the `beq` instruction are equal. This is because two identical numbers minus each other is 0. When that happens, both the inputs to the AND gate will be 1 and the output of the AND gate will be 1 as well. That output is connected to another multiplexer just before the PC. If that multiplexer select signal is 1 (which happens when the branch control signal is 1 and the zero flag is raised), the next instruction will not be PC + 4 (which is the first input to the multiplexer), but it will instead be the immediate value specified in the `beq` instruction multiplied by 4 added with PC + 4. This is the exact formula for calculating the branch target address (BTA), and the PC will be set to that address.

## Assignment 5: Factorial Function in Assembler
