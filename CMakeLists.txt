cmake_minimum_required(VERSION 3.0)
project(USBOscilloscope)

if(APPLE)  # Код для MacOS
    execute_process(
        COMMAND sw_vers -productVersion
        OUTPUT_VARIABLE OSX_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("-- build for MacOS ${OSX_VERSION}")
endif()

execute_process(  # Проверим есть ли у нас операционка Raspbian
        COMMAND cat /etc/os-release
        COMMAND grep "NAME=\"Raspbian GNU/Linux\""
        OUTPUT_VARIABLE IS_THIS_RASPBERRY_PI
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

if(IS_THIS_RASPBERRY_PI STREQUAL "NAME=\"Raspbian GNU/Linux\"")
    execute_process(  # Выведем уведомление о том какая у нас версия Raspberry
        COMMAND cat /proc/cpuinfo
        COMMAND grep "Model"
        COMMAND cut -d: -f2-
        OUTPUT_VARIABLE RASPBERRY_PI_INFO
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("-- build for${RASPBERRY_PI_INFO}")

endif()


# Выставить версию с++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Добавление исполняемого файла с именем main.out
add_executable(
    main.out
    ./project/main.cpp
    ./project/CommandDecoder.cpp
    ./oscPlug/oscilloscope_plug.cpp
    ./project/Server.cpp
    ./project/TypeCommands.cpp
    ./project/TypeVCommands.cpp
    ./project/TypeSCommands.cpp
    ./project/TypePCommands.cpp
    )


# Включение заголовочных файлов и библиотек libusb
if(APPLE)

endif()

if(IS_THIS_RASPBERRY_PI STREQUAL "NAME=\"Raspbian GNU/Linux\"")

endif()
