add_library(core STATIC modbus_proxy.cpp modbus_scenario_handler.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(core PUBLIC libmodbus::libmodbus Boost::boost Boost::program_options spdlog::spdlog nlohmann_json::nlohmann_json)