#ifndef PARSER_H
#define PARSER_H

#include <cmath>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "node.h"
#include "token.h"

namespace sympycpp
{

class Parser
{
private:

  // maches symbols in literals
  const std::string str_regex_symbol = "'x_[0-9]+'";

  // stores regular expressions to match literals to factories for
  // correspondng literal nodes
  std::map< std::string, std::function< std::shared_ptr< Node >( std::string token_value ) > > _literal_regexs;

  // stores names of operators to factories for correspondng operator nodes
  std::map<
    std::string, std::function< std::shared_ptr< Node >( const std::vector< std::shared_ptr< Node > >& children ) >
    > _operator_names;

  // maps symbol names to input nodes
  // std::map<  std::shared_ptr< Node > > _symbol_to_input_node;

  // stores symbol names with correspnding input nodes
  std::map< std::string, std::shared_ptr< Node > > _input_nodes;


  // creates a new node based on the tokens value and sets it children
  std::shared_ptr< Node > _create_new_node( const Token token, const std::vector< std::shared_ptr< Node > >& children );

  // parses an expression recursively
  std::shared_ptr< Node > _parse_recursively( const std::vector< Token >& tokens );

  // reads the next argument from Token stream
  void _read_next_arg( std::vector< Token >::const_iterator& it_token,
                       const std::vector< Token >::const_iterator& end,
                       std::vector< Token >& tokens ) const;

  // checks whether a Token corresponds to a Symbol
  bool _is_symbol( const Token& token ) const;

  // returns Symbol node corresponding to symbol name
  std::shared_ptr< Node > _get_input_node( const std::string name );

  // checks whether next token in stream has a certain value
  void _expect_value( const std::vector< Token >::const_iterator& it_token, const std::set< std::string > what ) const;

  // skips next token in stream, throws an exception if the token does not have the correct value
  void _skip_value( std::vector< Token >::const_iterator& it_token, const std::set< std::string > what ) const;

  // checks whether next token in stream has a certain type
  void _expect_type( const std::vector< Token >::const_iterator& it_token, const std::set< token_type > what ) const;

  // skips next token in stream, throws an exception if the token does not have the correct type
  void _skip_type( std::vector< Token >::const_iterator& it_token, const std::set< token_type > what ) const;

public:
  std::shared_ptr< Node > root;

  Parser();

  void parse( const std::string expr, const size_t n_inputs );
  double eval( const std::vector< double >& inputs );
};

}

#endif
