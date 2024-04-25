// Собирать
// Linux
// clang src/test.c -o test
// или
// gcc src/test.c -o test
// Windows developer powershell
// cl.exe /utf-8 src/test.c /Fetest.exe
//
// Запускать
// Linux
// ./test
// Windows developer powershell
// .\test.exe

#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#ifdef _MSC_VER
#define FILE_WRITE "w, ccs=UTF-8"
#define WFILE_WRITE L"w, ccs=UTF-8"
#else
#define FILE_WRITE "w"
#define WFILE_WRITE L"w"
#endif

#ifndef MAX_FILE_PATH
#define MAX_FILE_PATH 1024
#endif

FILE *wide_fopen(wchar_t const *name, wchar_t const *mode) {
	#ifdef _MSC_VER
	return _wfopen(name, mode);
	#else
	// usually there is some character limits in the os/fs
	// if that's not the case - feel free to redefine MAX_FILE_PATH
	// or even add heap-allocated temporary strings instead of mbname, mbmode
	char mbname[MAX_FILE_PATH];
	char mbmode[MAX_FILE_PATH];
    mbstate_t state = {0};
    size_t len = 1 + wcsrtombs(NULL, &name, 0, &state);
	if (len >= MAX_FILE_PATH) return NULL;
    if (wcsrtombs(mbname, &name, len, &state) == (size_t)-1) return NULL;
	state = (mbstate_t){0};
    len = 1 + wcsrtombs(NULL, &mode, 0, &state);
	if (len >= MAX_FILE_PATH) return NULL;
    if (wcsrtombs(mbmode, &mode, len, &state) == (size_t)-1) return NULL;
	FILE *fd = fopen(mbname, mbmode);
	fwide(fd, 1);
	return fd;
	#endif
}

int main(void) {
	setlocale(LC_ALL, "ru_RU.utf8");

#ifdef _WIN32
	_setmode(_fileno(stdout), _O_U16TEXT);
#else
	fwide(stdout, 1);
#endif

	FILE *fout = wide_fopen(L"я русский.txt", WFILE_WRITE);
	fwprintf(fout, L"привет\n");
	fclose(fout);

	wprintf(L"привет\n");
	return 0;
}
