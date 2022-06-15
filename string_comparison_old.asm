; https://retroscience.net/x64-assembly.html									- x64 Assembly
; https://en.wikibooks.org/wiki/X86_Assembly/SSE#IMM8_control_byte_description 	- how to use imm8
; https://www.officedaytime.com/simd512e/simdimg/str.php?f=pcmpistri

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
; pcmpestri Performs a packed compare of two explicit-length text strings; returns the index result in ECX.
; pcmpestrm Performs a packed compare of two explicit-length text strings; returns the mask result in XMM0.
; pcmpistri Performs a packed compare of two implicit-length text strings; returns the index result in ECX.
; pcmpistrm Performs a packed compare of two implicit-length text strings; returns the mask result in XMM0.
;
; Execution of the explicit-length instructions pcmpestri and pcmpestrm is essentially the same, except that the 
; text string fragment lengths must be specified using registers EAX and EDX.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Control Option		Value	Description
; Data Format [1:0]		00		Packed unsigned bytes
;						01		Packed unsigned words
;						10		Packed signed bytes
;						11		Packed signed words
;
; Aggregation			00		Equal any 		(match characters)
;						01		Equal range 	(match characters in range)
;						10		Equal each		(string compare)
;						11		Equal ordered	(substring search)
;
; Polarity				00		Copy IntRes1 to IntRes2.
;						01		Invert each bit of IntRes1 and set to IntRes2.
;						10 		Copy IntRes1 to IntRes2.
;						11		If the corresponding BYTE / WORD of (2) xmm2/m128 b is valid invert the bit of IntRes1 and set to IntRes2. else copy the bit of IntRes1 to IntRes2.
;
; Output format			0 		pcmpistri - ECX = index of least significant set bit in IntRes2
;						1 		pcmpistri - ECX = index of most significant set bit in IntRes2
;						0 		pcmpistrm - IntRes2 saved as a bit mask in low-order bits of XMM0 (high-order bits are zeroed)
;						1 		pcmpistrm - IntRes2 saved as byte/word mask in XMM0
;
; Bits are set from right to left, for example (00010100): 	00						01				01					00
;															^^ Output format		^^ Polarity		^^	Aggregation		^^ Data format bits

.code	
	; Packed Compare Implicit Length Strings, Return Mask
	; extern "C" void PCMPISTRM_ASM(char *FirstString, char *SecondString);
	PCMPISTRM_ASM proc
		movdqu xmm2, xmmword ptr [rcx]
		movdqu xmm1, xmmword ptr [rdx]
		pcmpistrm xmm2, xmm1, 0000b
		ret

	PCMPISTRM_ASM endp
		end
