// SPDX-FileCopyrightText: 2026 NVIDIA CORPORATION
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <memory>
#include <string>
#include <vector>

enum class NodeType
{
  AND,
  OR,
  VAR
};

struct Node
{
  NodeType              type;
  std::string           value;
  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;

  Node( NodeType t, std::string const & val ) : type( t ), value( val ), left( nullptr ), right( nullptr ) {}

  Node( NodeType t, std::shared_ptr<Node> l, std::shared_ptr<Node> r ) : type( t ), left( l ), right( r ) {}
};

class DependencyParser
{
public:
  DependencyParser( std::string const & expr ) : expr( expr ), pos( 0 ) {}

  std::shared_ptr<Node> parse();

private:
  bool                  match( std::string const & token );
  std::shared_ptr<Node> parse_and();
  std::shared_ptr<Node> parse_or();
  std::shared_ptr<Node> parse_primary();
  std::shared_ptr<Node> parse_variable();
  void                  skip_whitespace();

private:
  std::string expr;
  size_t      pos;
};

std::shared_ptr<Node>                 distribute_and_over_or( std::shared_ptr<Node> node );
std::shared_ptr<Node>                 normalize( std::shared_ptr<Node> node );
std::vector<std::vector<std::string>> vectorize( std::shared_ptr<Node> node );
