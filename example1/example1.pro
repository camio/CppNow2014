NAME=example1
include( ../smake/app.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)
addStaticLibDependency($$SANI_PATH,sani)
addStaticLibDependency($$SBASE_PATH,sbase)

## Sources

SOURCES += src/example1_main.cpp

## Build Options
