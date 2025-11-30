cmake_minimum_required(VERSION 3.16)

project(ManageMyBudget VERSION 1.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR C:/Qt/6.9.1/msvc2022_64/lib/cmake/Qt6)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(SRC_DIR src)

set(SOURCES
  ${SRC_DIR}/main.cpp

  ${SRC_DIR}/domain/Accountant.cpp
  ${SRC_DIR}/domain/Amount.cpp
  ${SRC_DIR}/domain/BankAccount.cpp
  ${SRC_DIR}/domain/Category.cpp
  ${SRC_DIR}/domain/Operation.cpp
  ${SRC_DIR}/domain/Profile.cpp

  ${SRC_DIR}/application/Application.cpp

  ${SRC_DIR}/data/StateManager.cpp
  ${SRC_DIR}/data/DataManager.cpp
  ${SRC_DIR}/data/BackUpManager.cpp

  ${SRC_DIR}/types/Converter.cpp
  ${SRC_DIR}/files/Files.cpp

  ${SRC_DIR}/exceptions/ApplicationException.cpp
  ${SRC_DIR}/exceptions/FileException.cpp
  ${SRC_DIR}/exceptions/ForbiddenActionException.cpp
  ${SRC_DIR}/exceptions/InvalidInputException.cpp

  ${SRC_DIR}/ui/MainWindow.cpp

  ${SRC_DIR}/ui/hooks/ConfirmationRequiser.cpp
  ${SRC_DIR}/ui/hooks/ExceptionHandler.cpp

  ${SRC_DIR}/ui/format/AmountValueFormatter.cpp
  ${SRC_DIR}/ui/format/StringFormatter.cpp
  ${SRC_DIR}/ui/format/MonthString.cpp

  ${SRC_DIR}/ui/account/AddAccountDialog.cpp
  ${SRC_DIR}/ui/account/EditAccountDialog.cpp
  ${SRC_DIR}/ui/account/ManageAccountsDialog.cpp

  ${SRC_DIR}/ui/category/EditCategoryDialog.cpp
  ${SRC_DIR}/ui/category/ManageCategoriesDialog.cpp

  ${SRC_DIR}/ui/operation/AddOperationForm.cpp
  ${SRC_DIR}/ui/operation/EditOperationDialog.cpp
  ${SRC_DIR}/ui/operation/OperationsList.cpp

  ${SRC_DIR}/ui/profile/AddProfileDialog.cpp
  ${SRC_DIR}/ui/profile/ManageProfilesDialog.cpp
  ${SRC_DIR}/ui/profile/RenameProfileDialog.cpp

  ${SRC_DIR}/ui/review/MonthlyReviewDialog.cpp
  ${SRC_DIR}/ui/review/YearlyReviewDialog.cpp
)

set(HEADERS
  ${SRC_DIR}/domain/Accountant.h
  ${SRC_DIR}/domain/Amount.h
  ${SRC_DIR}/domain/BankAccount.h
  ${SRC_DIR}/domain/Category.h
  ${SRC_DIR}/domain/Operation.h
  ${SRC_DIR}/domain/Profile.h

  ${SRC_DIR}/application/Application.h

  ${SRC_DIR}/data/StateManager.h
  ${SRC_DIR}/data/DataManager.h
  ${SRC_DIR}/data/BackUpManager.h

  ${SRC_DIR}/types/Converter.h
  ${SRC_DIR}/files/Files.h

  ${SRC_DIR}/exceptions/ApplicationException.h
  ${SRC_DIR}/exceptions/FileException.h
  ${SRC_DIR}/exceptions/ForbiddenActionException.h
  ${SRC_DIR}/exceptions/InvalidInputException.h

  ${SRC_DIR}/ui/MainWindow.h

  ${SRC_DIR}/ui/hooks/ConfirmationRequiser.h
  ${SRC_DIR}/ui/hooks/ExceptionHandler.h

  ${SRC_DIR}/ui/format/AmountValueFormatter.h
  ${SRC_DIR}/ui/format/StringFormatter.h
  ${SRC_DIR}/ui/format/MonthString.h

  ${SRC_DIR}/ui/account/AddAccountDialog.h
  ${SRC_DIR}/ui/account/EditAccountDialog.h
  ${SRC_DIR}/ui/account/ManageAccountsDialog.h

  ${SRC_DIR}/ui/category/EditCategoryDialog.h
  ${SRC_DIR}/ui/category/ManageCategoriesDialog.h

  ${SRC_DIR}/ui/operation/AddOperationForm.h
  ${SRC_DIR}/ui/operation/EditOperationDialog.h
  ${SRC_DIR}/ui/operation/OperationsList.h

  ${SRC_DIR}/ui/profile/AddProfileDialog.h
  ${SRC_DIR}/ui/profile/ManageProfilesDialog.h
  ${SRC_DIR}/ui/profile/RenameProfileDialog.h

  ${SRC_DIR}/ui/review/MonthlyReviewDialog.h
  ${SRC_DIR}/ui/review/YearlyReviewDialog.h
)

qt_add_executable(ManageMyBudget ${SOURCE} ${HEADERS})
target_link_libraries(ManageMyBudget PRIVATE Qt6::Core Qt6::Widgets)

target_include_directories(ManageMyBudget
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

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