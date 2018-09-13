# Copyright (c) 2017-2018 Hartmut Kaiser
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(tests
    simple_loop
   )
set(py_tests
    perf_test)

foreach(test ${tests})
  set(sources ${test}.cpp)

  source_group("Source Files" FILES ${sources})

  # add executable
  add_phylanx_executable(${test}_test
    SOURCES ${sources}
    ${${test}_FLAGS}
    EXCLUDE_FROM_ALL
    FOLDER "Tests/Performance/")

  add_phylanx_pseudo_target(tests.performance.${test})
  add_phylanx_pseudo_dependencies(tests.performance tests.performance.${test})
  add_phylanx_pseudo_dependencies(tests.performance.${test} ${test}_test_exe)

endforeach()

foreach(test ${py_tests})
  set(script ${test}.py)

  add_phylanx_python_performance_test("python" ${test}
          SCRIPT ${script}
          FOLDER "Tests/Performance"
          DEPENDS phylanx_py python_setup
          WORKING_DIRECTORY ${PHYLANX_PYTHON_EXTENSION_LOCATION}
          ENVIRONMENT "PYTHONPATH=${PHYLANX_PYTHON_EXTENSION_LOCATION}")

  add_phylanx_pseudo_target(tests.performance.${test}_py)
  add_phylanx_pseudo_dependencies(
          tests.performance tests.performance.${test}_py)
  add_phylanx_pseudo_dependencies(
          tests.performance.${test}_py ${test}_test_py)

endforeach()
