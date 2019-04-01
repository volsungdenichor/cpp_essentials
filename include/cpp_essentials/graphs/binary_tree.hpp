#ifndef CPP_ESSENTIALS_GRAPHS_BINARY_TREE_HPP_
#define CPP_ESSENTIALS_GRAPHS_BINARY_TREE_HPP_

#pragma once

#include <array>
#include <queue>

#include <cpp_essentials/graphs/tree_base.hpp>

namespace cpp_essentials::graphs
{

namespace detail
{

template <size_t Dim>
using is_binary_tree = std::enable_if_t<Dim == 2>;

template <class T, size_t D>
class tree_node_struct
{
public:
    tree_node_struct(const T& data)
        : _data(data)
        , _parent { nullptr }
    {
        std::fill(_children.begin(), _children.end(), nullptr);
    }

    tree_node_struct()
        : tree_node_struct { T {} }
    {
    }

    const T& data() const
    {
        return _data;
    }

    T& data()
    {
        return _data;
    }

    tree_node_struct* clone() const
    {
        tree_node_struct* result = new tree_node_struct(data());

        for (size_t i = 0; i < D; ++i)
        {
            const auto child = _children[i];

            if (child)
            {
                result->set_child(i, child->clone());
            }
        }

        return result;
    }

    tree_node_struct* parent() const
    {
        return _parent;
    }

    tree_node_struct* root() const
    {
        return parent() ? parent()->root() : const_cast<tree_node_struct*>(this);
    }

    void set_child(size_t index, tree_node_struct* child)
    {
        EXPECTS(child != nullptr);
        EXPECTS(child->parent() == nullptr);
        EXPECTS(index < D);

        delete _children[index];

        _children[index] = child;

        child->_parent = this;
    }

    void move_child(size_t index, tree_node_struct* child)
    {
        EXPECTS(child != nullptr);
        child->detach();

        if (_children[index])
        {
            detach_child(_children[index]);
        }

        set_child(index, child);
    }

    tree_node_struct* child_at(size_t index) const
    {
        EXPECTS(index < D);

        return _children[index];
    }

    size_t index() const
    {
        if (parent())
        {
            for (size_t i = 0; i < D; ++i)
            {
                if (parent()->_children[i] == this)
                {
                    return i;
                }
            }
        }

        return 0;
    }

    size_t depth() const
    {
        return parent() ? parent()->depth() + 1 : 0;
    }

    size_t max_depth() const
    {
        size_t result = 0;

        for (const auto& child : _children)
        {
            result = std::max(result, child ? child->max_depth() : 0);
        }

        return result + 1;
    }

    size_t children_count() const
    {
        size_t result = 0;

        for (const auto& child : _children)
        {
            if (child)
            {
                ++result;
            }
        }

        return result;
    }

    size_t descendants_count() const
    {
        size_t result = 0;

        for (const auto& child : _children)
        {
            if (child)
            {
                result += child->descendants_count() + 1;
            }
        }

        return result;
    }

    size_t siblings_count() const
    {
        return parent() ? parent()->children_count() - 1 : 0;
    }

    tree_node_struct* ancestor_at(size_t index) const
    {
        for (auto i = parent(); i; i = i->parent())
        {
            if (index == 0)
            {
                return i;
            }

            --index;
        }

        return nullptr;
    }

    tree_node_struct* first_leaf() const
    {
        auto result = const_cast<tree_node_struct*>(this);

        while (result && result->first_child())
        {
            result = result->first_child();
        }

        return result;
    }

    tree_node_struct* next_sibling() const
    {
        return get_sibling(0, D - 1, +1);
    }

    tree_node_struct* prev_sibling() const
    {
        return get_sibling(D - 1, 0, -1);
    }

    tree_node_struct* first_child() const
    {
        for (const auto& child : _children)
        {
            if (child)
            {
                return child;
            }
        }

        return nullptr;
    }

    void detach_child(tree_node_struct* child)
    {
        EXPECTS(child != nullptr);
        EXPECTS(child->parent() == this);

        child->_parent = nullptr;
        
        for (size_t i = 0; i < D; ++i)
        {
            if (_children[i] == child)
            {
                _children[i] = nullptr;
            }
        }
    }

    void detach()
    {
        if (_parent)
        {
            _parent->detach_child(this);
        }
    }

private:
    tree_node_struct* get_sibling(int first, int last, int dir) const
    {
        bool ok = false;

        if (parent())
        {
            for (auto i = first; i != last + dir; i += dir)
            {
                auto sibling = parent()->_children[i];

                if (sibling == this)
                {
                    ok = true;
                }
                else if (sibling && ok)
                {
                    return sibling;
                }
            }
        }

        return nullptr;
    }

    T _data;
    tree_node_struct* _parent;
    std::array<tree_node_struct*, D> _children;
};



template <tree_traversal Traversal, class T, size_t D>
struct tree_iterator_impl
{

};


template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::children, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        return node->first_child();
    }

    node_struct* next(node_struct* node)
    {
        return node->next_sibling();
    }
};



template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::siblings, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        _self = node;

        return node->parent() ? advance(node->parent()->first_child()) : nullptr;
    }

    node_struct* next(node_struct* node)
    {
        return advance(node->next_sibling());
    }

private:
    node_struct* advance(node_struct* node)
    {
        return node && node == _self ? node->next_sibling() : node;
    }

    node_struct* _self;
};



template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::upward, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        return node;
    }

    node_struct* next(node_struct* node)
    {
        return node->parent();
    }
};



template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::preorder, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        return node;
    }

    node_struct* next(node_struct* node)
    {
        auto first_child = node->first_child();

        if (first_child)
        {
            return first_child;
        }

        for (auto i = node; i; i = i->parent())
        {
            auto next_sibling = i->next_sibling();

            if (next_sibling)
            {
                return next_sibling;
            }
        }

        return nullptr;
    }
};



template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::postorder, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        return node->first_leaf();
    }

    node_struct* next(node_struct* node)
    {
        auto next_sibling = node->next_sibling();

        return next_sibling
            ? next_sibling->first_leaf()
            : node->parent();
    }
};



template <class T>
struct tree_iterator_impl<tree_traversal::inorder, T, 2>
{
    using node_struct = tree_node_struct<T, 2>;

    node_struct* init(node_struct* node)
    {
        add_node(node);

        auto result = _queue.front();

        _queue.pop();

        return result;
    }

    node_struct* next(node_struct* node)
    {
        if (_queue.empty())
        {
            return nullptr;
        }

        auto result = _queue.front();

        _queue.pop();

        return result;
    }

    void add_node(node_struct* node)
    {
        if (!node)
        {
            return;
        }

        add_node(node->child_at(0));
        _queue.push(node);
        add_node(node->child_at(1));
    }

private:
    std::queue<node_struct*> _queue;
};



template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::levelorder, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        return node;
    }

    node_struct* next(node_struct* node)
    {
        for (auto i = node->first_child(); i; i = i->next_sibling())
        {
            _queue.push(i);
        }

        if (_queue.empty())
        {
            return nullptr;
        }

        auto result = _queue.front();

        _queue.pop();

        return result;
    }

private:
    std::queue<node_struct*> _queue;
};



template <class T, size_t D>
struct tree_iterator_impl<tree_traversal::leaves, T, D>
{
    using node_struct = tree_node_struct<T, D>;

    node_struct* init(node_struct* node)
    {
        return node->first_leaf();
    }

    node_struct* next(node_struct* node)
    {
        for (auto i = node; i; i = i->parent())
        {
            auto next_sibling = i->next_sibling();

            if (next_sibling)
            {
                return next_sibling->first_leaf();
            }
        }

        return nullptr;
    }
};



template <class T, size_t D, tree_traversal Traversal>
class tree_iterator : public core::iterator_facade<tree_iterator<T, D, Traversal>, std::forward_iterator_tag, T>
{
public:
    using data_type = tree_node_struct<T, D>;
    using base_type = core::iterator_facade<tree_iterator<T, D, Traversal>, std::forward_iterator_tag, T>;

    INHERIT_ITERATOR_FACADE_TYPES(base_type)

    tree_iterator(data_type* data)
        : _impl {}
    {
        _data = data ? _impl.init(data) : nullptr;
    }

    tree_iterator()
        : tree_iterator { nullptr }
    {
    }

    template <tree_traversal OtherTraversal>
    tree_iterator(const tree_iterator<T, D, OtherTraversal>& other)
        : tree_iterator { other._data }
    {
    }

    reference ref() const
    {
        EXPECTS(_data != nullptr);
        return _data->data();
    }

    pointer ptr() const
    {
        EXPECTS(_data != nullptr);
        return &_data->data();
    }

    bool is_equal(const tree_iterator& other) const
    {
        return _data == other._data;
    }

    void inc()
    {
        EXPECTS(_data != nullptr);
        _data = _impl.next(_data);
    }

    explicit operator bool() const
    {
        return _data != nullptr;
    }

    size_t index() const
    {
        return _data ? _data->index() : 0;
    }

    size_t depth() const
    {
        return _data ? _data->depth() : 0;
    }

    size_t max_depth() const
    {
        return _data ? _data->max_depth() : 0;
    }

    size_t children_count() const
    {
        return _data ? _data->children_count() : 0;
    }

    size_t descendants_count() const
    {
        return _data ? _data->descendants_count() : 0;
    }

    size_t siblings_count() const
    {
        return _data ? _data->siblings_count() : 0;
    }

    tree_iterator root() const
    {
        return { _data ? _data->root() : nullptr };
    }

    tree_iterator parent() const
    {
        return { _data ? _data->parent() : nullptr };
    }

    tree_iterator ancestor(size_t index) const
    {
        return { _data ? _data->ancestor_at(index) : nullptr };
    }

    tree_iterator child(size_t index) const
    {
        return { _data ? _data->child_at(index) : nullptr };
    }

    template <size_t Index>
    tree_iterator child() const
    {
        static_assert(Index < D, "invalid index");
        return child(Index);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    tree_iterator left_child() const
    {
        return child<0>();
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    tree_iterator right_child() const
    {
        return child<1>();
    }

    bool has_child(size_t index) const
    {
        return !!child(index);
    }

    template <size_t Index>
    bool has_child() const
    {
        return !!child<Index>();
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    bool has_left_child() const
    {
        return has_child<0>();
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    bool has_right_child() const
    {
        return has_child<1>();
    }

    bool is_leaf() const
    {
        return _data && _data->first_child() == nullptr;
    }

    template <tree_traversal TraversalType>
    core::iterator_range<tree_iterator<T, D, TraversalType>> traverse() const
    {
        tree_iterator<T, D, TraversalType> begin { _data };
        tree_iterator<T, D, TraversalType> end {};

        return { begin, end };
    }

    auto children() const
    {
        return traverse<tree_traversal::children>();
    }

    auto siblings() const
    {
        return traverse<tree_traversal::siblings>();
    }

    auto preorder() const
    {
        return traverse<tree_traversal::preorder>();
    }

    auto postorder() const
    {
        return traverse<tree_traversal::postorder>();
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    auto inorder() const
    {
        return traverse<tree_traversal::inorder>();
    }

    auto levelorder() const
    {
        return traverse<tree_traversal::levelorder>();
    }

    auto leaves() const
    {
        return traverse<tree_traversal::leaves>();
    }

    auto upward() const
    {
        return traverse<tree_traversal::upward>();
    }

    data_type* _data;
    tree_iterator_impl<Traversal, T, D> _impl;
};



template <class T, size_t D>
using children_iterator = tree_iterator<T, D, tree_traversal::children>;

template <class T, size_t D>
using siblings_iterator = tree_iterator<T, D, tree_traversal::siblings>;

template <class T, size_t D>
using preorder_iterator = tree_iterator<T, D, tree_traversal::preorder>;

template <class T, size_t D>
using postorder_iterator = tree_iterator<T, D, tree_traversal::postorder>;

template <class T>
using inorder_iterator = tree_iterator<T, 2, tree_traversal::inorder>;

template <class T, size_t D>
using levelorder_iterator = tree_iterator<T, D, tree_traversal::levelorder>;

template <class T, size_t D>
using leaves_iterator = tree_iterator<T, D, tree_traversal::leaves>;

template <class T, size_t D>
using upward_iterator = tree_iterator<T, D, tree_traversal::upward>;

} /* namespace detail */

using detail::is_binary_tree;

template <class T, size_t D = 0>
class tree
{
private:
    using data_type = detail::tree_node_struct<T, D>;

public:
    using value_type = T;

    using children_iterator = detail::children_iterator<T, D>;
    using siblings_iterator = detail::siblings_iterator<T, D>;
    using preorder_iterator = detail::preorder_iterator<T, D>;
    using postorder_iterator = detail::postorder_iterator<T, D>;
    using levelorder_iterator = detail::levelorder_iterator<T, D>;
    using leaves_iterator = detail::leaves_iterator<T, D>;
    using upward_iterator = detail::upward_iterator<T, D>;
    using inorder_iterator = std::enable_if_t<D == 2, detail::inorder_iterator<T>>;

    using iterator = preorder_iterator;

    tree()
        : _root { nullptr }
    {
    }

    tree(const tree& other)
        : _root { other._root ? other._root->clone() : nullptr }
    {
    }

    tree(tree&& other)
        : tree {}
    {
        swap();
    }

    template <tree_traversal Traversal>
    tree(detail::tree_iterator<T, D, Traversal> iter)
        : _root { iter._data ? iter._data->clone() : nullptr }
    {
    }

    ~tree()
    {
        clear();
    }

    tree& operator =(tree other)
    {
        swap(other);
        return *this;
    }

    void swap(tree& other)
    {
        std::swap(_root, other._root);
    }

    size_t size() const
    {
        return empty() ? 0 : begin().descendants_count() + 1;
    }

    bool empty() const
    {
        return _root == nullptr;
    }

    iterator root() const
    {
        return iterator { _root };
    }

    iterator root(const value_type& data)
    {
        clear();

        return set_root(new data_type { data });
    }

    iterator root(iterator iter)
    {        
        EXPECTS(!!iter);

        clear();

        return iter
            ? set_root(iter._data->clone())
            : iterator {};
    }

    iterator set_child(iterator iter, size_t index, const value_type& data)
    {
        return set_child(iter, index, new data_type { data });
    }

    template <size_t Index>
    iterator set_child(iterator iter, const value_type& data)
    {
        static_assert(Index < D, "invalid index");
        return set_child(iter, Index, data);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator set_left_child(iterator iter, const value_type& data)
    {
        return set_child<0>(iter, data);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator set_right_child(iterator iter, const value_type& data)
    {
        return set_child<1>(iter, data);
    }

    iterator set_child(iterator iter, size_t index, iterator other)
    {
        EXPECTS(!!other);

        return iter
            ? set_child(iter, index, other._data->clone())
            : iterator {};
    }

    template <size_t Index>
    iterator set_child(iterator iter, iterator other)
    {
        static_assert(Index < D, "invalid index");
        return set_child(iter, Index, other);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator set_left_child(iterator iter, iterator other)
    {
        return set_child<0>(iter, other);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator set_right_child(iterator iter, iterator other)
    {
        return set_child<1>(iter, other);
    }

    void clear()
    {
        delete _root;
        _root = nullptr;
    }

    bool erase(iterator iter)
    {
        if (!iter)
        {
            return false;
        }

        auto data = iter._data;

        if (data && data->root() == _root)
        {
            if (data->parent())
            {
                data->parent()->detach_child(data);
            }

            if (data == _root)
            {
                _root = nullptr;
            }

            delete data;

            return true;
        }

        return false;
    }

    iterator begin() const
    {
        return iterator { _root };
    }

    iterator end() const
    {
        return iterator {};
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator rotate(iterator pivot, size_t dir)
    {        
        auto p = pivot._data;

        if (!p)
        {
            return {};
        }

        auto q = p->parent();

        if (!q)
        {
            return {};
        }

        auto p_index = p->index();

        if (p_index != dir)
        {
            return {};
        }

        auto q_parent = q->parent();
        auto q_index = q->index();

        auto b = p->child_at(1 - dir);

        if (b)
        {
            q->move_child(dir, b);
        }

        p->move_child(1 - dir, q);

        if (q_parent)
        {
            q_parent->move_child(q_index, p);
            return q_parent;
        }
       
        p->detach();
        _root = p;
        return _root;
    }

    template <size_t C, size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator rotate(iterator pivot)
    {
        static_assert(C < 2, "invalid rotation direction");
        return rotate(pivot, C);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator rotate_left(iterator pivot)
    {
        return rotate<1>(pivot);
    }

    template <size_t Dim = D, CONCEPT = is_binary_tree<Dim>>
    iterator rotate_right(iterator pivot)
    {
        return rotate<0>(pivot);
    }

private:
    iterator set_root(data_type* new_data)
    {
        _root = new_data;
        return root();
    }

    iterator set_child(iterator iter, size_t index, data_type* new_data)
    {
        iter._data->set_child(index, new_data);

        return iterator { new_data };
    }

    data_type* _root;
};

template <class T>
using binary_tree = tree<T, 2>;

template <class T>
using quadtree = tree<T, 4>;

template <class T>
using octree = tree<T, 8>;

} /* namespace cpp_essentials::graphs */

#endif /* CPP_ESSENTIALS_GRAPHS_BINARY_TREE_HPP_ */
