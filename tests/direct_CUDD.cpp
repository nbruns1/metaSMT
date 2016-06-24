#define BOOST_TEST_MODULE direct_CUDD
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/CUDD_Context.hpp>

using namespace metaSMT::solver;
using namespace metaSMT;
struct Solver_Fixture {
  typedef DirectSolver_Context<CUDD_Context> ContextType;
  ContextType ctx ;
};

#include "test_solver.cpp"
#include "test_unsat.cpp"
#include "test_lazy.cpp"
