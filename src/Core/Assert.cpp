#include <pch.h>
#include "Assert.h"

static const char* GetDX12ErrorString(HRESULT hr)
{
    switch (hr)
    {
        case DXGI_ERROR_DEVICE_HUNG:           return "GPU stopped responding";
        case DXGI_ERROR_DEVICE_REMOVED:        return "GPU was physically removed or driver crashed";
        case DXGI_ERROR_DEVICE_RESET:          return "GPU was reset by the driver";
        case DXGI_ERROR_DRIVER_INTERNAL_ERROR: return "Driver encountered an internal error";
        case DXGI_ERROR_INVALID_CALL:          return "DirectX API called with invalid parameters";
        case DXGI_ERROR_UNSUPPORTED:           return "Feature is not supported by the GPU";
        case E_OUTOFMEMORY:                    return "System or GPU ran out of memory";
        case E_INVALIDARG:                     return "Invalid argument passed to DirectX function";
        default:                               return nullptr;
    }
}

void FatalError(const char* file, int line, const char* expr, const char* fmt, ...)
{
    char msg[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    char buffer[2048];
    snprintf(buffer, sizeof(buffer), 
        "%s\n"
        "    In %s:%d: '%s'",
        msg, file, line, expr
    );
#ifdef _DEBUG
    OutputDebugStringA("[FATAL] ");
    OutputDebugStringA(buffer);
    OutputDebugStringA("\n\n");
    __debugbreak();
#else
    MessageBoxA(nullptr, buffer, "Fatal Error", MB_OK | MB_ICONERROR);
#endif
    ExitProcess(EXIT_FAILURE);
}

void GetWin32ErrorDesc(DWORD error, char* buffer, size_t bufSize)
{
    DWORD result = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buffer,
        bufSize,
        nullptr
    );

    while (result > 0 && (buffer[result - 1] == '\n' || buffer[result - 1] == '\r'))
        buffer[--result] = '\0';

    if (result == 0)
        snprintf(buffer, bufSize, "No description available for this error");
}

void GetDX12ErrorDesc(HRESULT hr, char* buffer, size_t bufSize)
{
    const char* details = GetDX12ErrorString(hr);
    if (details)
        snprintf(buffer, bufSize, "%s", details);
    else
        GetWin32ErrorDesc(static_cast<DWORD>(hr), buffer, bufSize);
}