#pragma once

/* Fatal error reporting */
void FatalError(const char* file, int line, const char* expr, const char* fmt, ...);

void GetWin32ErrorDesc(DWORD error, char* buffer, size_t bufSize);
void GetDX12ErrorDesc(HRESULT hr, char* buffer, size_t bufSize);

#define INTERNAL_FATAL(expr, fmt, ...) \
    FatalError(__FILE__, __LINE__, expr, fmt, __VA_ARGS__)

/* Assert macro */
#ifdef _DEBUG
    #define ASSERT(expr, msg)                                                                   \
        do {                                                                                    \
            if (!(expr))                                                                        \
                INTERNAL_FATAL(#expr, "[CORE] Assertion failure: %s", msg);                     \
        } while (0)
#else
    #define ASSERT(cond, msg) ((void) 0)
#endif

/* Failure check macros */
#define CHECK(expr, msg)                                                                        \
    do {                                                                                        \
        if (!(expr))                                                                            \
            INTERNAL_FATAL(#expr, "[CORE] %s", msg);                                            \
    } while (0)

#define WIN32_CHECK(expr, msg)                                                                  \
    do {                                                                                        \
        if (!(expr))                                                                            \
        {                                                                                       \
            DWORD _error = GetLastError();                                                      \
            char _desc[512];                                                                    \
            GetWin32ErrorDesc(_error, _desc, sizeof(_desc));                                    \
            INTERNAL_FATAL(#expr, "[WIN32] %s: %s (error code 0x%08x)", msg, _desc, _error);    \
        }                                                                                       \
    } while (0)

#define DX12_CHECK(expr, msg)                                                                   \
    do {                                                                                        \
        HRESULT _hr = (expr);                                                                   \
        if (FAILED(_hr))                                                                        \
        {                                                                                       \
            char _desc[512];                                                                    \
            GetDX12ErrorDesc(_hr, _desc, sizeof(_desc));                                        \
            INTERNAL_FATAL(#expr, "[DX12] %s: %s (HRESULT 0x%08x)", msg, _desc, _hr);           \
        }                                                                                       \
    } while (0)