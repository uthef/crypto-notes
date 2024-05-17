cmake_minimum_required(VERSION 3.15)

project(crypto-notes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
    set(SQLITE_PATH ${CMAKE_SOURCE_DIR}/libs/win/sqlite3-sqlcipher-icu.o)
    set(CMAKE_EXE_LINKER_FLAGS -fstack-protector)
    set(ICU_PATHS
        ${CMAKE_SOURCE_DIR}/libs/win/libicuin74.dll
        ${CMAKE_SOURCE_DIR}/libs/win/libicuuc74.dll
    )
    add_definitions(-D_WIN=1)
else()
    set(SQLITE_PATH ${CMAKE_SOURCE_DIR}/libs/linux/libsqlcipher.so.0.8.6)
    # set(ICU_PATHS
    #     /usr/lib/libicui18n.so
    #     /usr/lib/libicuuc.so
    # )
endif()

find_package(Qt6 REQUIRED COMPONENTS Quick)

include_directories(
    headers
    libs/headers
)

qt_standard_project_setup()

qt_add_executable(crypto-notes
    res.qrc
    src/main.cpp
    headers/appcontext.h
    src/appcontext.cpp
    headers/database.h
    src/database.cpp
    headers/note.h
    src/note.cpp
    headers/queries.h
    headers/notelist.h
    src/notelist.cpp
    headers/notelistmodel.h
    src/notelistmodel.cpp
    headers/formatter.h
    src/formatter.cpp
    headers/appconfig.h
    src/appconfig.cpp
)

qt_policy(SET QTP0001 OLD)

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
)

if(WIN32)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(crypto-notes PRIVATE
        OpenSSL::SSL
        ${ICU_PATHS}
    )
endif()

target_link_libraries(crypto-notes PRIVATE
    Qt6::Quick
    ${SQLITE_PATH}
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
