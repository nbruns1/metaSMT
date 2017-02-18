#include "../metaSMT/impl/_var_id.hpp"

#include <atomic>

unsigned metaSMT::impl::new_var_id()
{
  static std::atomic_uint _id ( 0u );
  ++_id;
  return _id;
} 

