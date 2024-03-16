option(USE_SYSTEM_CATCH2 YES)

if (USE_SYSTEM_CATCH2)
    find_package(Catch2 3.0.1 CONFIG)
endif ()

if (NOT USE_SYSTEM_CATCH2 OR NOT Catch2_FOUND)
    if (NOT USE_SYSTEM_CATCH2)
        message(STATUS "Building Catch2 Locally")
    else ()
        message(STATUS "Could NOT Find Catch2 (Building Locally)")
    endif ()
    include(FetchContent)
    if (DEFINED CATCH2_SOURCE)
        message(STATUS "Building Catch2 with user specified source")
        FetchContent_Declare(
                catch2
                URL ${CATCH2_SOURCE})
    else ()
        message(STATUS "Building Catch2 with source from internet")
        FetchContent_Declare(
                catch2
                GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                GIT_TAG v3.3.2)
    endif ()
    if (NOT catch2_POPULATED)
        FetchContent_Populate(catch2)
        set(CATCH_BUILD_TESTING OFF CACHE BOOL "Build SelfTest project" FORCE)
        set(CATCH_INSTALL_DOCS OFF CACHE BOOL "Install documentation alongside library" FORCE)
        set(CATCH_INSTALL_HELPERS OFF CACHE BOOL "Install contrib alongside library" FORCE)
        add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
    endif ()
else ()
    message(STATUS "Found Catch2 VERSION ${Catch2_VERSION} DIR ${Catch2_DIR}")
endif ()

if (CMAKE_CXX_STANDARD LESS 23)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-Werror=invalid-constexpr" COMPILER_SUPPORTS_INVALID_CONSTEXPR_CHECK)
    if (COMPILER_SUPPORTS_INVALID_CONSTEXPR_CHECK)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=invalid-constexpr")
    endif ()
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-generate -Wcoverage-mismatch")

add_executable(BasicTests BasicTests.cpp)
target_link_libraries(BasicTests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)

add_test(NAME BASIC_TESTS COMMAND BasicTests)
