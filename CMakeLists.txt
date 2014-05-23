set(CMAKE_BUILD_TYPE_INIT "Release")

project(DepthView)
cmake_minimum_required(VERSION 2.8)

option(DEPTHVIEW_USE_QT4 "use Qt 4 rather than Qt 5." OFF)
option(DEPTHVIEW_PORTABLE "build a portable version of DepthView (i.e. settings are saved in the application directory)" OFF)

set(DepthView_VERSION_MAJOR 1)
set(DepthView_VERSION_MINOR 05)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Silences warnings about the policy not being set.
# We still manually link to Qt5::WinMain to support older versions of CMake.
# As far as I can tell there is no harm in doing both.
if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif(POLICY CMP0020)

include_directories("./include")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

file(GLOB HEADERS "include/*.h")
file(GLOB SOURCES "src/*.cpp")
file(GLOB FORMS "forms/*.ui")
set(RESOURCES "resources.qrc")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp" @ONLY)
list(APPEND SOURCES "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")

if(WIN32)
    if(${CMAKE_GENERATOR} MATCHES "Visual Studio*")
        add_definitions(-DDEPTHVIEW_MSVC_CONFIGURATION="$(ConfigurationName)")
    endif()

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/DepthView.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/DepthView.rc" @ONLY)
    set(WINDRES "${CMAKE_CURRENT_BINARY_DIR}/DepthView.rc")
else(WIN32)
    set(WINDRES "")
endif(WIN32)

if(DEPTHVIEW_PORTABLE)
    add_definitions(-DDEPTHVIEW_PORTABLE)
endif(DEPTHVIEW_PORTABLE)

if(DEPTHVIEW_USE_QT4)
    find_package(Qt4 REQUIRED)
    include(${QT_USE_FILE})

    qt4_add_resources(RESOURCES_RCC ${RESOURCES})
    qt4_wrap_ui(FORM_HEADERS ${FORMS})
else(DEPTHVIEW_USE_QT4)
    find_package(Qt5Widgets)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    add_definitions(${Qt5Widgets_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

    qt5_wrap_ui(FORM_HEADERS ${FORMS})
    qt5_add_resources(RESOURCES_RCC ${RESOURCES})
endif(DEPTHVIEW_USE_QT4)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${FORM_HEADERS} ${HEADERS} ${RESOURCES_RCC} ${WINDRES})

if(DEPTHVIEW_USE_QT4)
    target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
else(DEPTHVIEW_USE_QT4)
    qt5_use_modules(${PROJECT_NAME} Widgets)

    if(WIN32)
        target_link_libraries(${PROJECT_NAME} Qt5::WinMain)
    endif(WIN32)
endif(DEPTHVIEW_USE_QT4)

# TODO - make this whole CPack thing a lot less messy...

install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION bin
        COMPONENT applications)

# TODO - add other places to the search path...
set(DIRS ${CMAKE_BINARY_DIR})
set(_extension)

IF(WIN32)
  set(_extension ".exe")

  # these dlls do not get detected by BundleUtilities.
  # TODO - make these less hard coded
  install(FILES "${CMAKE_BINARY_DIR}/libjpeg-8.dll" DESTINATION bin COMPONENT applications)
  install(FILES "${CMAKE_BINARY_DIR}/libEGL.dll" DESTINATION bin COMPONENT applications)
ENDIF(WIN32)

install(DIRECTORY "${CMAKE_BINARY_DIR}/imageformats" DESTINATION bin COMPONENT applications)
install(DIRECTORY "${CMAKE_BINARY_DIR}/platforms" DESTINATION bin COMPONENT applications)

INSTALL(CODE "
file(GLOB QT_PLUGINS \"${CMAKE_INSTALL_PREFIX}/imageformats/*${CMAKE_SHARED_LIBRARY_SUFFIX}\" \"${CMAKE_INSTALL_PREFIX}/platforms/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
include(\"BundleUtilities\")
fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/bin/DepthView${_extension}\" \"\${QT_PLUGINS}\" \"${DIRS}\")
" COMPONENT applications)

set(CPACK_PACKAGE_NAME "DepthView")
set(CPACK_PACKAGE_VENDOR "chipgw")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A basic viewer for stereoscopic 3D images.")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DepthView")
set(CPACK_PACKAGE_VERSION_MAJOR ${DepthView_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${DepthView_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_NSIS_MENU_LINKS "bin\\\\DepthView" "DepthView")
include(CPack)
