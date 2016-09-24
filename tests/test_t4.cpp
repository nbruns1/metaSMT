#include <metaSMT/frontend/QF_BV.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/dynamic_bitset.hpp>
#include <string>

using namespace metaSMT;
using namespace metaSMT::logic;
using namespace metaSMT::logic::QF_BV;
namespace proto = boost::proto;
using boost::dynamic_bitset;

BOOST_FIXTURE_TEST_SUITE(T4, Solver_Fixture )

BOOST_AUTO_TEST_CASE( t4 )
{
  bitvector a = new_bitvector(32);
  bitvector b = new_bitvector(32);
  bitvector c = new_bitvector(32);
  assertion(ctx,equal(bvadd(a,b),c)); //item: a() + b() == c()
  assertion(ctx,bvsle(bvint(10,32),a)); //item1: 10 <= a()
  assertion(ctx,bvsle(a,bvint(20,32))); //item1: a() <= 20, a()
  assertion(ctx,bvsle(bvadd(bvadd(a,b),c),bvint(200,32)));//item1: a() + b() + c() <= 200
  assertion(ctx,equal(bvadd(bvadd(a,b),c),bvsint(100, 32)));//item2: 
  assumption(ctx,equal(bvsint(0, 32), b));
  assumption(ctx,equal(bvsint(0, 32), c));
  BOOST_REQUIRE(! solve(ctx) );
  assumption(ctx,equal(bvsint(0, 32), c));
  BOOST_REQUIRE(! solve(ctx) );
  BOOST_REQUIRE( solve(ctx) );
}

BOOST_AUTO_TEST_SUITE_END() //T4

//  vim: ft=cpp:ts=2:sw=2:expandtab
