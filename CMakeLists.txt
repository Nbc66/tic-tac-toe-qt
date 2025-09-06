# CMakeList.txt : CMake project for tic-tac-toe-qt
cmake_minimum_required(VERSION 3.8)

project("tic-tac-toe-qt" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt – works with both Qt6 and Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,\
         $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,\
         $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Add sources from src/
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/GameController.cpp src/GameController.h
    src/Board.cpp src/Board.h
    src/Agents.cpp src/Agents.h
    src/BoardModel.cpp src/BoardModel.h
)

add_executable(tic-tac-toe-qt ${SOURCES})

# use windeployqt to copy over the dlls
if(WIN32)
    add_custom_command(TARGET tic-tac-toe-qt POST_BUILD
        COMMAND Qt${QT_VERSION_MAJOR}::windeployqt
                --no-translations
                --no-system-d3d-compiler
                $<$<CONFIG:Debug>:--debug>
                $<$<CONFIG:Release>:--release>
                "$<TARGET_FILE:tic-tac-toe-qt>"
        COMMENT "Running windeployqt..."
    )
endif()

# Enable Qt's MOC for Q_OBJECT
set_target_properties(tic-tac-toe-qt PROPERTIES AUTOMOC ON)

# Link against Qt Widgets
target_link_libraries(tic-tac-toe-qt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Keep MSVC warnings / hot reload stuff
if (MSVC)
    target_compile_options(tic-tac-toe-qt PRIVATE /W4 /permissive-)
else()
    target_compile_options(tic-tac-toe-qt PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: bump C++ if CMake > 3.12
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tic-tac-toe-qt PROPERTY CXX_STANDARD 20)
endif()

# Enable CTest framework
enable_testing()

function(add_qt_test name)
    add_executable(${name} ${ARGN})
    target_include_directories(${name} PRIVATE src)
    target_link_libraries(${name} PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
    set_target_properties(${name} PROPERTIES AUTOMOC ON)
    add_test(NAME ${name} COMMAND ${name})

    if(WIN32)
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND Qt${QT_VERSION_MAJOR}::windeployqt
                    --no-translations
                    --no-system-d3d-compiler
                    $<$<CONFIG:Debug>:--debug>
                    $<$<CONFIG:Release>:--release>
                    "$<TARGET_FILE:${name}>"
            COMMENT "Running windeployqt for ${name}..."
        )
    endif()
endfunction()

add_qt_test(BoardTests tests/TestBoard.cpp src/Board.cpp)
add_qt_test(AgentTests tests/TestAgents.cpp src/Board.cpp src/Agents.cpp)
add_qt_test(GameControllerTests
    tests/TestGameController.cpp
    src/Board.cpp src/Agents.cpp src/BoardModel.cpp src/GameController.cpp
)
