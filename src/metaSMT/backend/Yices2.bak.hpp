#pragma once

#include "../tags/QF_BV.hpp"
#include "../result_wrapper.hpp"
#include <yices.h>
#include <boost/mpl/map/map40.hpp>
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <list>
#include "../tags/Array.hpp"
#include <iostream>

namespace metaSMT {
  namespace solver {
    namespace predtags = ::metaSMT::logic::tag;
    namespace bvtags = ::metaSMT::logic::QF_BV::tag;
    namespace arraytags = ::metaSMT::logic::Array::tag;

    /**
     * @ingroup Backend
     * @class CVC4 CVC4.hpp metaSMT/backend/CVC4.hpp
     * @brief The CVC4 backend
     */
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

	result_type operator() (arraytags::array_var_tag const &var,
                              boost::any const & ) {}

	result_type operator() (arraytags::select_tag const &
                              , result_type const &array
                              , result_type const &index) {
		return yices_select(index,array);
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
        //return (yices_check_context(ctx, NULL) == STATUS_SAT);
	return false;
	}

      result_wrapper read_value(result_type var) {}

      // predtags
      result_type operator()( predtags::var_tag const & , boost::any ) {
	type_t bool_type = yices_bool_type();
	return yices_new_uninterpreted_term(bool_type);
	}

      result_type operator()( predtags::false_tag , boost::any ) {return yices_false();}

      result_type operator()( predtags::true_tag , boost::any ) {return yices_true();}

      result_type operator()( predtags::not_tag , result_type e ) {return yices_not(e);}

      result_type operator()( predtags::ite_tag , result_type a, result_type b, result_type c ) {return yices_ite(a,b,c);}

      // bvtags
      result_type operator()( bvtags::var_tag const & var, boost::any ) {
	assert ( var.width != 0 );
	return yices_bv_type(var.width);
      }

      result_type operator()( bvtags::bit0_tag , boost::any ) {return yices_bvconst_zero(1);}

      result_type operator()( bvtags::bit1_tag , boost::any ) {return yices_bvconst_one(1);}

      result_type operator()( bvtags::bvuint_tag , boost::any arg ) {
	typedef boost::tuple<unsigned long, unsigned long> Tuple;
        Tuple tuple = boost::any_cast<Tuple>(arg);
        unsigned long value = boost::get<0>(tuple);
        unsigned long width = boost::get<1>(tuple);

        return yices_bvconst_uint64(width,value);
	}

      result_type operator()( bvtags::bvsint_tag , boost::any arg ) {
	typedef boost::tuple<unsigned long, unsigned long> Tuple;
        Tuple tuple = boost::any_cast<Tuple>(arg);
        unsigned long value = boost::get<0>(tuple);
        unsigned long width = boost::get<1>(tuple);
	
        return yices_bvconst_int64(width,value);
}

      result_type operator()( bvtags::bvbin_tag , boost::any arg ) {
	std::string val = boost::any_cast<std::string>(arg);
	return yices_parse_bvbin(val.c_str());
	}

      result_type operator()( bvtags::bvhex_tag , boost::any arg ) {
	std::string hex = boost::any_cast<std::string>(arg);
	return yices_parse_bvhex(hex.c_str());
	}

      result_type operator()( bvtags::bvnot_tag , result_type e ) {return yices_bvnot(e);}

      result_type operator()( bvtags::bvneg_tag , result_type e ) {return yices_bvneg(e);}

      result_type operator()( bvtags::extract_tag const &
        , unsigned long upper, unsigned long lower
        , result_type e)
      {return yices_bvextract(e,lower,upper);}

      result_type operator()( bvtags::zero_extend_tag const &
        , unsigned long width
        , result_type e)
      {return yices_zero_extend(e,width);}

      result_type operator()( bvtags::sign_extend_tag const &
        , unsigned long width
        , result_type e)
      {return yices_sign_extend(e,width);}

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

      ////////////////////////
      // Fallback operators //
      ////////////////////////

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
      result_type operator() (TagT , result_type a, result_type b) {
	/*
	namespace mpl = boost::mpl;
	        typedef mpl::map33<
          // binary Logic tags
          mpl::pair<predtags::and_tag,     Yices_F2<&yices_and2> >
        , mpl::pair<predtags::or_tag,      Yices_F2<&yices_or2> >
        , mpl::pair<predtags::xor_tag,     Yices_F2<&yices_xor2> >
        , mpl::pair<predtags::implies_tag, Yices_F2<&yices_implies> >
        // binary QF_BV tags
        , mpl::pair<bvtags::bvand_tag,     Yices_F2<&yices_bvand2> >
        , mpl::pair<bvtags::bvnand_tag,    Yices_F2<&yices_bvnand> >
        , mpl::pair<bvtags::bvor_tag,      Yices_F2<&yices_bvor2> >
        , mpl::pair<bvtags::bvnor_tag,     Yices_F2<&yices_bvnor> >
        , mpl::pair<bvtags::bvxor_tag,     Yices_F2<&yices_bvxor2> >
        , mpl::pair<bvtags::bvxnor_tag,    Yices_F2<&yices_bvxnor> >
        , mpl::pair<bvtags::bvadd_tag,     Yices_F2<&yices_bvadd> >
        , mpl::pair<bvtags::bvsub_tag,     Yices_F2<&yices_bvsub> >
        , mpl::pair<bvtags::bvmul_tag,     Yices_F2<&yices_bvmul> >
        , mpl::pair<bvtags::bvudiv_tag,    Yices_F2<&yices_bvdiv> >
        , mpl::pair<bvtags::bvurem_tag,    Yices_F2<&yices_bvrem> >
        , mpl::pair<bvtags::bvsdiv_tag,    Yices_F2<&yices_bvsdiv> >
        , mpl::pair<bvtags::bvsrem_tag,    Yices_F2<&yices_bvsrem> >
        , mpl::pair<bvtags::bvslt_tag,     Yices_F2<&yices_bvslt_atom> >
        , mpl::pair<bvtags::bvsle_tag,     Yices_F2<&yices_bvsle_atom> >
        , mpl::pair<bvtags::bvsgt_tag,     Yices_F2<&yices_bvsgt_atom> >
        , mpl::pair<bvtags::bvsge_tag,     Yices_F2<&yices_bvsge_atom> >
        //, mpl::pair<bvtags::bvult_tag,     Yices_F2<BITVECTOR_ULT> >
        //, mpl::pair<bvtags::bvule_tag,     Yices_F2<BITVECTOR_ULE> >
        //, mpl::pair<bvtags::bvugt_tag,     Yices_F2<BITVECTOR_UGT> >
        //, mpl::pair<bvtags::bvuge_tag,     Yices_F2<BITVECTOR_UGE> >
        //, mpl::pair<bvtags::concat_tag,    Yices_F2<&yices_bvconcat> >
        //, mpl::pair<bvtags::bvcomp_tag,    Yices_F2<BITVECTOR_COMP> >
        , mpl::pair<bvtags::bvshl_tag,     Yices_F2<&yices_bvshl> >
        //, mpl::pair<bvtags::bvshr_tag,     Yices_F2<&yices_bvshr> >
        , mpl::pair<bvtags::bvashr_tag,    Yices_F2<&yices_bvashr> >
        > Opcode_Map;*/
}

      template <typename TagT>
      result_type operator() (TagT , result_type , result_type , result_type ) {}
	
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
