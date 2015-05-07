/
/   int testandset(int *lockPtr)
/
/   Using the word pointed to by lockPtr, indivisibly test and
/   set its value.  If the lock is held the function returns 1, if not held
/   the function returns 0.  In either case the word pointed to by
/   lockPtr is set to 1.
/
/   Example of usage:
/
/        int aLock = 0;
/
/        while (testandset(&aLock)) /*spin*/;
/

	.file	"x86_testandset.c"
	.version	"01.01"
	.xstabs	".stab.index", "Xt ; V=3.0 ; R=3.0",0x3c,0,0,0
	.xstabs	".stab.index", "/home/wayne/public_html/cpsc322/threads; /opt/SUNWspro/bin/../SC3.0.1/bin/cc -S  x86_testandset.c -W0,-xp",0x34,0,0,0
/ASM
	.ident	"acomp: PC3.0.1 21 Jan 1995 Sun C 3.0.1"
/ASMEND
	.ident	"hc2.6c -O0 -386 -fstrict -on SHAREABLE x86_testandset.c\n"
	.section	.text
	.align	4
.L1_.text:

#====================
# testandset
#--------------------
	.align	4
	.align	4
	.globl	testandset
testandset:
#   Save registers
	pushl	%ebp
	movl	%esp,%ebp
	subl	$4,%esp

	movl	8(%ebp),%eax   # Fetch the address of the lock

#	Indivisibly test and set the lock bit
	lock                  # This is not an instruction, but rather
                              #   tells the assembler that a bit should be
                              #   set in the next instruction's code that it
                              #   should be "indivisible" (atomic).
	btsl  $0,(%eax)       # Test bit 0 of lock, AND set bit to "1".
	jc	  held        # If bit was already 1, jump to return "true"

	movl    $0, -4(%ebp)  # Bit not set (lock not held), return "false" (0)
	jmp   done

held:
	movl    $1, -4(%ebp)  # Lock held, return "true" (1)

# Restore registers and return
done:
	movl	-4(%ebp), %eax 
	movl	%ebp,%esp
	popl	%ebp
	ret                   # Return
	
	.align	4
	.type	testandset,@function
	.size	testandset,.-testandset
	.stabs	"",0x62,0,0,0x0
