print:
    lodsb
    test al, al
    jz   done
    mov  ah, 0x0E
    int  0x10
    jmp  print
done: ret