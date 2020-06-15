# taskForProgrammer

Необходимо написать на чистом С++ класс, умеющий максимально быстро читать текстовые лог-файлы огромных размеров (сотни мегабайт, десятки гигабайт) и выдавать строки, удовлетворяющие условиям простейшего regexp: (как минимум операторы * и ?, более широкий набор возможностей приветствуется):

- cимвол '*' - последовательность любых символов неограниченной длины;
- cимвол "?" - один любой символ;
- должны корректно отрабатываться маски: *Some*, *Some, Some*, *****Some*** - нет никаких ограничений на положение * в маске.
- Результатом поиска должны быть строки, удовлетворяющие маске.

Пример:

Маска *abc*  отбирает все строки содержащие abc и начинающиеся и заканчивающиеся любой последовательностью символов.
Маска abc*  отбирает все строки начинающиеся с abc и заканчивающиеся любой последовательностью символов.
Маска abc?  отбирает все строки начинающиеся с abc и заканчивающиеся любым дополнительным символом.
Маска abc   отбирает все строки которые равны этой маске.

Требование:
- Использование ANSI;
- Функциональность: максимальная производительность (кэширование результатов поиска и файла не требуется);
- Ресурсы: затраты памяти должны быть минимальны (в разумных пределах);
- Компоненты: запрещено использование сторонних библиотек (в том числе STL) и компонентов. Разрешено использовать только WinAPI и CRT;
- Исключения: нельзя использовать Windows и С++ исключения;
- Код должен быть абсолютно «неубиваемый» и защищённым от ошибок.
