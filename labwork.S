  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
   
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global hexasc
.global enable_interrupt
.global delay
.global time2string
.global led_test


.data

.text
 

delay:
	PUSH $ra
	PUSH $a0
	PUSH $s1
	PUSH $s2
	PUSH $s3
	PUSH $s4
	
	addi $s1, $0, 4500 # constant
	
	add $s4, $0, $0 # while true or false for while
	add $s2, $0, $0
	
while:
	slti $s4, $a0, 1
	bne $s4, $0, whiledone
	addi $a0, $a0, -1 
	
	add $s3, $0, $0 # i saknade komma
for:
	slt $s2, $s1, $s3 	
	bne $s2, $0, fordone
	addi $s3, $s3, 1 
	
	j for
fordone:
	j while
whiledone:
	POP $s4
	POP $s3
	POP $s2
	POP $s1
	POP $a0
	POP $ra
	jr $ra
	nop 
 
hexasc:
	PUSH $s0
	PUSH $s1
	addi $s0, $0, 0x0
	
	slti $s0, $a0, 0x0A
	bne $s0, $0, hexasc0to9
	addi $v0, $a0, 0x27
	
	POP $s1
	POP $s0
	jr $ra
	
	hexasc0to9:
	addi $v0, $a0, 0x30
	
	POP $s1
	POP $s0
	jr $ra


enable_interrupt:
	ei
	jr $ra
	nop
	
	
	

	

	

