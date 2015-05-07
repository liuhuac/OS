# CpSc 322 -- thread switch function
#
#  Prototype:
#
#     void doSwitch(tcb_t *nexttcb, tcb_t *prevtcb);
#
#     NOTE: This code is dependent upon the stack pointer being
#           in the first word of the tcb_t structure.

        .file   "doSwitch.s"
        .text
.globl doSwitch
        .type   doSwitch, @function
doSwitch:
        pushl   %ebp
        movl    %esp, %ebp
#       Save rest of previous threads's registers
        pushfl          # save flags register
        pushl   %eax    # and rest of general registers
        pushl   %ebx
        pushl   %ecx
        pushl   %edx
        pushl   %esi
        pushl   %edi

#       Save current stack pointer for previous thread
        movl    12(%ebp), %eax  # Pick up address of 2nd parameter (prevtcb)
        movl    %esp, (%eax)    # And save stack pointer into prevtcb's pcb.

#       Now load saved stack pointer for next thread
        movl    8(%ebp), %eax   # Pick up address of 1std parameter (newtcb)
        movl    (%eax), %esp    # Ta-daa!!! This did the thread switch

#       The following is the equivalent of "re-enabling interrupts"
#       Reset signal handler: "sigaction(vtSigNum, &vtAlrmAction, NULL);"
        subl    $24, %esp
        movl    vtSigNum, %eax
        movl    $0, 8(%esp)
        movl    $vtAlrmAction, 4(%esp)
        movl    %eax, (%esp)
        call    sigaction
#       Reset the timeslice:  "setalarm(quantum);"
        movl    quantum, %eax
        movl    %eax, (%esp)
        call    setalarm

        addl    $24, %esp

#       Now restore registers for next thread from its stack.
        popl    %edi   # restore general registers
        popl    %esi
        popl    %edx
        popl    %ecx
        popl    %ebx
        popl    %eax
        popfl           # and restore flags register
        popl    %ebp

#       And continue execution with next thread
        ret           
        .size   doSwitch, .-doSwitch
