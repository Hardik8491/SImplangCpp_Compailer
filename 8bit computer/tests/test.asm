.text


start:

    ldi A 10
    sta %b
    ldi A 4
    sta %a
    hlt

.data

b = 0
a = 0
