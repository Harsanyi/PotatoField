cmake_minimum_required(VERSION 3.28)
project(PotatoField VERSION 1.1.0 LANGUAGES CXX)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(sfml
    URL https://github.com/SFML/SFML/archive/refs/tags/2.6.2.zip
    URL_HASH SHA256=19d6dbd9c901c74441d9888c13cb1399f614fe8993d59062a72cfbceb00fed04
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL SYSTEM)
FetchContent_Declare(glm
    URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
    URL_HASH SHA256=09c5716296787e1f7fcb87b1cbdbf26814ec1288ed6259ccd30d5d9795809fa5
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(sfml glm)

set(POTATOFIELD_SOURCES
    src/Assets.cpp src/BackgroundMusic.cpp src/Bullet.cpp src/Button.cpp
    src/Enemy.cpp src/Explosion.cpp src/FireSprite.cpp src/Game.cpp
    src/GameOverMenu.cpp src/Graphics.cpp src/HealthBar.cpp src/HealthPill.cpp
    src/Label.cpp src/Log.cpp src/MainMenu.cpp src/Player.cpp
    src/PlayerBullet.cpp src/PlayerScore.cpp src/Potato.cpp src/Random.cpp
    src/Sprite.cpp src/Star.cpp src/Texture.cpp src/WeaponPair.cpp src/WeaponUI.cpp)
add_library(PotatoFieldRuntime STATIC ${POTATOFIELD_SOURCES})
target_include_directories(PotatoFieldRuntime PUBLIC src)
target_compile_features(PotatoFieldRuntime PUBLIC cxx_std_17)
target_compile_definitions(PotatoFieldRuntime PUBLIC GLM_ENABLE_EXPERIMENTAL)
target_link_libraries(PotatoFieldRuntime PUBLIC sfml-graphics sfml-window sfml-system sfml-audio glm::glm)

option(POTATOFIELD_SANITIZE "Instrument project code with AddressSanitizer" OFF)
if(POTATOFIELD_SANITIZE)
    if(MSVC)
        # SFML's prebuilt static dependencies do not carry MSVC STL container annotations.
        target_compile_definitions(PotatoFieldRuntime PUBLIC _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION)
        target_compile_options(PotatoFieldRuntime PUBLIC /fsanitize=address /Zi)
        target_link_options(PotatoFieldRuntime PUBLIC /INCREMENTAL:NO)
    else()
        target_compile_options(PotatoFieldRuntime PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(PotatoFieldRuntime PUBLIC -fsanitize=address,undefined)
    endif()
endif()

function(potatofield_runtime_files target)
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${target}>/assets"
        VERBATIM)
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${sfml_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll"
                "$<TARGET_FILE_DIR:${target}>"
            VERBATIM)
    endif()
endfunction()

add_executable(PotatoField src/main.cpp)
target_link_libraries(PotatoField PRIVATE PotatoFieldRuntime)
potatofield_runtime_files(PotatoField)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT PotatoField)

include(CTest)
if(BUILD_TESTING)
    add_executable(CoreChecks tests/CoreChecks.cpp)
    target_include_directories(CoreChecks PRIVATE src)
    target_compile_features(CoreChecks PRIVATE cxx_std_17)
    target_link_libraries(CoreChecks PRIVATE glm::glm)
    add_test(NAME CoreChecks COMMAND CoreChecks)
    add_executable(RuntimeSmoke tests/RuntimeSmoke.cpp)
    target_link_libraries(RuntimeSmoke PRIVATE PotatoFieldRuntime)
    potatofield_runtime_files(RuntimeSmoke)
    add_test(NAME RuntimeSmoke COMMAND RuntimeSmoke)
    set_tests_properties(RuntimeSmoke PROPERTIES TIMEOUT 45)
endif()

install(TARGETS PotatoField RUNTIME DESTINATION .)
install(DIRECTORY assets DESTINATION .)
install(FILES LICENSE README.md THIRD_PARTY_NOTICES.md DESTINATION .)
install(DIRECTORY third_party DESTINATION .)
install(DIRECTORY docs/images DESTINATION docs)
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(POTATOFIELD_OPENAL_ARCH x64)
    else()
        set(POTATOFIELD_OPENAL_ARCH x86)
    endif()
    install(FILES "${sfml_SOURCE_DIR}/extlibs/bin/${POTATOFIELD_OPENAL_ARCH}/openal32.dll" DESTINATION .)
endif()
set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_FILE_NAME "PotatoField-${PROJECT_VERSION}-Windows-x64")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
include(CPack)
