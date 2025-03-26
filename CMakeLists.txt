cmake_minimum_required(VERSION 3.10)
project(HTMLQuery VERSION 0.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    lexbor
    GIT_REPOSITORY https://github.com/lexbor/lexbor.git
    GIT_TAG        master  # or lock to a commit/hash
)
FetchContent_MakeAvailable(lexbor)



# Force C++20 manually as a fallback if needed
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(hq src/main.cpp)
install(TARGETS hq
    RUNTIME DESTINATION bin
)
install(FILES docs/hq.1 DESTINATION share/man/man1)
target_link_libraries(hq PRIVATE lexbor_static)


# configure_file(src/Config.h.in src/Config.h)

target_include_directories(hq PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/lib"
                           )

