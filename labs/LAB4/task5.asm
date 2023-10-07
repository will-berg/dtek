addi $a0, $0, 4 	# adding the input (n) to a0
addi $v0, $0, 1 	# storing the result in v0, starting at 1 which is the basecase 0!
addi $v1, $a0, 1 	# keeping track of k in v1, starting at n + 1
addi $a2, $0, 1 	# initiating the loop counter in a2, starting at 1

loop: 	beq $a2, $v1, done 	# branch if i = k
	addi $a1, $0, 0 	# set the product = 0
	addi $a3, $0, 1 	# initiate inner loop counter j in a3, starting at 1
	addi $a2, $a2, 1 	# increment i, i++
	
	
	mulloop: 	beq $a3, $a2, next 	# branch if j = i
			add $a1, $a1, $v0 	# add the product and the result, store in result
			addi $a3, $a3, 1 	# increment j, j++
			beq $0, $0, mulloop 	# go back to inner loop, without jump instruction

next: 	addi $v0, $a1, 0 	# set result to product
     	beq $0, $0, loop 	# go back to outer loop, without jump
     
done: 	beq $0, $0, done 	# jump to itself endlessly, nop instruction