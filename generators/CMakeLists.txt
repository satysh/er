# Create a library called "libgenerators" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${GEANT4_INCLUDE_DIR}
${ROOT_INCLUDE_DIR}
${Boost_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/generators
${CMAKE_SOURCE_DIR}/data
${CMAKE_SOURCE_DIR}/base
${CMAKE_SOURCE_DIR}/passive
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${BASE_LINK_DIRECTORIES}
${FAIRROOT_LIBRARY_DIR}
${GEANT4_LIB_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	ERGenerator.cxx
	ERGammaGenerator.cxx
	ERIonGenerator.cxx
	ERIonMixGenerator.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF ERGeneratorsLinkDef.h)
Set(LIBRARY_NAME generators)
Set(DEPENDENCIES ERBase ERData Base Core G4particles)

GENERATE_LIBRARY()

