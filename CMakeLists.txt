project(DepthView)
cmake_minimum_required(VERSION 2.8)

set(DepthView_VERSION_MAJOR 1)
set(DepthView_VERSION_MINOR 04)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories("./include")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/")

include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp" @ONLY)

file(GLOB HEADERS "include/*.h")
file(GLOB SOURCES "src/*.cpp")
file(GLOB FORMS "forms/*.ui")
set(RESOURCES "icons.qrc")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/DepthView.rc.in" "${CMAKE_CURRENT_SOURCE_DIR}/DepthView.rc" @ONLY)
set(WINDRES "DepthView.rc")

find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

qt5_wrap_ui(FORM_HEADERS ${FORMS})
qt5_add_resources(RESOURCES_RCC ${RESOURCES})


add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${FORM_HEADERS} ${HEADERS} ${RESOURCES_RCC} ${WINDRES})

qt5_use_modules(${PROJECT_NAME} Widgets)

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
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Example showing CPack in action")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "DepthView")
set(CPACK_PACKAGE_VERSION_MAJOR ${DepthView_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${DepthView_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_NSIS_MENU_LINKS "bin\\\\DepthView" "DepthView")
include(CPack)