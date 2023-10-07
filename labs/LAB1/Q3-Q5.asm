			  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,1000
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

# you can write your code for subroutine "hexasc" below this line
# when hexasc is done, $v0 contains the ascii char for the input
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


delay:
	addi	$t0,$0,0	# initialize counter variable
	while:
		slti	$t1,$a0,1	# check condition 
		beq	$t1,1,donewhile
		nop
		addi	$a0,$a0,-1	# decrement variable
		for:
			beq	$t0,10000,donefor	# check condition
			nop
			addi	$t0,$t0,1	# increment variable
			j	for
			nop
		donefor:
		j	while
		nop
	donewhile:
	jr	$ra
	nop

# time2string converts time-info (mytime) into a string of printable characters, with a null-byte as an end-of-string-marker.
time2string:
	PUSH	($ra)	# save ra on stack 
	# load ascii values for colon and nullbyte
	li	$t1,0x3A
	li	$t2,0x00
	andi	$a1,$a1,0xFFFF	# use and operator on argument so only the 16 lsb are non-zero
	add	$t0,$0,$0	# initialize counter variable
	move	$t3, $a0	
	move 	$a0, $a1 	
	j	loop
	nop
	loop:
		# ensure that the characters are stored in the correct byte location 
		beq	$t0,2,colon
		nop
		beq	$t0,5,nullbyte
		nop
		PUSH	($a0)
		PUSH	($t0)
		PUSH	($t1)
		PUSH	($t2)
		jal	hexasc
		nop
		POP	($t2)
		POP	($t1)
		POP	($t0)
		POP	($a0)
		beq	$t0,0,target4
		nop
		beq	$t0,1,target3
		nop
		beq	$t0,3,target2
		nop
		beq	$t0,4,target1
		nop
		target1:
			sb	$v0,0($t3)
			j	loop2
			nop
		target2:
			sb	$v0,1($t3)
			j	loop2
			nop
		target3:
			sb	$v0,3($t3)
			beq	$v0,0x30,maybeding
			nop
			j	noding
			maybeding:
				beq	$s1,0x30,yesding
				nop
				j	noding
				yesding:
					addi	$t4,$0,0x474E4944
					sw	$t4,0($t3)
					sb	$t2,4($t3)
					POP	($ra)
					jr	$ra
					nop
			noding:
				j	loop2
				nop
		target4:
			#beq	$v0,0x39,nine
			move	$s1,$v0
			sb	$v0,4($t3)
			j	loop2
			nop
			#nine: 
			#	addi	$t4,$0,0x454E494E 
			#	sw	$t4,4($t3)
			#	sb	$t2,8($t3)	# make room for 'NINE' by placing nullbyte in byte 8 
			#	POP	($ra)
			#	jr	$ra
			#	nop
		loop2:
			srl	$a0,$a0,4
			addi	$t0,$t0,1
			j	loop
			nop
	colon:
		sb	$t1,2($t3)
		addi	$t0,$t0,1
		j	loop
		nop
	nullbyte:
		#move	$t4,$t3
		#andi	$t4,0xFFFFFF
		#beq	$t4,0x3030,ding
		sb	$t2,5($t3)
		#j	noding
		#ding:
		#	addi	$t5,$0,0x474E4944
		#	sw	$t5,0($t3)
		#	sb	$t2,4($t3)
		#noding:
			POP	($ra)
			jr	$ra
			nop

			
		
