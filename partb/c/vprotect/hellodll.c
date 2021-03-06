#include <windows.h> 
#include <stdio.h> 
#include "hellodll.h" 
#include <winbase.h> 

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) { 

    switch (fdwReason) { 
	case DLL_PROCESS_ATTACH: { 
             break; 
		} 

        case DLL_PROCESS_DETACH: {
            break; 
		} 

        case DLL_THREAD_ATTACH: { 
            break; 
		} 

        case DLL_THREAD_DETACH: { 
            break; 
		} 
    } 

	return TRUE; 
} 

EXPORT void hello(void) {
 DWORD oldprotect = NULL; 
 int vpretval; 

 typedef void (*pfunc)(); 
 HANDLE hdll; 
 pfunc vprotectaddy; 
 hdll = LoadLibrary("kernel32.dll"); 
 vprotectaddy = (pfunc)GetProcAddress(hdll, "VirtualProtect");
// DWORD oldprotect = NULL; 
 int oldprot = &oldprotect;
/**		Things we need to at least sort of do in ASM
		DWORD baseaddress - we need to save eip
		DWORD endaddress - we need to force eip to the end of the function and save eip
		unsigned int memlen - we need to do a little math
		DWORD newprotect = 0x40; - we need to set
		DWORD oldprotect = NULL;  - we need to pass in the pointer
		int vpretval = VirtualProtect(  baseaddress,  memlen,  newprotect,  &oldprotect); 
**/

__asm__(
"jmp Label_passed_start;"
/*Little ASM to get current EIP  */
"Label_eip:"
"mov (%%esp), %%eax;"
"ret;"
"Label_passed_start:"

/*Push in reverse order*/
"push %[old];"
/*pointer to old protection variable*/
"push $0x40;"
/*new protection option*/
"call Label_getend;"
"push %%eax;"
"subl %%eax, %%edx;"
"pop %%eax;"
/*we need to get base and end before we know length so...*/
"push %%edx;"
/*len*/
"push %%eax;"
/*base address*/
/*"addl $0x10, %%esp;"*/
"call %[vprotect];"
"movl %%eax, %[vpret];"
"call Label_eip;"
"nop;"
"nop;"
"nop;"
"nop;"
"xorl %%eax, (%%eax);"
:/*Output */[vpret] "=r" (vpretval)
:/*Input */[vprotect] "r" (vprotectaddy), [old] "r" (oldprot)
:/*Clobbered */ "eax", "edx"
);
/**the four nops was to get us away from old eip before the XOR**/
/**decoder would be here**/

/**payload would be here**/
			printf("Return value:    0x%8x\n", vpretval); 
			printf("Old protection:  0x%8x\n", oldprotect); 

__asm__(
"jmp Label_passed_end;"
/*Little ASM to springboard to geteip so we actually get end */
"Label_getend:"
"call Label_eip;"
"movl %eax, %edx;"
"mov (%esp), %eax;"
"ret;"
"Label_passed_end:"
);
/**re-encoder would be here**/

}

