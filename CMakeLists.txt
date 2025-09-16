cmake_minimum_required(VERSION 3.10)
project(MyProject LANGUAGES CXX)

# Richiedi C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(ROOT REQUIRED COMPONENTS Hist Graf Gpad RIO)

# Dichiara i tuoi header
set(MY_HEADERS
    Hits.h
    MyGen.h
    PCollision.h
    Scattering.h
    TrackRec.h
)

# Genera dictionary
ROOT_GENERATE_DICTIONARY(G__MyDict ${MY_HEADERS} LINKDEF LinkDef.h)

add_executable(
    fastMC
    main.cpp 
    Hits.cxx
    MyGen.cxx 
    PCollision.cxx 
    Scattering.cxx 
    TrackRec.cxx 
    simulation.cxx 
    reconstruction.cxx
    moltepl.cxx
    ztrue.cxx
    G__MyDict.cxx
    )

target_include_directories(fastMC PRIVATE ${ROOT_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})

target_link_libraries(fastMC ${ROOT_LIBRARIES})

# Mette i binari  dentro build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)