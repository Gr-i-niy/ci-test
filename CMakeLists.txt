cmake_minimum_required(VERSION 3.15)
project(finance_manager)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Найти зависимости через vcpkg
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Пути к заголовочным файлам
include_directories(include)

# Основные исходные файлы
set(
        FINANCE_MANAGER_SOURCES
        src/main.cpp
        src/transaction.cpp
        src/finance_manager.cpp
)

# Тестовые исходные файлы
set(
        FINANCE_MANAGER_TEST_SOURCES
        tests/finance_manager_test.cpp
        src/transaction.cpp
        src/finance_manager.cpp
)

# Основной исполняемый файл
add_executable(finance_manager ${FINANCE_MANAGER_SOURCES})
target_link_libraries(finance_manager PRIVATE
        unofficial::sqlite3::sqlite3
        fmt::fmt-header-only
)
set_property(TARGET finance_manager PROPERTY CXX_STANDARD 23)

# Тестовый исполняемый файл
add_executable(finance_tests ${FINANCE_MANAGER_TEST_SOURCES})
target_link_libraries(finance_tests PRIVATE
        unofficial::sqlite3::sqlite3
        fmt::fmt-header-only
        GTest::gtest
        GTest::gtest_main
)
set_property(TARGET finance_tests PROPERTY CXX_STANDARD 23)

# Настройка тестирования
enable_testing()
add_test(NAME FinanceManagerTests COMMAND finance_tests)

# Цели для проверки и форматирования кода
add_custom_target(
        check-format
        COMMAND clang-format --dry-run ${FINANCE_MANAGER_SOURCES} ${FINANCE_MANAGER_TEST_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "checking format in sources..."
)

add_custom_target(
        format
        COMMAND clang-format -i ${FINANCE_MANAGER_SOURCES} ${FINANCE_MANAGER_TEST_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "formatting sources..."
)
