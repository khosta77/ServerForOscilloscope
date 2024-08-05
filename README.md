# ServerForOscilloscope

Сервер для удаленного доступа к осциллографу, возможно подключать различные осциллографы, которые
соответветствуют родительскому классу `Oscilloscope` из `oscilloscopes.h`. Необходимо задать свои **IP**
адрес в сети и **PORT**, а так же указать *префикс*:

```cpp
int main()
{
    oscilloscopes::Ваш_осциллограф oscilloscope;
    server::CommandDecoder cmdec( "prefix", &oscilloscope );
    server::Server server( SERVER_ADRESS, SERVER_PORT, &cmdec );
    return server.run();
}
```

В качестве примера в проекте подключается осциллограф Hantek 6022BE ~eму посвещен отдельный [репозиторий](https://github.com/khosta77/HT6022_lib_cpp.git)~.

## Про тесты

Написанием тестов к каждому методу/функции в проекте я не занимался. Проводил испытания вызова методов
и проверки на отказо устойчивость.

* Если не использовать заглушку, ошибок в данной ветке быть не должно. Если установить заглушку может
быть ошибка с malloc ее природу я не установил.

* Если использовать **HT6022** на 10.000 итерациях проблем обнаружено не было.

* Что касается кода, после запуска в нем максимальная защита через блок `try {} catch( ... ) {};` такие ошибки отлавливаются.

## Сборка

1. Скопировать репозиторий сразу с подкачкой `submoduls`

```cmd
git clone --recurse-submodules git@github.com:khosta77/ServerForOscilloscope.git
```

или, если не планируется *пушить* код:

```cmd
git clone --recurse-submodules https://github.com/khosta77/ServerForOscilloscope.git
```

2. Переходим в директорию

```cmd
cd ServerForOscilloscope
```

3. На всякий случай убеждаемся, что все модули в ветке `main`.

```cmd
git submodule foreach git pull origin main
```

4. Собираем и компилируем проект.

```cmd
cmake .
```

```cmd
make
```

5. Запуск. Для примера

```
./main osc 192.168.1.91 8000
```
