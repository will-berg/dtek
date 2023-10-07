  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,9		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window

stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)
	

  # You can write your own code for hexasc here
  # Hexasc convets input values in the range 0-9 into the ascii codes for those numbers
  # (48-57) and the values 10-15 are converted to the ascii value for A-F (65-70).

hexasc:
	andi	$a0,$a0,0xF	# use and operator on argument value so only the 4 lsb are non-zero
	slti	$t0,$a0,10
	beq	$t0,1,less10
	j	more10
	nop
	less10:
		addi	$v0,$a0,48
		andi	$v0,$v0,0x7F	# use and operator on return value so only the 7 lsb are non-zero
		jr	$ra
		nop
	more10:
		add 	$v0,$a0,55
		andi	$v0,$v0,0x7F	# use and operator on return value so only the 7 lsb are non-zero
		jr	$ra
		nop
		
