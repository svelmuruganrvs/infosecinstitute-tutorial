#include <windows.h> 
#include <stdio.h> 
#include <winbase.h> 
int main(int argc, char *argv[]) {

//WlanOpenHandle Pointers
 int wlohreta;
 int wlohretb;
 int plohreta = &wlohreta;
 int plohretb = &wlohretb;

//WlanSetProperty pointers 1
 int one = 1;
 int ptone = &one;
 int nulls = NULL;
 int pnull = &nulls;

//AND function pointers...
 typedef void (*pfunc)(); 
 HANDLE hdll; 
 hdll = LoadLibrary("wlanapi.dll"); 
 pfunc WlanOpenHandle = (pfunc)GetProcAddress(hdll, 29);
 pfunc WlanSetP = (pfunc)GetProcAddress(hdll, "WlanHostedNetworkSetProperty");
 pfunc WClose = (pfunc)GetProcAddress(hdll, "WlanCloseHandle");

/**
		Things we need to at least sort of do in ASM
	WlanOpenHandle(0x2, 0x0, wlohreta, wlohretb)
        WlanSetProperty(0x1, 0x3, 0x4, ptone, ptnull, 0x0)
        WlanSetProperty(0x1, 0x0, 0x28, ptssid, ptnull, 0x0)
        WlanSetSecondaryKey(0x1, 0xA, ptasciikey, 0x1, 0x1, ptnull, 0x0)
        WlanCloseHandle(0x1, 0x0)
**/

__asm__(
//Open Handle
"push %[pab];"
"push %[paa];"
"push $0x00;"
"push $0x02;"
"call %[woh];"
:
:[paa] "r" (plohreta), [pab] "r" (plohretb), [woh] "r" (WlanOpenHandle)
);

__asm__(

"push $0x0;"
"push %[ptn];"
"push %[pto];"
"push $0x04;"
"push $0x03;"
"push $0x01;"
"call %[wsp];"
:
:[wsp] "r" (WlanSetP), [ptn] "r" (pnull), [pto] "r" (ptone)
);

__asm__(
//Wlan close handle
"push $0x0;"
"push $0x1;"
"call %[wch]"

:
:[wch] "r" (WClose)
);

}
//  So now you should be able to use in-line AES from openssl or your own lib for self modifying code with a little work..




