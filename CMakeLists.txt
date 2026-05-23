cmake_minimum_required(VERSION 3.20)
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
add_executable(CodeSentinelCLI main.cpp)
target_link_libraries(CodeSentinelCLI PRIVATE CodeSentinelCore)

# Qt'siz / Designer'siz Windows masaustu arayuzu.
# CLion + MinGW ortaminda direkt target olarak secilip calistirilir.
if (WIN32)
    add_executable(CodeSentinelDesktop WIN32
        gui_winapi/main_winapi.cpp
    )
    target_link_libraries(CodeSentinelDesktop PRIVATE CodeSentinelCore comdlg32)
else()
    message(WARNING "CodeSentinelDesktop WinAPI kullandigi icin sadece Windows'ta olusur. Konsol icin CodeSentinelCLI calisir.")
endif()
