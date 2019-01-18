#ifndef SYMPYCPP_FACTORIES_H
#define SYMPYCPP_FACTORIES_H

#include <memory>
#include <vector>

#include "functions.h"
#include "node.h"

namespace sympycpp
{

inline
std::shared_ptr< Node > create_operator_node_float( const std::vector< std::shared_ptr< Node > >& children )
{
  // get rid of uninitialized "precision" node
  std::vector< std::shared_ptr< Node > > reduced_children( children.begin(), children.begin() + 1 );
  return std::make_shared< NaryOpNode< identity > >( reduced_children );
}

template< double( *f )( double, double ) >
inline
std::shared_ptr< Node > create_operator_node_binaryop( const std::vector< std::shared_ptr< Node > >& children )
{
  return std::make_shared< NaryOpNode< f > >( children );
}

inline
std::shared_ptr< Node > create_literal_node_symbol( const std::string )
{
  return std::make_shared< ValueNode >();
}

inline
std::shared_ptr< Node > create_integer_node( const std::string token_value )
{
  std::shared_ptr< ValueNode > node = std::make_shared< ValueNode >();
  node->value = std::stoi( token_value );
  return node;
}

inline
std::shared_ptr< Node > create_float_node( const std::string token_value )
{
  std::shared_ptr< ValueNode > node = std::make_shared< ValueNode >();
  // ignore ' before and after number
  node->value = std::stof( token_value.substr( 1, token_value.size() - 1 ) );
  return node;
}

inline
std::shared_ptr< Node > create_precision_node( const std::string )
{
  // ignore precision
  return nullptr;
}

inline
std::shared_ptr< Node > create_free_float_node( const std::string token_value )
{
  std::shared_ptr< ValueNode > node = std::make_shared< ValueNode >();
  node->value = std::stof( token_value );
  return node;
}

}

#endif
