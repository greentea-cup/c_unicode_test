# Как включить поддержку юникода (русские и не только символы) в C/C++
## Общее
Есть "многобайтовые узкие" (multibyte narrow) и "широкие" строки как альтернатива обычным узким (narrow).
Многобайтовые пока не до конца понял, так что предлагаю сначала широкие.

Вместо обычных "узких" (narrow) строк и символов использовать широкие, а также заменить функции для узких символов для функции для широких символов (для тех файлов, в которые нужно печатать по-русски).
```c
// было
printf("sample text\n");
// стало
wprintf(L"я русский\n");
```
Использовать локаль (для нас, русских - русскую), иначе символы могут превратиться в транслит
C - `<locale.h>`
C++ - `<clocale>` (пространство имён std)
Больше о локалях:
- [cppreference категории локалей en](https://en.cppreference.com/w/c/locale/LC_categories)
- [cppreference категории локалей ru](https://ru.cppreference.com/w/c/locale/LC_categories)
- [cppreference локали en](https://en.cppreference.com/w/c/locale)
- [msvc 170 setlocale en](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170)
- [msvc 170 setlocale utf8-support en](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/setlocale-wsetlocale?view=msvc-170#utf-8-support)
Читать https://stackoverflow.com/a/65167190/20935957
Пример:
```c
// только символы
setlocale(LC_CTYPE, "ru_RU.utf8")
// только десятичная запятая вместо точки (float)
setlocale(LC_NUMERIC, "ru_RU.utf8")
// я русский
setlocale(LC_ALL, "ru_RU.utf8")
```

Для C++ использовать соответствующие широкие версии стримов (тогда в обычные нельзя будет писать)
```cpp
// было
std::cout << "narrow chars\n";
// стало
std::wcout << L"я русский\n";
```

`fwide` для установки файла в широкий режим.
```c
// сверху setlocale
fwide(stdout, 1); // установить stdout в широкий режим
FILE *fout = fopen("name", "w");
fwide(fout, 1); // установить открытый файл в широкий режим
```

## Отдельно для Windows/MSVC
### fwide
В MSVC `fwide` не реализован. Он не выдаёт ошибок, но и не рабоает. Его надо заменять несколькими способами.
Для уже имеющихся стримов (stdin, stdout, stderr) Использовать `_setmode` с режимом `_O_UTF16TEXT`:
```c
#if defined(_WIN32) && defined(_MSC_VER)
// эти хедеры есть только в виндовс
// *технически, msvc и так тоьлко под windows
// но обратноне не верно - например, есть clang и mingw
#include <io.h>
#include <fcntl.h>
#endif
...
int main(void) {
	setlocale(...);
	#ifdef _MSC_VER
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	#else
	fwide(stdin, 1);
	fwide(stdout, 1);
	fwide(stderr, 1);
	#endif
	...
}
```
Для файлое, открытых через `fopen` **нужно** использовать второй параметр:
после режима добавить `, ccs=UTF-8` (точь-в-точь)
```c
#ifdef _MCS_VER
#define FILE_WRITE "w, ccs=UTF-8"
#else
#define FILE_WRITE "w"
#endif
FILE *fout = fopen("name", FILE_WRITE);
fwide(fout, 1); // msvc пофиг, остальные используют
```
Также в msvc (или в Windows) есть версия `fopen`, принимающая широкие строки в оба аргумента (позволяя открывать файлы с русскими символами в названии).
На Linux такого, насколько хватило изучить, можно достичь через ручное преобразование широкой строки в обычную многобайтовую и вызовом, соответственно, обычного `fopen`.
```c
FILE *fout;
#ifdef _MSC_VER
fout = _wfopen(L"я_русский.txt", L"w, ccs=UTF-8");
#else
{
	// https://www.programmerall.com/article/41141245628/
	// https://en.cppreference.com/w/c/string/multibyte/wcsrtombs
	...
	fout = fopen(..., "w");
	...
}
fwide(fout, 1);
#endif
```
### UTF-8
Для MSVC нужен параметр `/utf-8` для правильной обработки символов в строках в  исходном коде.
