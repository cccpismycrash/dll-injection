
#include <Windows.h>
#include <winerror.h>

int main()
{
    // You also specify the complete path.
    LoadLibraryW(L"VirusDLL.dll");
    return 0;
}