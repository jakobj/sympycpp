#ifndef SYMPYCPP_NODE_H
#define SYMPYCPP_NODE_H

#include <functional>
#include <memory>
#include <vector>

namespace sympycpp
{

class Node
{
public:
  std::vector< std::shared_ptr< Node > > input_nodes;

  virtual double eval() const = 0;
};

class ValueNode : public Node
{
public:
  double value;

  ValueNode()
  {
    input_nodes.resize( 0 );
  }

  ValueNode( const double value )
    : value( value )
  {
    input_nodes.resize( 0 );
  }

  virtual double eval() const override
  {
    return value;
  }
};


template< double( *F )( double, double ) >
class NaryOpNode : public Node
{
  public:

  NaryOpNode( const std::vector< std::shared_ptr< Node > >& children )
  {
    input_nodes.resize( children.size(), nullptr );
    for ( size_t i = 0; i < children.size(); ++i )
    {
      input_nodes[ i ] = children[ i ];
    }
  }

  virtual double eval() const override
  {
    double result = input_nodes[ 0 ]->eval();
    for ( size_t i = 1; i < input_nodes.size(); ++i )
    {
      result = F( result, input_nodes[ i ]->eval() );
    }
    return result;
  }

};

}

#endif
