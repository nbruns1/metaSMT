#define BOOST_TEST_MODULE direct_Boolector
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/Boolector.hpp>

using namespace metaSMT::solver;
using namespace metaSMT;
struct Solver_Fixture {
  typedef DirectSolver_Context<Boolector> ContextType;
  ContextType ctx;
};

#include "test_solver.cpp"
#include "test_QF_BV.cpp"
#include "test_Array.cpp"
#include "test_unsat.cpp"
#include "test_cardinality.cpp"
#include "test_lazy.cpp"
#include "test_annotate.cpp"
#include "test_evaluator.cpp"
