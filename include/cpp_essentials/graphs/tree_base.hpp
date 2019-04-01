#ifndef CPP_ESSENTIALS_GRAPHS_TREE_BASE_HPP_
#define CPP_ESSENTIALS_GRAPHS_TREE_BASE_HPP_

#pragma once

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/iterator_facade.hpp>

#include <cpp_essentials/sq/sq.hpp>

namespace cpp_essentials::graphs
{

enum class tree_traversal
{
    children,
    siblings,
    preorder,
    postorder,
    inorder,
    levelorder,
    leaves,
    upward,
};

} /* namespace cpp_essentials::graphs */

#endif /* CPP_ESSENTIALS_GRAPHS_TREE_BASE_HPP_ */
