cmake_minimum_required(VERSION 3.0)
project(USBOscilloscope)

if(APPLE)  # Код для MacOS
    execute_process(
        COMMAND sw_vers -productVersion
        OUTPUT_VARIABLE OSX_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("-- build for MacOS ${OSX_VERSION}")
    include_directories(/opt/homebrew/Cellar/libusb/1.0.26/include)
    link_directories(/opt/homebrew/Cellar/libusb/1.0.26/lib)
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

SET(CMAKE_BUILD_TYPE "Debug") 
# Add support for GDB
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

# Добавление исполняемого файла с именем main.out
add_executable(
    main.out
    ./project/main.cpp
    ./project/CommandDecoder.cpp
    ./HT6022_lib_cpp/project/ht6022lib.cpp
    ./HT6022_lib_cpp/project/hantek6022.cpp
    ./oscPlug/oscilloscope_plug.cpp
    ./project/utils.cpp
    ./project/Server.cpp
    ./project/TypeCommands.cpp
    ./project/TypeVCommands.cpp
    ./project/TypeSCommands.cpp
    ./project/TypePCommands.cpp
    ./project/TypeICommands.cpp
    )


# Включение заголовочных файлов и библиотек libusb
if(APPLE)
    target_link_libraries(main.out libusb-1.0.dylib)
endif()

if(IS_THIS_RASPBERRY_PI STREQUAL "NAME=\"Raspbian GNU/Linux\"")

endif()
