add_library(SeggerRTT STATIC) 
file(GLOB SEGGERTT_SOURCES "src/*.c")


set(SEGGERRTT_BASE_SRC ${SEGGERTT_SOURCES})
target_sources(SeggerRTT PRIVATE ${SEGGERRTT_BASE_SRC})

target_include_directories(SeggerRTT PUBLIC
        inc
)