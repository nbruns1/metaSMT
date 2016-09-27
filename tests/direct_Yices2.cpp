#define BOOST_TEST_MODULE direct_Yices2
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/Yices2.hpp>

using namespace metaSMT;
struct Solver_Fixture {
  typedef DirectSolver_Context<metaSMT::solver::Yices2> ContextType;
  ContextType ctx;
};

#include "test_solver.cpp"
#include "test_unsat.cpp"
#include "test_QF_UF.cpp"
#include "test_QF_BV.cpp"
#include "test_Array.cpp"
