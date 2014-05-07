NAME=testdriver
include( ../smake/app.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)
addStaticLibDependency($$SBASE_PATH,sbase)

## Sources

SOURCES += src/testdriver_main.cpp

## Build Options
