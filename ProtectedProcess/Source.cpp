#include <Windows.h>
#include <winerror.h>
#include <stdio.h>

BOOL WINAPI CreateProcessWithBlockDllPolicy(
    IN LPSTR lpProcessPath, 
    OUT DWORD* dwProcessId, 
    OUT HANDLE* hProcess,
    OUT HANDLE* hThread
) {
  
}