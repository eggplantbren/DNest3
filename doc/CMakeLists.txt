find_package (LATEX)

if (PDFLATEX_COMPILER)
  add_custom_command(
    OUTPUT manual.pdf
    COMMAND ${PDFLATEX_COMPILER} manual.tex
    COMMAND ${PDFLATEX_COMPILER} manual.tex
    COMMAND ${PDFLATEX_COMPILER} manual.tex
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
  )
  add_custom_target(manual ALL echo
    DEPENDS manual.pdf
  )
else (PDFLATEX_COMPILER)
    message (STATUS "pdflatex not available; not building documentation")
endif (PDFLATEX_COMPILER)
