// Собирать:
// Linux
// CC=clang CXX=clang ./regen.sh
// или
// CC=gcc CXX=g++ ./regen.sh
// Windows developer powershell
// .\regen.ps1
// Windows Visual Studio
// Конфигурация "x64-release" или "x86-release" (32 бита)
//
// Запускать
// Linux
// ./Build/Release/app
// Windows developer powershell
// .\Build\x64-release\app.exe
// или
// .\Build\x86-release\app.exe
// Windows Visual Studio
// сама запускает

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#if defined(_WIN32) && defined(_MSC_VER)
// эти хедеры есть только на виндовс
// нужны при msvc
#include <io.h>
#include <fcntl.h>
#endif

#ifdef _MSC_VER
#define WNSTR_FMT L"%hs"
// важно писать часть после запятой точь-в-точь
// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170#unicode-support
#define FILE_WRITE "w, ccs=UTF-8"
#else
#define WNSTR_FMT L"%ls"
#define FILE_WRITE "w"
#endif

int main(void) {
	setlocale(LC_ALL, "ru_RU.utf8");
#ifdef _MSC_VER
	// msvc - fwide не реализован
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
#else
	fwide(stdin, 1);
	fwide(stdout, 1);
	fwide(stderr, 1);
#endif
	wprintf(L"1:\n%d %f\n%c %lc\n"WNSTR_FMT" %ls\n", 15, -4.3, 'f', L'ж', "narrow chars",
		L"широкие символы");

#define N 1024
	wchar_t data[N];
	fgetws(data, N, stdin); // в конце будет L'\0'
	for (unsigned int i = 0; i < N && data[i] != L'\n' && data[i] != L'\0'; i++) {
		wprintf(L"[%4d] %lc - %d\n", i, data[i], data[i]);
	}
	wprintf(L"%ls", data);
	FILE *fout;
	fout = fopen("ru.txt", FILE_WRITE);
	// для открытых файлов _setmode не нужен
	// = кодировка ставится в fopen через `, ccs=UTF-8`
	fwide(fout, 1);
	fwprintf(fout, L"123 привет\n%ls\n", data);
	fclose(fout);
}
