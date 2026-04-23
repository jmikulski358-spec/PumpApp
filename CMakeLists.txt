cmake_minimum_required(VERSION 3.19)
project(PumpApp LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# Backend
set(CORE_SOURCES
    core/Exercise.cpp
    core/WeightedExercise.cpp
    core/BodyweightExercise.cpp
    core/WorkoutPlan.cpp
    core/ExerciseRepository.cpp
    core/WorkoutPlanRepository.cpp
    core/DatabaseManager.cpp
)

set(CORE_HEADERS
    core/Exercise.h
    core/WeightedExercise.h
    core/BodyweightExercise.h
    core/ExerciseFactory.h
    core/WorkoutPlan.h
    core/ExerciseRepository.h
    core/WorkoutPlanRepository.h
    core/DatabaseManager.h
)

# GUI Files
set(GUI_SOURCES
    gui/MainWindow.cpp
    gui/ExerciseDialog.cpp
    gui/WorkoutPlanDialog.cpp
)

set(GUI_HEADERS
    gui/MainWindow.h
    gui/ExerciseDialog.h
    gui/WorkoutPlanDialog.h
)

set(GUI_FORMS
    gui/MainWindow.ui
    gui/ExerciseDialog.ui
    gui/WorkoutPlanDialog.ui
)

option(BUILD_TESTS "Build tests" OFF)



qt_add_executable(PumpApp
    WIN32 MACOSX_BUNDLE
    main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${GUI_FORMS}
    gui/MainWindow.ui
    gui/ExerciseDialog.ui
    gui/WorkoutPlanDialog.ui
    .gitignore
)

target_include_directories(PumpApp PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/PumpApp_autogen/include
)


target_link_libraries(PumpApp
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)



install(TARGETS PumpApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET PumpApp
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Pobranie Google Test z GitHub
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# Windows: zapobiega nadpisywaniu flag kompilatora
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Włączenie testowania
enable_testing()

# Executable dla testów
add_executable(PumpApp_tests
    tests/test_main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

# Linkowanie z GTest i Qt Core
target_link_libraries(PumpApp_tests
    PRIVATE
    gtest_main
    Qt::Core
)

# Dodanie ścieżek include (takich samych jak w głównej aplikacji)
target_include_directories(PumpApp_tests PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/PumpApp_autogen/include
)

# Automatyczne odkrywanie testów
include(GoogleTest)
gtest_discover_tests(PumpApp_tests)
