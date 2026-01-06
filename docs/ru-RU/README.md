# lib
Несколько однофайловых заголовочных файлов типа [stb](https://github.com/nothings/stb) для C и C++.

Если вы хотите проверять, как использовать утилиты, содержащую только один заголовок, то, пожалуйста, прочтите [гид по stb](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt).

## Оглавление
1. [Использование](#использование)
2. C
    1. [Версия](/docs/ru-RU/c/version.md)
    2. [Логгер](/docs/ru-RU/c/logger.md)
    3. [Флаг](/docs/ru-RU/c/flag.md)
    4. [Строка](/docs/ru-RU/c/string.md)
    5. Скопления
        1. [Список букв](/docs/ru-RU/c/collections/string_builder.md)
3. CPP
    1. [Версия](/docs/ru-RU/cpp/version.md)
    2. [Ошибка](/docs/ru-RU/cpp/exception.md)
    3. [Логгер](/docs/ru-RU/cpp/logger.md)
    4. [Скопления](/docs/ru-RU/cpp/collections/collections.md)
        1. Интерфейсы
            1. [Скопление](/docs/ru-RU/cpp/collections/collection.md)
            2. [Размерный](/docs/ru-RU/cpp/collections/sized.md)
        2. Структуры
            1. [Список](/docs/ru-RU/cpp/collections/array.md)

## Использование
Простой способ использования этой утилиты, Во-первых скопировать каждый заголовочный файл в свой проект. Затем вы можете включить `lib.h` или `lib.hpp` в любое место вашего проекта на C или C++ соответственно.
Чтобы включить конкретный файл утилиты в свой проект, используйте `include` и укажите его определение ```#define <LIBRARY_NAME>_IMPLEMENTATION```.