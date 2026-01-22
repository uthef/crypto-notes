cmake_minimum_required(VERSION 3.15)

project(crypto-notes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLCipher REQUIRED sqlcipher)

include_directories(
    include
    ${SQLCipher_INCLUDE_DIRS}
)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    ${app_icon_resource_windows}
    res.qrc
    src/main.cpp
    include/appcontext.h
    src/appcontext.cpp
    include/storage/database.h
    src/storage/database.cpp
    include/storage/note.h
    src/storage/note.cpp
    include/storage/queries.h
    include/storage/notelist.h
    src/storage/notelist.cpp
    include/models/notelistmodel.h
    src/models/notelistmodel.cpp
    include/utils/formatter.h
    src/utils/formatter.cpp
    include/storage/appconfig.h
    src/storage/appconfig.cpp
    src/utils/passwordgenerator.cpp
    include/filters/mouseeventfilter.h
    src/filters/mouseeventfilter.cpp
    include/tasks/backgroundbackupthread.h
    src/tasks/backgroundbackupthread.cpp
    include/models/backuppathlistmodel.h
    src/models/backuppathlistmodel.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSION="${PROJECT_VERSION}")

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_qml_module(${PROJECT_NAME}
    URI cryptonotes
    VERSION 1.0
    QML_FILES
        ui/Auth.qml
        ui/Editor.qml
        ui/ErrorMessage.qml
        ui/List.qml
        ui/ListRow.qml
        ui/Settings.qml
        ui/Window.qml
        ui/About.qml
        ui/BackupPathListRow.qml
    RESOURCES
        res.qrc
        ui/icons/restore.svg
        ui/icons/note.png
        ui/icons/bin.svg
        ui/icons/check.svg
        ui/icons/edit.svg
        ui/icons/green_check.svg
        ui/icons/lock.svg
        ui/icons/error.svg
        ui/icons/floppy.svg
        ui/icons/plus.svg
        ui/icons/search_inactive.svg
        ui/icons/search.svg
        ui/icons/folder.svg
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    OpenSSL::SSL
    ${SQLCipher_LINK_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.uthef.cryptonotes
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

# === Dependencies ===
# libb2-1.dll
# libbrotlicommon.dll
# libbrotlidec.dll
# libbz2-1.dll
# libcrypto-3-x64.dll
# libdouble-conversion.dll
# libfreetype-6.dll
# libgcc_s_seh-1.dll
# libglib-2.0-0.dll
# libgraphite2.dll
# libharfbuzz-0.dll
# libiconv-2.dll
# libicudt78.dll
# libicuin78.dll
# libicuuc78.dll
# libintl-8.dll
# libmd4c.dll
# libpcre2-16-0.dll
# libpcre2-8-0.dll
# libpng16-16.dll
# libsqlcipher-0.dll
# libstdc++-6.dll
# libwinpthread-1.dll
# libzstd.dll
# zlib1.dll