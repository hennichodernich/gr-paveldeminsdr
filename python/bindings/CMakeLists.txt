# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT paveldeminsdr_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT paveldeminsdr_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND paveldeminsdr_python_files
    source_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(paveldeminsdr
   ../..
   gr::paveldeminsdr
   "${paveldeminsdr_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET paveldeminsdr_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:paveldeminsdr_python>
    ${CMAKE_BINARY_DIR}/test_modules/paveldeminsdr/
)

install(TARGETS paveldeminsdr_python DESTINATION ${GR_PYTHON_DIR}/paveldeminsdr COMPONENT pythonapi)
