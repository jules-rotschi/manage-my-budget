cmake_minimum_required(VERSION 3.16)

project(ManageMyBudgetTests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (APP_SRC_DIR ../src)
set (TEST_SRC_DIR src)

set (APP_SOURCES
  ${APP_SRC_DIR}/domain/Amount.cpp
)

set (TEST_SOURCES
${TEST_SRC_DIR}/Test.cpp
)

set (APP_HEADERS
  ${APP_SRC_DIR}/domain/Amount.h
)

set (TEST_HEADERS
  ${TEST_SRC_DIR}/AmountTest.h
)

add_executable (ManageMyBudgetTests ${APP_SOURCES} ${TEST_SOURCES} ${APP_HEADERS} ${TEST_HEADERS})

target_include_directories(ManageMyBudgetTests PRIVATE ${APP_SRC_DIR} ${TEST_SRC_DIR})