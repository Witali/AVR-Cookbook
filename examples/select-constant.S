.text
.global select_constant
.type select_constant, @function
select_constant:
    ; AVR-GCC ABI: fallback/result in r24, validated selector in r22.
#if VARIANT == 1
    sbrc r22, 0
#elif VARIANT == 0
    tst r22
    breq 1f
#else
#error Unsupported VARIANT
#endif
    ldi r24, 0xb6
1:  ret
.size select_constant, .-select_constant
