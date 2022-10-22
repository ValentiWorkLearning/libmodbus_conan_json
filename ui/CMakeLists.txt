add_library(application_ui STATIC application_ui.cpp)

target_include_directories(application_ui PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(application_ui PUBLIC ftxui::ftxui ftxui::component ftxui::dom ftxui::screen)