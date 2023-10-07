# dtek_lab1
Lab 1 i dtek för Gustav Och William

### Assignment 1
Line 14 behövde ändras, så konstanten som adderas till $s0 är 3 istället för 1. Detta gör att syscall med v11(vilket är print character i  $a0) kommer att printa var tredje karaktär, alltså 0x30 = 0, 0x33 = 3, 0x36 = 6, osv..

Vi behöver även ändra line 16, så att $t0 laddas in med värdet 0x5d(93) istället för 0x5b(91). Detta är för att loopen ska terminata efter att den har printat W. Eftersom att loopen terminatas när $s0 och $t0 är lika, måste $t0 laddas med ett värde så att $t0 % 3 = 0. 0x5d(93) är det första talet som uppfyller dessa krav.

### Assignment 2
No saved registers were used in the created subroutine *hexasc*, the only registers that were used were temporary registers (\$t0), the argument register (\$a0), the return value register (\$v0) and the return address register (\$ra).

If the argument in \$a0 is 17, the code will use the and operator to zero everything but the 4 lsb; this will make \$a0 equal to 1. The code will then branch to the *less10* label and add 48 and 1 = 49 and that value will be used with the and operator with the hexadecimal value of 0x0000007F (to make sure only the 7 lsb are non-zero). This will do nothing in this case and the value in \$v0 will be 49. Since 49 maps to 1 in the ASCII table, that is what will be printed when the program is run with argument 17.

Argument values that are smaller than 10 will branch to the *less10* label and argument values that are bigger than 10 will branch to the *more10* label.

### Assignment 3
`time2string` PUSH and POP multiple register values of the stack. It starts off by pushing the value in the return address register (\$ra) to the stack so that the program can find its way back to line 38 where `time2string` was initially called with `jal`. The return address needs to be saved in this case because the program counter is being set to a new value when we jump and link `jal` again to the `hexasc` subroutine on line 137. The register value is then popped off the stack right before the `jr  $ra` statement on line 188 so that the program returns to the correct place in the main function after `time2string` is done (and not back up to line 137 where `hexasc` was called). \$a0, \$t0, \$t1 and \$t2 are pushed and popped before and after calling the `hexasc` function in case that subroutine uses any of those registers￼. In the case of \$a0 it needs to be saved so that `hexasc` can handle one digit at a time (the 16 bit value in \$a0 needs to be saved because `hexasc` removes all but the 4 LSBs).

All of the temporary registers used in `time2string` are not saved since temporary registers are caller-saved; meaning that they are saved by the caller and there is therefore no danger in modifying them.

Lines 143-150 assure that we branch to the correct line to handle the different digits in the time. In the case of the 5 in 16:53, that would be the *target3* block on lines 159-162.

### Assignment 4
If the value in \$a0 is 0, that would mean that we do not want any delay. Going to our delay subroutine, the counter variable would be initialized and set to zero. Then we would enter the *while* label and execute the `slti` instruction. This would set the value of \$t1 to 1, since the value in \$a0 (0) is less than 1. We would then execute the `beq` instruction and go to the *donewhile* label where we would go finally back to where we called the subroutine by way of the `jr  $ra` instruction.

For a negative number (-1), the exact same instructions would be executed as for the number 0 and there would be no delay.

### Assignment 6
It is there because the labels are local by default and can therefore not be accessed by other files (the functions are being called in other files and therefore have to be declared global).

### Assignment 7
Different specs and hardware between the computer running mars (and the simulator itself) and the chip kit.
