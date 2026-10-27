#include <Windows.h>
#include <winerror.h>
#include <stdio.h>

BOOL WINAPI CreateProcessWithBlockDllPolicy(
    IN LPSTR lpProcessPath, 
    OUT DWORD* dwProcessId, 
    OUT HANDLE* hProcess,
    OUT HANDLE* hThread
) {
    STARTUPINFOEXA SiEx = { 0 };
    PROCESS_INFORMATION Pi = { 0 };
    SIZE_T sAttrSize = NULL;
    if (lpProcessPath == NULL) return false;
    RtlSecureZeroMemory(&SiEx, sizeof(STARTUPINFOEXA));
    RtlSecureZeroMemory(&Pi, sizeof(PROCESS_INFORMATION));
    SiEx.StartupInfo.cb = sizeof(STARTUPINFOEXA);
    SiEx.StartupInfo.dwFlags = EXTENDED_STARTUPINFO_PRESENT; 
}