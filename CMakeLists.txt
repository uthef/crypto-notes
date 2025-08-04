cmake_minimum_required(VERSION 3.15)

project(crypto-notes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLCipher REQUIRED sqlcipher)


include_directories(
    include
    ${SQLCipher_INCLUDE_DIRS}
)

qt_standard_project_setup()

qt_add_executable(crypto-notes
    ${app_icon_resource_windows}
    res.qrc
    src/main.cpp
    include/appcontext.h
    src/appcontext.cpp
    include/database.h
    src/database.cpp
    include/note.h
    src/note.cpp
    include/queries.h
    include/notelist.h
    src/notelist.cpp
    include/notelistmodel.h
    src/notelistmodel.cpp
    include/formatter.h
    src/formatter.cpp
    include/appconfig.h
    src/appconfig.cpp
)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_qml_module(crypto-notes
    URI cryptonotes
    VERSION 1.0
    QML_FILES
        ui/auth.qml
        ui/editor.qml
        ui/ErrorMessage.qml
        ui/list.qml
        ui/ListRow.qml
        ui/settings.qml
        ui/window.qml
        RESOURCES src/icon.rc
)

target_link_libraries(crypto-notes PRIVATE
    Qt6::Quick
    OpenSSL::SSL
    ${SQLCipher_LINK_LIBRARIES}
)

set_target_properties(crypto-notes PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appda
    # MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    # MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS crypto-notes
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
