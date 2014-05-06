NAME=examples
include( ../smake/app.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)
addStaticLibDependency($$SANI_PATH,sani)
addStaticLibDependency($$SBASE_PATH,sbase)

## Sources

SOURCES += src/examples_main.cpp

## Build Options
