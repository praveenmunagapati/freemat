######################################################################
# PortAudio Package
######################################################################

MESSAGE("Searching for system PortAudio package")

FIND_LIBRARY(PORTAUDIO_LIBRARY NAMES portaudio DOC "Location of the PORTAUDIO library" PATHS ${LOCAL_PATH})
FIND_PATH(PORTAUDIO_INCLUDE_DIR NAMES portaudio.h DOC "Location of portaudio.h" PATHS ${LOCAL_PATH})
if (PORTAUDIO_LIBRARY AND PORTAUDIO_INCLUDE_DIR)
  SET(PORTAUDIO_FOUND TRUE)
  SET(PORTAUDIO_INCLUDE ${PORTAUDIO_INCLUDE_DIR})
  SET(PORTAUDIO_LIBS ${PORTAUDIO_LIBRARY})
  
  IF(MINGW)
    SET(PORTAUDIO_LIBS ${PORTAUDIO_LIBS} -lwinmm)
  ENDIF(MINGW)
  IF (APPLE)
    FIND_LIBRARY(COREAUDIO_LIBRARY CoreAudio)
    FIND_LIBRARY(AUDIOTOOLBOX_LIBRARY AudioToolbox)
    FIND_LIBRARY(AUDIOUNIT_LIBRARY AudioUnit)
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    SET(PORTAUDIO_LIBS ${PORTAUDIO_LIBS} ${COREAUDIO_LIBRARY} ${AUDIOTOOLBOX_LIBRARY} ${AUDIOUNIT_LIBRARY} ${CARBON_LIBRARY})
  ENDIF (APPLE)
  MESSAGE("Found PortAudio here ${PORTAUDIO_LIBS}")
else()
  SET(PORTAUDIO_FOUND FALSE)
  MESSAGE("Unable to find PortAudio")
endif()

