#include <Windows.h>
#include <winerror.h>
#include <stdio.h>

BOOL WINAPI CreateProcessWithBlockDllPolicy(
    IN LPWSTR lpProcessPath, 
    OUT DWORD* dwProcessId, 
    OUT HANDLE* hProcess,
    OUT HANDLE* hThread
) {
    STARTUPINFOEXW SiEx = { 0 };
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
        return FALSE;
    }

    DWORD64 dwPolicy = PROCESS_CREATION_MITIGATION_POLICY_BLOCK_NON_MICROSOFT_BINARIES_ALWAYS_ON;
    if (!UpdateProcThreadAttribute(
        pAttrBuf, 
        NULL, 
        PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY, 
        &dwPolicy, 
        sizeof(DWORD64), 
        NULL, 
        NULL)) {
        printf("[!] UpdateProcThreadAttribute Failed With Error : %d \n", GetLastError());
        return FALSE;
    }
    SiEx.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)pAttrBuf;

    if (!CreateProcessW(
        NULL,
        lpProcessPath,
        NULL,
        NULL,
        FALSE,
        EXTENDED_STARTUPINFO_PRESENT,
        NULL,
        NULL,
        &SiEx.StartupInfo,
        &Pi)) {
            printf("[!] CreateProcessW Failed With Error : %d \n", GetLastError());
            return FALSE;
    }

    *dwProcessId = Pi.dwProcessId;
    *hProcess = Pi.hProcess;
    *hThread = Pi.hThread;

    DeleteProcThreadAttributeList(pAttrBuf);
    HeapFree(GetProcessHeap(), 0, pAttrBuf);

    if (
        *dwProcessId != NULL &&
        *hProcess != NULL &&
        *hThread != NULL
    ) return TRUE;
    else return FALSE;
}

int main(int argc, char* argv[]) {
    DWORD dwProcessId = NULL;
    HANDLE hProcess = NULL,
        hThread = NULL;

    PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY lpBuffer = { 0 };

    GetProcessMitigationPolicy(
        GetCurrentProcess(),
        ProcessSignaturePolicy,
        &lpBuffer,
        sizeof(lpBuffer)
    );

    if (lpBuffer.MicrosoftSignedOnly == 1) {
        printf("[+] Process Is Now Protected With The Block Dll Policy \n");

        int i = 0;
        while (true)
        {
            printf("Processing - %d\n", i++);
            Sleep(1000);
        }
        return 0;

    } else {
        WCHAR pcFilename[MAX_PATH * 2];
        if (!GetModuleFileNameW(
            NULL,
            (LPWSTR)&pcFilename,
            MAX_PATH * 2
        )) {
            printf("[!] GetModuleFileNameW Failed With Error : %d \n", GetLastError());
            return -1;
        }

        DWORD dwBufferSize = (DWORD)(lstrlenW(pcFilename) + 0xFF);
        WCHAR* pcBuffer = (WCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
        if (!pcBuffer) return -1;

        swprintf_s(pcBuffer, dwBufferSize, L"%s", pcFilename);
        if (!CreateProcessWithBlockDllPolicy(
            pcBuffer,
            &dwProcessId,
            &hProcess,
            &hThread        
        )) {
            return -1;
        }

        HeapFree(GetProcessHeap(), 0, pcBuffer);
        printf("[i] Process Created With PID %d \n", dwProcessId);
    }
}