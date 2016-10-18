#pragma once

#include "../tags/QF_BV.hpp"
#include "../result_wrapper.hpp"
#include <yices.h>
#include <boost/mpl/map/map40.hpp>
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <list>
#include "../tags/Array.hpp"

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
      //typedef std::list< ::CVC4::Expr > Exprs;

      Yices2()
      {
      }

      ~Yices2() {
      }

	result_type operator() (arraytags::array_var_tag const &var,
                              boost::any const & ) {}

	result_type operator() (arraytags::select_tag const &
                              , result_type const &array
                              , result_type const &index) {}

      void assertion( result_type e ) {
      }

      void assumption( result_type e ) {
      }

      bool solve() {}

      result_wrapper read_value(result_type var) {}

      // predtags
      result_type operator()( predtags::var_tag const & , boost::any ) {}

      result_type operator()( predtags::false_tag , boost::any ) {}

      result_type operator()( predtags::true_tag , boost::any ) {}

      result_type operator()( predtags::not_tag , result_type e ) {}

      result_type operator()( predtags::ite_tag , result_type a, result_type b, result_type c ) {}

      // bvtags
      result_type operator()( bvtags::var_tag const & var, boost::any ) {}

      result_type operator()( bvtags::bit0_tag , boost::any ) {}

      result_type operator()( bvtags::bit1_tag , boost::any ) {}

      result_type operator()( bvtags::bvuint_tag , boost::any arg ) {}

      result_type operator()( bvtags::bvsint_tag , boost::any arg ) {}

      result_type operator()( bvtags::bvbin_tag , boost::any arg ) {}

      result_type operator()( bvtags::bvhex_tag , boost::any arg ) {}

      result_type operator()( bvtags::bvnot_tag , result_type e ) {}

      result_type operator()( bvtags::bvneg_tag , result_type e ) {}

      result_type operator()( bvtags::extract_tag const &
        , unsigned long upper, unsigned long lower
        , result_type e)
      {}

      result_type operator()( bvtags::zero_extend_tag const &
        , unsigned long width
        , result_type e)
      {}

      result_type operator()( bvtags::sign_extend_tag const &
        , unsigned long width
        , result_type e)
      {}

      result_type operator()( predtags::equal_tag const &
                             , result_type a
                             , result_type b) {}

      result_type operator()( predtags::nequal_tag const &
                             , result_type a
                             , result_type b) {}

      result_type operator()( predtags::distinct_tag const &
                             , result_type a
                             , result_type b) {}

      ////////////////////////
      // Fallback operators //
      ////////////////////////

      template <typename TagT>
      result_type operator() (TagT , boost::any ) {}

      template <typename TagT>
      result_type operator() (TagT , result_type a, result_type b) {}

      template <typename TagT>
      result_type operator() (TagT , result_type , result_type , result_type ) {}
	
   void command (Yices2 const & ) { }


    private:
    }; // class Yices2

  } // solver
} // metaSMT
