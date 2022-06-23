cmake_minimum_required(VERSION 2.8)


project(anyoption)
set(PROJECT_VERSION "1.3")
set(PROJECT_DESCRIPTION "A C++ class for easy parsing of complex commandline options")
set(PROJECT_HOMEPAGE_URL "https://github.com/hackorama/AnyOption")

include(CPackComponent)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "${PROJECT_NAME} developers")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_COMPONENTS_GROUPING IGNORE)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_NSIS_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

set(IncludeDir "${CMAKE_CURRENT_SOURCE_DIR}")

set(srcs
  ${IncludeDir}/${PROJECT_NAME}.cpp
  )

set(hdrs
  ${IncludeDir}/${PROJECT_NAME}.h
  )

set(PackagingTemplatesDir "${CMAKE_CURRENT_SOURCE_DIR}/packaging")

cpack_add_component(lib
	DISPLAY_NAME "library"
	DESCRIPTION "${PROJECT_NAME} library itself, needed to run the software"
)

set(CPACK_DEBIAN_LIB_PACKAGE_NAME "lib${CPACK_PACKAGE_NAME}")
set(CPACK_RPM_LIB_PACKAGE_NAME "lib${CPACK_PACKAGE_NAME}")

set("CPACK_DEBIAN_SDK_PACKAGE_NAME" "${CPACK_DEBIAN_LIB_PACKAGE_NAME}-dev")
set("CPACK_RPM_SDK_PACKAGE_NAME" "${CPACK_RPM_LIB_PACKAGE_NAME}-devel")

add_library(${PROJECT_NAME} SHARED ${srcs} ${hdrs})

include_directories(${PROJECT_NAME} $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
	LIBRARY
		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		COMPONENT "lib"
	ARCHIVE
		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		COMPONENT "sdk"
	INCLUDES
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
		# COMPONENT "sdk" # component is not allowed for includes! Headers are installed separately! Includes only marks the headers for export
)

cpack_add_component(sdk
	DISPLAY_NAME "Development files"
	DESCRIPTION "Headers and other files needed for using ${PROJECT_NAME} library in own software"
	DEPENDS "lib"
)

install(EXPORT ${PROJECT_NAME}-config
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	COMPONENT sdk
)
install(FILES ${hdrs}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	COMPONENT sdk
)

set(CONFIG_FILE_NAME_WITHOUT_EXT "${PROJECT_NAME}Config")
set(CMAKE_CONFIG_FILE_BASENAME "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME_WITHOUT_EXT}")
set(CMAKE_CONFIG_VERSION_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}-version.cmake")
set(CMAKE_CONFIG_FILE_NAME "${CMAKE_CONFIG_FILE_BASENAME}.cmake")

if(${CMAKE_VERSION} VERSION_GREATER "3.14")
	set(OPTIONAL_ARCH_INDEPENDENT "ARCH_INDEPENDENT")
endif()

set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")
configure_file("${PackagingTemplatesDir}/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
install(FILES "${PKG_CONFIG_FILE_NAME}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
	COMPONENT "sdk"
)

option(WITH_TESTS ON "Build tests")

if(WITH_TESTS)
	find_package(Catch2)
	include(Catch)

	add_executable(tests "${CMAKE_CURRENT_SOURCE_DIR}/test.cpp")
	target_link_libraries(tests PRIVATE ${PROJECT_NAME} Catch2::Catch2)

	catch_discover_tests(tests)
	enable_testing()
endif()


include(CPack)

