.text


start:

    push A
    ldi A 1
    sta %a
    pop A

    push A
    ldi A 2
    sta %b
    pop A

    lda  %a
    push A
    lda %b
    mov B A
    pop A
    add
    sta %c
    hlt

.data

a = 0
b = 0
c = 0
