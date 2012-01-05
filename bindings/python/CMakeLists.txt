find_package( PythonLibs REQUIRED )
include_directories( ${PYTHON_INCLUDE_DIRS} )
include_directories( ../../src )
include_directories( ${metaSMT_INCLUDES} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

macro(set_solver varname)
  set(req_all 1)

  foreach( req ${ARGN} )
    if(NOT ${req} )
      set(req_all 0)
      break()
    endif()
  endforeach( req )
  set(${varname} ${req_all})
endmacro(set_solver)

set_solver(ENABLE_SOLVER_SWORD SWORD_FOUND)
set_solver(ENABLE_SOLVER_BOOLECTOR Boolector_FOUND)
set_solver(ENABLE_SOLVER_Z3 Z3_FOUND)
set_solver(ENABLE_SOLVER_CUDD CUDD_FOUND)
set_solver(ENABLE_SOLVER_AIGER Aiger_FOUND)
set_solver(ENABLE_SOLVER_MINISAT MiniSat_FOUND)
set_solver(ENABLE_SOLVER_MINISAT_EXECUTABLE MiniSat_EXECUTABLE)
set_solver(ENABLE_SOLVER_PICOSAT PicoSAT_FOUND)
set_solver(ENABLE_SOLVER_PICOSAT_EXECUTABLE PicoSAT_EXECUTABLE)
set_solver(ENABLE_SOLVER_PRECOSAT_EXECUTABLE PrecoSat_EXECUTABLE)
set_solver(ENABLE_SOLVER_PLINGELING_EXECUTABLE Plingeling_EXECUTABLE)
set_solver(ENABLE_SOLVER_GLUCOSER Glucoser_EXECUTABLE)
set_solver(ENABLE_SOLVER_SMT2 Z3_EXECUTABLE Boost_IOSTREAMS_LIBRARY)
set_solver(ENABLE_SOLVER_CONSTRAINT Z3_FOUND CUDD_FOUND Boost_THREAD_LIBRARY)

configure_file(python_config.hxx.in ${CMAKE_CURRENT_BINARY_DIR}/python_config.hxx)

add_library( metasmt_python SHARED
  python.cpp
  expressions.cpp
  solvers.cpp
)

set_target_properties( metasmt_python
  PROPERTIES PREFIX ""
  COMPILE_FLAGS "${MiniSat_CXXFLAGS}"
)

target_link_libraries( metasmt_python
  ${Boost_PYTHON_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  metaSMT
)

find_package( PythonInterp )

set(metasmt_PYTHONPATH ${CMAKE_CURRENT_SOURCE_DIR}:${CMAKE_CURRENT_BINARY_DIR})

set(PYMETASMT ${CMAKE_CURRENT_BINARY_DIR}/pymetaSMT)

configure_file(pymetaSMT.in ${PYMETASMT})

if(PYTHONINTERP_FOUND)
  add_test(py_core ${PYMETASMT} ${CMAKE_CURRENT_SOURCE_DIR}/tests/core.py)
  add_test(py_operators ${PYMETASMT} ${CMAKE_CURRENT_SOURCE_DIR}/tests/operators.py)
  add_test(py_support ${PYMETASMT} ${CMAKE_CURRENT_SOURCE_DIR}/tests/support.py)
  add_test(py_smt2 ${PYMETASMT} ${CMAKE_CURRENT_SOURCE_DIR}/tests/smt2.py)
endif()


install(DIRECTORY metasmt 
  DESTINATION python
  FILES_MATCHING PATTERN "*.py"
)
install(DIRECTORY examples 
  DESTINATION python
  FILES_MATCHING PATTERN "*.py"
)
install(TARGETS metasmt_python
  DESTINATION python
)

install(FILES ${PYMETASMT}
  DESTINATION python
  PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_READ GROUP_EXECUTE 
)