cmake_minimum_required(VERSION 3.22)
project (modbus_test C CXX ASM)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CROSS_BUILD_FOR_ARM ON)
if(CROSS_BUILD_FOR_ARM)
    set(CMAKE_CROSSCOMPILING TRUE)
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=3bef79da16c2e031dc429e1dac87a08b9226418b300ce004cc125a82687baeef
                TLS_VERIFY ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(${CMAKE_BINARY_DIR}/conan.cmake)


set(CONAN_REQUIRED_LIBRARIES_LIST
    spdlog/1.9.2
    libmodbus/3.1.6
    boost/1.79.0
    nlohmann_json/3.10.5
)

set(CONAN_PACKAGES_OPTIONS
    drogon:with_orm=False
)


conan_cmake_configure(REQUIRES
                      ${CONAN_REQUIRED_LIBRARIES_LIST}
                      GENERATORS cmake_find_package
                      IMPORTS "bin, *.dll -> ${CMAKE_CURRENT_BINARY_DIR}"
                      IMPORTS "lib, *.dylib* -> ${CMAKE_CURRENT_BINARY_DIR}"
                      OPTIONS ${CONAN_PACKAGES_OPTIONS}
                      )

conan_cmake_autodetect(settings)

#issue with pkg-conf
#https://github.com/conan-io/conan-center-index/issues/3026
#https://github.com/conan-io/conan-center-index/issues/9005#issuecomment-1018441082
#https://github.com/conan-io/conan/issues/8081

if(CROSS_BUILD_FOR_ARM)
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    PROFILE_BUILD default PROFILE_HOST ${CMAKE_CURRENT_LIST_DIR}/conan_profiles/orange_zero_profile
                    REMOTE conancenter
                    SETTINGS ${settings})
else()
conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    PROFILE default
                    SETTINGS ${settings})
endif()


find_package(libmodbus REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(modbus_test main.cpp modbus_proxy.cpp modbus_scenario_handler.cpp)
target_include_directories(modbus_test PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(modbus_test  PRIVATE libmodbus::libmodbus Boost::boost Boost::program_options spdlog::spdlog nlohmann_json::nlohmann_json)

