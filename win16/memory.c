/* Far-memory allocator for original WinFile's directory-entry blocks.
 * Copyright (c) 2026 GeosOne. Licensed under the MIT License. */
#include "platform.h"

LPVOID W16Alloc(UINT size)
{
    HGLOBAL handle = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, size);
    LPVOID pointer;
    if (!handle) return NULL;
    pointer = GlobalLock(handle);
    if (!pointer) GlobalFree(handle);
    return pointer;
}

void W16Free(LPVOID pointer)
{
    HGLOBAL handle;
    if (!pointer) return;
    handle = (HGLOBAL)LOWORD(GlobalHandle(FP_SEG(pointer)));
    GlobalUnlock(handle);
    GlobalFree(handle);
}

UINT W16Size(LPVOID pointer)
{
    HGLOBAL handle = (HGLOBAL)LOWORD(GlobalHandle(FP_SEG(pointer)));
    DWORD size = GlobalSize(handle);
    return size > 65535UL ? 0 : (UINT)size;
}
