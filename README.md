# ServerForOscilloscope

Сервер для удаленного доступа к осциллографу, возможно подключать различные осциллографы, которые
соответветствуют родительскому классу `Oscilloscope` из `oscilloscopes.h`. Необходимо задать свои **IP**
адрес в сети и **PORT**, а так же указать *префикс*:

```cpp
#include "../Ваш_осциллограф.h"
#include "Server.h"
#include "utils.h"


int main( int argc, char* argv[] )
{
    oscilloscopes::Ваш_осциллограф oscilloscope;
    auto arg = readArg( argc, argv );
    server::CommandDecoder cmdec( std::get<0>(arg), &oscilloscope );
    server::Server server( std::get<1>(arg), std::get<2>(arg), &cmdec );
    return server.run();
}
```

В качестве примера в проекте подключается осциллограф Hantek 6022BE ~eму посвещен отдельный [репозиторий](https://github.com/khosta77/HT6022_lib_cpp.git)~.

## Архитектура

### Архитектура декодера

![](/img/d1.png)

В распакованном виде

![](/img/d1_big.png)

## Про тесты

Написанием тестов к каждому методу/функции в проекте я не занимался. Проводил испытания вызова методов
и проверки на отказо устойчивость.

* Протестировано с **Hantek 6022BE** на 10.000 итерациях, на *Mac OS* и *Raspberry Pi* проблем обнаружено не было.

* Что касается кода, после запуска в нем максимальная защита через блок `try {} catch( ... ) {};` такие ошибки отлавливаются.

## Коды ошибки

| Код ошибки | Название макроса ошибки                  | Расшифровка         |
|------------|------------------------------------------|---------------------|
|          1 | OSCILLOSCOPE\_ERROR\_INVALID\_PARAM      |Некорректный параметр на входе метода осциллографа|
|          0 | OSCILLOSCOPE\_SUCCESS                    |Все отработало штатно|
|         -3 | OSCILLOSCOPE\_ERROR\_ACCESS              |Ошибка доступа, может быть вызвана из за того, что вызов не из под`sudo`|
|         -4 | OSCILLOSCOPE\_ERROR\_NO\_DEVICE          |Нет подключенного устройства|
|         -7 | OSCILLOSCOPE\_ERROR\_TIMEOUT             |Ошибка времени ожидания |
|         -11| OSCILLOSCOPE\_ERROR\_NO\_MEM             |Не выделена память |
|         -21| ERROR\_INFO\_EXTRA                       |При запросе информации произошло что-то экстро ординарное |
|         -22| ERROR\_PULSE\_EXTRA                      | |
|         -23| ERROR\_SAMPLE\_EXTRA                     | |
|         -24| ERROR\_VX\_EXTRA                         | |
|         -25| ERROR\_TRIG\_EXTRA                       | |
|         -31| ERROR\_RANGE\_UNKHOWN\_PARAMETR          | |
|         -32| ERROR\_CURRENT\_UNKNOWN\_PARAMETR        | |
|         -33| ERROR\_SET\_UNKNOWN\_NEW\_LEVEL          | |
|         -34| ERROR\_SET\_UNKNOWN\_PARAMETR            | |
|         -35| ERROR\_GET\_DELAY\_UNKNOWN               | |
|         -41| ERROR\_RANGE\_PROBLEM\_GET               | |
|         -42| ERROR\_CURRENT\_PROBLEM\_GET             | |
|         -43| ERROR\_SET\_PROBLEM\_SET                 | |
|         -44| ERROR\_GET\_PROBLEM\_GET                 | |
|         -51| ERROR\_CURRENT\_CHANNEL\_NUMBER\_UNKNOWN | |
|         -52| ERROR\_SET\_CHANNEL\_NUMBER\_UNKNOWN     | |
|         -53| ERROR\_GET\_CHANNEL\_NUMBER\_UNKNOWN     | |
|         -61| ERROR\_RANGE\_EMPTY                      | |
|         -62| ERROR\_GET\_MESSAGE\_THROW               | |
|         -94| ERROR\_NO\_PREFIX                        | |
|         -95| ERROR\_NO\_OSC                           | |
|         -96| ERROR\_NO\_METHOD                        | |
|         -97| ERROR\_UNKNOWN\_MESSAGE                  | |
|         -98| ERROR\_UNKNOWN\_COMMAND                  | |
|         -99| OSCILLOSCOPE\_ERROR\_OTHER               | |
|------------|------------------------------------------|-|
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
./main
```
