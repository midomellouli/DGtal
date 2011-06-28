SET(DGTAL_TESTS_SRC_IO_READERS
       testPNMReader
       testVolReader
       testRawReader     
       testPointListReader  )


FOREACH(FILE ${DGTAL_TESTS_SRC_IO_READERS})
  add_executable(${FILE} ${FILE})
  target_link_libraries (${FILE} ${LIBDGTAL_NAME} ${LIBBOARD_NAME})
  add_test(${FILE} ${FILE})
ENDFOREACH(FILE)


IF(MAGICK++_FOUND)

  SET(DGTAL_TESTS_SRC_IO_READERS_Magick
    testMagickReader
    )

  FOREACH(FILE ${DGTAL_TESTS_SRC_IO_READERS_Magick})
    add_executable(${FILE} ${FILE})
    target_link_libraries (${FILE} ${LIBDGTAL_NAME} ${DGtalLibDependencies})
    add_test(${FILE} ${FILE})
  ENDFOREACH(FILE)

ENDIF(MAGICK++_FOUND)
