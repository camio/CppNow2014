NAME=fp
include( ../smake/app.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)

## Sources

SOURCES += src/fp_main.cpp

## Build Options
