#define BOOST_TEST_MODULE direct_Z3
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/Z3_Backend.hpp>

using namespace metaSMT::solver;
using namespace metaSMT;
struct Solver_Fixture {
  typedef DirectSolver_Context<Z3_Backend> ContextType;
  ContextType ctx;
};

#include "test_solver.cpp"
#include "test_QF_BV.cpp"
#include "test_QF_UF.cpp"
#include "test_Array.cpp"
#include "test_unsat.cpp"
#include "test_cardinality.cpp"
#include "test_annotate.cpp"
#include "test_evaluator.cpp"
