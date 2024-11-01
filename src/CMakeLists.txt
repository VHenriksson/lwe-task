set(SOURCES
    main.cpp 
)


add_library(SimpleTFHE ${SOURCES})

add_executable(EncryptManyTimes main.cpp)
target_link_libraries(EncryptManyTimes PRIVATE SimpleTFHE)

# This (using /../) is really ugly, but it is good enough for now
target_include_directories(SimpleTFHE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
