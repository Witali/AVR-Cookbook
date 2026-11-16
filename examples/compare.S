.text
.global words_equal
.type words_equal, @function
words_equal:
    ; avr-gcc ABI: left in r25:r24, right in r23:r22, result in r24.
    cp r24, r22
#if VARIANT
    cpc r25, r23
#else
    brne 1f
    cp r25, r23
#endif
    brne 1f
    ldi r24, 1
    ret
1:  ldi r24, 0
    ret
.size words_equal, .-words_equal
