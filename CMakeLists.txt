project(tile-based-high-resolution-3d-engine) # or, THR3E

add_library(THR3E INTERFACE)

target_sources(THR3E INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/model.cpp
    ${CMAKE_CURRENT_LIST_DIR}/render-3d.cpp
)

target_include_directories(THR3E INTERFACE ${CMAKE_CURRENT_LIST_DIR})

if(32BLIT_PICO)
  target_link_libraries(THR3E INTERFACE hardware_interp pico_multicore pico_sync)
endif()