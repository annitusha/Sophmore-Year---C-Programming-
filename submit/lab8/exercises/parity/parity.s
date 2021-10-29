	.text
	.globl get_parity
#edi contains n	
get_parity:
	#@TODO: add code here to set eax to 1 iff edi has even parity
	testl	%edi, %edi
	jpe	.jump1
	xorl	%eax, %eax
	ret

	.jump1:
	xorl	%eax, %eax
	movl	$1,	%eax
	ret
	
