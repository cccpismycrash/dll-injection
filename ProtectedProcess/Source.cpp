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

    InitializeProcThreadAttributeList(NULL, 1, NULL, &sAttrSize); // записываем в sAttrSize размер атрибутов в байтах    
    LPPROC_THREAD_ATTRIBUTE_LIST pAttrBuf = 
        (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sAttrSize); // выделяем память на куче
    if (!InitializeProcThreadAttributeList(pAttrBuf, 1, NULL, &sAttrSize)) {
        printf("[!] InitializeProcThreadAttributeList Failed With Error : %d \n", GetLastError());
        return false;
    }
}