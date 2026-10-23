/* Tests execute the ported Microsoft modules, not copies of their logic.
 * Copyright (c) 2026 GeosOne. Licensed under the MIT License. */
#include "platform.h"
#include "../src/wfdocb.h"
#include "../src/wfmem.h"
#include <stdio.h>

HINSTANCE hAppInstance;
TCHAR szDecimal[] = ".";
TCHAR szInternational[] = "intl";
static FILE *log_file;
static int failures;

static void check(int okay, const char *name)
{
    fprintf(log_file, "%s %s\n", okay ? "PASS" : "FAIL", name);
    fflush(log_file);
    if (!okay) failures++;
}

static void names(void)
{
    char result[300];
    struct { char text[5]; char sentinel[4]; } small;
    check(!I_LFNCanon(0, "C:/TOOLS/Test.txt", result) &&
        !lstrcmp(result, "C:\\TOOLS\\Test.txt"), "original path canonicalization");
    check(I_LFNCanon(0, "C:\\bad|name", result) == ERROR_INVALID_PARAMETER,
        "original invalid-name rejection");
    check(I_LFNCanon(0, "C:\\*.TXT", result) == ERROR_INVALID_PARAMETER,
        "original literal wildcard rejection");
    check(!I_LFNCanon(1, "C:\\*.TXT", result), "original wildcard-mask acceptance");
    check(!I_LFNEditName("report.txt", "*.bak", result, sizeof(result)) &&
        !lstrcmp(result, "report.bak"), "original wildcard rename");
    check(!I_LFNEditName("report.txt", "???.bak", result, sizeof(result)) &&
        !lstrcmp(result, "rep.bak"), "original question-mark rename");
    _fmemset(&small, 0x5a, sizeof(small));
    check(I_LFNEditName("report.txt", "*.bak", small.text, sizeof(small.text)) ==
        ERROR_INVALID_PARAMETER && small.sentinel[0] == 0x5a &&
        small.sentinel[3] == 0x5a, "rename respects short buffer boundary");
}

static void entries(void)
{
    LPXDTALINK first = MemNew(), last, link, clone;
    LPXDTA entry;
    unsigned i, count;
    char expected[80];
    check(first != NULL, "original directory allocation");
    if (!first) return;
    last = first;
    for (i = 0; i < 1500; i++) {
        entry = MemAdd(&last, 70, 12);
        if (!entry) break;
        wsprintf(expected, "File %04u with a deliberately long directory entry.txt", i);
        lstrcpy(MemGetFileName(entry), expected);
        lstrcpy(MemGetAlternateFileName(entry), "FILE.TXT");
        entry->qFileSize.LowPart = i;
        entry->qFileSize.HighPart = 0;
        MemLinkToHead(first)->dwEntries++;
    }
    check(i == 1500 && FP_SEG(first) != FP_SEG(last),
        "directory spans far segments and exceeds 64 KiB");
    check(MemAdd(&last, 65535U, 65535U) == NULL,
        "oversized entry cannot wrap 16-bit allocation");
    check(MemAdd(&last, 2000, 0) == NULL,
        "oversized entry cannot overrun fixed directory block");
    clone = MemClone(first);
    check(clone && clone != first, "original directory clone");
    if (clone) {
        check(MemLinkToHead(clone)->dwEntries == 1500,
            "clone preserves directory metadata");
        MemDelete(first);
        link = clone;
        entry = MemFirst(link);
        for (count = 0; entry && count < 1500; count++) {
            wsprintf(expected, "File %04u with a deliberately long directory entry.txt", count);
            if (lstrcmp(MemGetFileName(entry), expected) ||
                lstrcmp(MemGetAlternateFileName(entry), "FILE.TXT") ||
                entry->qFileSize.LowPart != count) break;
            entry = MemNext(&link, entry);
        }
        check(count == 1500 && !entry && !link,
            "clone survives original free and iterates across all far blocks");
        MemDelete(clone);
    } else MemDelete(first);
    MemDelete(NULL);
}

static void sizes(void)
{
    LARGE_INTEGER size;
    char text[80];
    size.QuadPart = 999;
    ShortSizeFormatInternal(text, size);
    check(!lstrcmp(text, "999 bytes"), "original byte-size format");
    size.QuadPart = 1536;
    ShortSizeFormatInternal(text, size);
    check(!lstrcmp(text, "1.50 KB"), "original fractional kilobyte format");
    size.LowPart = 0;
    size.HighPart = 1;
    ShortSizeFormatInternal(text, size);
    check(!lstrcmp(text, "4.00 GB"), "original 64-bit size format above 4 GiB");
}

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show)
{
    (void)previous; (void)command; (void)show;
    hAppInstance = instance;
    log_file = fopen("C:\\WFPORT.LOG", "w");
    if (!log_file) return 2;
    names();
    entries();
    sizes();
    fprintf(log_file, "RESULT %s\n", failures ? "FAIL" : "PASS");
    fclose(log_file);
    return failures ? 1 : 0;
}
