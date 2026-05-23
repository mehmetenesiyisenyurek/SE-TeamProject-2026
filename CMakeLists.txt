cmake_minimum_required(VERSION 3.20)

# MinGW statik runtime - proje tanımından ÖNCE olmalı
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

project(CodeSentinel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        infrastructure
        lexer
        parser
        syntax_analyzer
        pointer
        analysis_engine
        reporting
        services
        gui_winapi
)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
        "infrastructure/*.cpp"
        "lexer/*.cpp"
        "parser/*.cpp"
        "syntax_analyzer/*.cpp"
        "pointer/*.cpp"
        "analysis_engine/*.cpp"
        "reporting/*.cpp"
        "services/*.cpp"
)

add_library(CodeSentinelCore STATIC ${CORE_SOURCES})
target_include_directories(CodeSentinelCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Konsol surumu: Her sistemde calisir.
add_executable(CodeSentinelCLI main.cpp app.rc)
target_link_libraries(CodeSentinelCLI PRIVATE CodeSentinelCore)

# Qt'siz / Designer'siz Windows masaustu arayuzu.
if (WIN32)
    add_executable(CodeSentinelDesktop WIN32
            gui_winapi/main_winapi.cpp
            app.rc
    )
    target_link_libraries(CodeSentinelDesktop PRIVATE CodeSentinelCore comdlg32)
else()
    message(WARNING "CodeSentinelDesktop WinAPI kullandigi icin sadece Windows'ta olusur. Konsol icin CodeSentinelCLI calisir.")
endif()