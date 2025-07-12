cmake_minimum_required(VERSION 3.16)

project(ManageMyBudget VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR C:/Qt/6.9.1/msvc2022_64/lib/cmake/Qt6)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set (SRC_DIR src)

set (SOURCES_FILES
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/core/Accountant.cpp
  ${SRC_DIR}/core/Amount.cpp
  ${SRC_DIR}/core/BankAccount.cpp
  ${SRC_DIR}/core/Operation.cpp

  ${SRC_DIR}/app/DataManager.cpp

  ${SRC_DIR}/app/Profile.cpp

  ${SRC_DIR}/app/ApplicationException.cpp
  ${SRC_DIR}/app/FileException.cpp
  ${SRC_DIR}/app/ForbiddenActionException.cpp
  ${SRC_DIR}/app/InvalidInputException.cpp

  ${SRC_DIR}/app/AmountValueFormatter.cpp
  ${SRC_DIR}/app/ConfirmationRequiser.cpp
  ${SRC_DIR}/app/ExceptionHandler.cpp
  ${SRC_DIR}/app/StringFormatter.cpp
  ${SRC_DIR}/app/MonthString.cpp

  ${SRC_DIR}/app/MainWindow.cpp

  ${SRC_DIR}/app/AddAccountDialog.cpp
  ${SRC_DIR}/app/AddOperationForm.cpp
  ${SRC_DIR}/app/AddProfileDialog.cpp
  ${SRC_DIR}/app/EditAccountDialog.cpp
  ${SRC_DIR}/app/EditOperationDialog.cpp
  ${SRC_DIR}/app/ManageAccountsDialog.cpp
  ${SRC_DIR}/app/ManageCategoriesDialog.cpp
  ${SRC_DIR}/app/ManageProfilesDialog.cpp
  ${SRC_DIR}/app/MonthlyReviewDialog.cpp
  ${SRC_DIR}/app/OperationsList.cpp
  ${SRC_DIR}/app/RenameCategoryDialog.cpp
  ${SRC_DIR}/app/RenameProfileDialog.cpp
  ${SRC_DIR}/app/YearlyReviewDialog.cpp
)

set (HEADERS_FILES
  ${SRC_DIR}/core/Accountant.h
  ${SRC_DIR}/core/Amount.h
  ${SRC_DIR}/core/BankAccount.h
  ${SRC_DIR}/core/Operation.h

  ${SRC_DIR}/app/DataManager.h

  ${SRC_DIR}/app/Profile.h

  ${SRC_DIR}/app/ApplicationException.h
  ${SRC_DIR}/app/FileException.h
  ${SRC_DIR}/app/ForbiddenActionException.h
  ${SRC_DIR}/app/InvalidInputException.h

  ${SRC_DIR}/app/AmountValueFormatter.h
  ${SRC_DIR}/app/ConfirmationRequiser.h
  ${SRC_DIR}/app/ExceptionHandler.h
  ${SRC_DIR}/app/StringFormatter.h
  ${SRC_DIR}/app/MonthString.h

  ${SRC_DIR}/app/MainWindow.h

  ${SRC_DIR}/app/AddAccountDialog.h
  ${SRC_DIR}/app/AddOperationForm.h
  ${SRC_DIR}/app/AddProfileDialog.h
  ${SRC_DIR}/app/EditAccountDialog.h
  ${SRC_DIR}/app/EditOperationDialog.h
  ${SRC_DIR}/app/ManageAccountsDialog.h
  ${SRC_DIR}/app/ManageCategoriesDialog.h
  ${SRC_DIR}/app/ManageProfilesDialog.h
  ${SRC_DIR}/app/MonthlyReviewDialog.h
  ${SRC_DIR}/app/OperationsList.h
  ${SRC_DIR}/app/RenameCategoryDialog.h
  ${SRC_DIR}/app/RenameProfileDialog.h
  ${SRC_DIR}/app/YearlyReviewDialog.h
)

qt_add_executable(ManageMyBudget ${SOURCES_FILES} ${HEADERS_FILES})
target_link_libraries(ManageMyBudget PRIVATE Qt6::Core Qt6::Widgets)

target_include_directories(ManageMyBudget
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

set(CMAKE_WIN32_EXECUTABLE ON)

install(TARGETS ManageMyBudget
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ManageMyBudget
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})