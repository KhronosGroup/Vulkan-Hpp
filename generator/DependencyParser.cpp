// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#include "DependencyParser.hpp"

#include <cassert>
#include <stdexcept>

std::shared_ptr<Node> DependencyParser::parse()
{
  auto result = parse_or();
  if ( pos != expr.length() )
  {
    throw std::runtime_error( "Unexpected characters at end of expression" );
  }
  return result;
}

bool DependencyParser::match( std::string const & token )
{
  skip_whitespace();
  if ( expr.substr( pos, token.length() ) == token )
  {
    pos += token.length();
    skip_whitespace();
    return true;
  }
  return false;
}

std::shared_ptr<Node> DependencyParser::parse_and()
{
  auto left = parse_primary();
  while ( match( "+" ) )
  {
    auto right = parse_primary();
    left       = std::make_shared<Node>( NodeType::AND, left, right );
  }
  return left;
}

std::shared_ptr<Node> DependencyParser::parse_or()
{
  auto left = parse_and();
  while ( match( "," ) )
  {
    auto right = parse_and();
    left       = std::make_shared<Node>( NodeType::OR, left, right );
  }
  return left;
}

std::shared_ptr<Node> DependencyParser::parse_primary()
{
  if ( match( "(" ) )
  {
    auto result = parse_or();
    if ( !match( ")" ) )
    {
      throw std::runtime_error( "Expected closing parenthesis" );
    }
    return result;
  }
  return parse_variable();
}

std::shared_ptr<Node> DependencyParser::parse_variable()
{
  size_t start = pos;
  while ( pos < expr.length() && ( std::isalnum( expr[pos] ) || ( expr[pos] == '_' ) ) )
  {
    pos++;
  }
  if ( start == pos )
  {
    throw std::runtime_error( "Expected variable" );
  }
  return std::make_shared<Node>( NodeType::VAR, expr.substr( start, pos - start ) );
}

void DependencyParser::skip_whitespace()
{
  while ( pos < expr.length() && std::isspace( expr[pos] ) )
  {
    pos++;
  }
}

std::shared_ptr<Node> distribute_and_over_or( std::shared_ptr<Node> node )
{
  if ( node->type == NodeType::AND )
  {
    if ( node->left->type == NodeType::OR )
    {
      return std::make_shared<Node>( NodeType::OR,
                                     distribute_and_over_or( std::make_shared<Node>( NodeType::AND, node->left->left, node->right ) ),
                                     distribute_and_over_or( std::make_shared<Node>( NodeType::AND, node->left->right, node->right ) ) );
    }
    else if ( node->right->type == NodeType::OR )
    {
      return std::make_shared<Node>( NodeType::OR,
                                     distribute_and_over_or( std::make_shared<Node>( NodeType::AND, node->left, node->right->left ) ),
                                     distribute_and_over_or( std::make_shared<Node>( NodeType::AND, node->left, node->right->right ) ) );
    }
  }
  return node;
}

std::shared_ptr<Node> normalize( std::shared_ptr<Node> node )
{
  if ( node->left )
  {
    node->left = normalize( node->left );
  }
  if ( node->right )
  {
    node->right = normalize( node->right );
  }
  if ( node->type == NodeType::AND )
  {
    node = distribute_and_over_or( node );
  }
  return node;
}

std::vector<std::vector<std::string>> vectorize( std::shared_ptr<Node> node )
{
  if ( node->type == NodeType::VAR )
  {
    return { { node->value } };
  }
  else if ( node->type == NodeType::OR )
  {
    auto left  = vectorize( node->left );
    auto right = vectorize( node->right );
    left.insert( left.end(), right.begin(), right.end() );
    return left;
  }
  else
  {
    assert( node->type == NodeType::AND );
    auto                                  left  = vectorize( node->left );
    auto                                  right = vectorize( node->right );
    std::vector<std::vector<std::string>> result;
    for ( auto const & l : left )
    {
      for ( auto const & r : right )
      {
        result.push_back( l );
        result.back().insert( result.back().end(), r.begin(), r.end() );
      }
    }
    return result;
  }
}
