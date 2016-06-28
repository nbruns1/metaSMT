#pragma once

#include "../result_wrapper.hpp"
#include "../tags/Logic.hpp"
#include "../tags/QF_BV.hpp"
#include <boost/mpl/map/map40.hpp>
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <list>
#include <iostream>

#include <yices.h>

namespace metaSMT {
  namespace solver {
    namespace predtags = ::metaSMT::logic::tag;
    namespace bvtags = ::metaSMT::logic::QF_BV::tag;

    class Yices2 {
    public:
      typedef term_t result_type;
      typedef std::list< term_t > Exprs;

      private:
      Exprs assumptions_;
      Exprs assertions_;
      bool isPushed_;
      context_t *ctx;
      public:
      Yices2()
      {
	yices_init();
	ctx_config_t *config = yices_new_config();
	yices_set_config(config, "mode", "interactive");
	ctx = yices_new_context(config);
	yices_free_config(config);
	
      }

      ~Yices2() {
	yices_exit();
      }

      void assertion( result_type e ) {
	assertions_.push_back( e );
      }

      void assumption( result_type e ) {
	assumptions_.push_back( e );
      }

      bool solve() {
	removeOldAssumptions();
        pushAssertions();
        pushAssumptions();
        return (yices_check_context(ctx, NULL) == STATUS_SAT);
	}

      result_wrapper read_value(result_type var) {}

	result_type operator()( predtags::var_tag const & , boost::any ) {
	type_t bool_type = yices_bool_type();
	return yices_new_uninterpreted_term(bool_type);
	}

      // predtags
      result_type operator()( predtags::false_tag , boost::any ) {return yices_false();}

      result_type operator()( predtags::true_tag , boost::any ) {return yices_true();}

      result_type operator()( predtags::not_tag , result_type e ) {return yices_not(e);}

      result_type operator()( predtags::equal_tag const &
                             , result_type a
                             , result_type b) {
	if (yices_term_is_bool(a) && yices_term_is_bool(b) ) {
          return yices_iff(a,b);
        } else {
          return yices_eq(a,b);
        }
	}

      result_type operator()( predtags::nequal_tag const &
                             , result_type a
                             , result_type b) {return yices_neq(a,b);}

      result_type operator()( predtags::distinct_tag const &
                             , result_type a
                             , result_type b) {return yices_neq(a,b);}

     result_type operator()( predtags::and_tag const &
                             , result_type a
                             , result_type b) {return yices_and2(a,b);}

     result_type operator()( predtags::nand_tag const &
                             , result_type a
                             , result_type b) {return yices_not(yices_and2(a,b));}

     result_type operator()( predtags::or_tag const &
                             , result_type a
                             , result_type b) {return yices_or2(a,b);}

     result_type operator()( predtags::nor_tag const &
                             , result_type a
                             , result_type b) {return yices_not(yices_or2(a,b));}

     result_type operator()( predtags::xor_tag const &
                             , result_type a
                             , result_type b) {return yices_xor2(a,b);}

     result_type operator()( predtags::xnor_tag const &
                             , result_type a
                             , result_type b) {return yices_not(yices_xor2(a,b));}

     result_type operator()( predtags::implies_tag const &
                             , result_type a
                             , result_type b) {return yices_implies(a,b);}

     result_type operator()( predtags::ite_tag , result_type a, result_type b, result_type c ) {return yices_ite(a,b,c);}



      
      ////////////////////////
      // Fallback operators //
      ////////////////////////
/*
	template< result_type (*FN) (result_type, result_type) >
        struct Yices_F2 {
          static result_type exec(result_type x, result_type y)
          { return (*FN)(x,y);}
        };

      template <typename TagT>
      result_type operator() (TagT , boost::any ) {
	assert(false && "unknown operator");
        return yices_false();
	}

      template <typename TagT>
      result_type operator() (TagT , result_type a, result_type b) {}

      template <typename TagT>
      result_type operator() (TagT , result_type , result_type , result_type ) {}
*/
   void command (Yices2 const & ) { }


    private:

     void removeOldAssumptions() {
        if (isPushed_) {
          yices_pop(ctx);
          isPushed_ = false;
        }
      }

      void pushAssumptions() {
	yices_push(ctx);
        isPushed_ = true;

        applyAssertions(assumptions_);
        assumptions_.clear();
      }

      void pushAssertions() {
        applyAssertions(assertions_);
        assertions_.clear();
      }

      void applyAssertions(Exprs const& expressions) {
        for ( Exprs::const_iterator it = expressions.begin(),
              ie = expressions.end(); it != ie; ++it ) {
          yices_assert_formula(ctx, *it);
        }
      }
    
    }; // class Yices2

  } // solver
} // metaSMT
