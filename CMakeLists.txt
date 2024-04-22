cmake_minimum_required(VERSION 3.5)

project(SeamCarving LANGUAGES CXX)

find_package(Imagine REQUIRED COMPONENTS Images)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SeamCarving main.cpp
    Images_av.h Images_av.cpp
    Energie.h Energie.cpp
    Seam.h Seam.cpp
    utils.h utils.cpp)

target_link_libraries(SeamCarving Imagine::Images)

include(GNUInstallDirs)
install(TARGETS SeamCarving
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
