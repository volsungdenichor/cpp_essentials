#ifndef CPP_ESSENTIALS_GRAPHS_TREE_HPP_
#define CPP_ESSENTIALS_GRAPHS_TREE_HPP_

#pragma once

#include <queue>

#include <cpp_essentials/graphs/binary_tree.hpp>

namespace cpp_essentials::graphs
{

namespace detail
{

template <class T>
class tree_node_struct<T, 0>
{
public:
    tree_node_struct(const T& data)
        : _data(data)
        , _parent { nullptr }
        , _next_sibling { nullptr }
        , _first_child { nullptr }
    {
    }

    tree_node_struct()
        : tree_node_struct { T {} }
    {
    }

    ~tree_node_struct()
    {
        delete _next_sibling;
        delete _first_child;
    }

    tree_node_struct* clone() const
    {
        auto result = std::make_unique<tree_node_struct>(data());

        for (auto i = first_child(); i; i = i->next_sibling())
        {
            result->append_child(i->clone());
        }

        return result.release();
    }

    const T& data() const
    {
        return _data;
    }

    T& data()
    {
        return _data;
    }

    tree_node_struct* parent() const
    {
        return _parent;
    }

    tree_node_struct* root() const
    {
        return parent() ? parent()->root() : const_cast<tree_node_struct*>(this);
    }

    tree_node_struct* next_sibling() const
    {
        return _next_sibling;
    }

    tree_node_struct* prev_sibling() const
    {
        auto result = first_sibling_or_self();

        while (result && result->next_sibling() != this)
        {
            result = result->next_sibling();
        }

        return result;
    }

    size_t children_count() const
    {
        size_t result = 0;

        for (auto i = first_child(); i; i = i->next_sibling())
        {
            ++result;
        }

        return result;
    }

    size_t descendants_count() const
    {
        size_t result = 0;

        for (auto i = first_child(); i; i = i->next_sibling())
        {
            result += i->descendants_count() + 1;
        }

        return result;
    }

    size_t siblings_count() const
    {
        return parent() ? parent()->children_count() - 1 : 0;
    }

    tree_node_struct* first_child() const
    {
        return _first_child;
    }

    tree_node_struct* last_child() const
    {
        auto result = first_child();

        while (result && result->next_sibling())
        {
            result = result->next_sibling();
        }

        return result;
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

    void append_child(tree_node_struct* child)
    {
        EXPECTS(child != nullptr);
        EXPECTS(child->parent() == nullptr);

        child->_parent = this;

        auto last = last_child();

        if (last)
        {
            last->_next_sibling = child;
        }
        else
        {
            _first_child = child;
        }
    }

    void prepend_child(tree_node_struct* child)
    {
        EXPECTS(child != nullptr);
        EXPECTS(child->parent() == nullptr);

        child->_parent = this;

        child->_next_sibling = first_child();
        _first_child = child;
    }

    void insert_child(tree_node_struct* child, tree_node_struct* after)
    {
        EXPECTS(child != nullptr);
        EXPECTS(after != nullptr);

        EXPECTS(child->parent() == nullptr);
        EXPECTS(after->parent() == this);

        child->_parent = this;

        child->_next_sibling = after->next_sibling();
        after->_next_sibling = child;
    }

    void insert_sibling_after(tree_node_struct* child, tree_node_struct* after)
    {
        EXPECTS(child != nullptr);
        EXPECTS(after != nullptr);

        EXPECTS(child->parent() == nullptr);
        EXPECTS(after->parent() != nullptr);

        child->_parent = parent();

        child->_next_sibling = next_sibling();
        _next_sibling = child;
    }

    void insert_sibling_before(tree_node_struct* child, tree_node_struct* before)
    {
        EXPECTS(child != nullptr);
        EXPECTS(before != nullptr);

        EXPECTS(child->parent() == nullptr);
        EXPECTS(before->parent() != nullptr);

        child->_parent = parent();

        auto prev = prev_sibling();

        if (prev)
        {
            prev->_next_sibling = child;
        }

        child->_next_sibling = this;
    }

    void detach_child(tree_node_struct* child)
    {
        EXPECTS(child != nullptr);
        EXPECTS(child->parent() == this);

        auto prev = child->prev_sibling();

        if (prev)
        {
            prev->_next_sibling = child->next_sibling();
        }

        if (first_child() == child)
        {
            _first_child = child->next_sibling();
        }

        child->_parent = nullptr;
        child->_next_sibling = nullptr;
    }

    tree_node_struct* child(const T& value) const
    {
        for (auto i = first_child(); i; i = i->next_sibling())
        {
            if (i->data() == value)
            {
                return i;
            }
        }

        return nullptr;
    }

    tree_node_struct* child_at(size_t index) const
    {
        for (auto i = first_child(); i; i = i->next_sibling())
        {
            if (index == 0)
            {
                return i;
            }

            --index;
        }

        return nullptr;
    }

    tree_node_struct* sibling(const T& value) const
    {
        for (auto i = first_sibling_or_self(); i; i = i->next_sibling())
        {
            if (i != this)
            {
                if (i->data() == value)
                {
                    return i;
                }
            }
        }

        return nullptr;
    }

    tree_node_struct* sibling_at(size_t index) const
    {
        for (auto i = first_sibling_or_self(); i; i = i->next_sibling())
        {
            if (i != this)
            {
                if (index == 0)
                {
                    return i;
                }

                --index;
            }
        }

        return nullptr;
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

    tree_node_struct* ancestor(const T& value) const
    {
        for (auto i = parent(); i; i = i->parent())
        {
            if (i->data() == value)
            {
                return i;
            }
        }

        return nullptr;
    }

    size_t index() const
    {
        size_t result = 0;

        for (auto i = first_sibling_or_self(); i != this; i = i->next_sibling())
        {
            ++result;
        }

        return result;
    }

    size_t depth() const
    {
        return parent() ? parent()->depth() + 1 : 0;
    }

    size_t max_depth() const
    {
        size_t result = 0;

        for (auto i = first_child(); i; i = i->next_sibling())
        {
            size_t child_depth = i->max_depth();

            if (child_depth > result)
            {
                result = child_depth;
            }
        }

        return result + 1;
    }

private:
    tree_node_struct* first_sibling_or_self() const
    {
        return parent() ? parent()->first_child() : nullptr;
    }

    T _data;
    tree_node_struct* _parent;
    tree_node_struct* _next_sibling;
    tree_node_struct* _first_child;
};



template <class T>
struct tree_iterator_impl<tree_traversal::children, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;

    node_struct* init(node_struct* node)
    {
        return node->first_child();
    }

    node_struct* next(node_struct* node)
    {
        return node->next_sibling();
    }
};



template <class T>
struct tree_iterator_impl<tree_traversal::siblings, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;

    node_struct* init(node_struct* node)
    {
        _self = node;

        auto parent = node->parent();

        return parent ? advance(parent->first_child()) : nullptr;
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



template <class T>
struct tree_iterator_impl<tree_traversal::upward, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;

    node_struct* init(node_struct* node)
    {
        return node;
    }

    node_struct* next(node_struct* node)
    {
        return node->parent();
    }
};



template <class T>
struct tree_iterator_impl<tree_traversal::preorder, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;
    node_struct* _root;

    node_struct* init(node_struct* node)
    {
        _root = node;
        return node;
    }

    node_struct* next(node_struct* node)
    {
        auto first_child = node->first_child();

        if (first_child)
        {
            return first_child;
        }

        for (auto i = node; i && i != _root; i = i->parent())
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



template <class T>
struct tree_iterator_impl<tree_traversal::postorder, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;

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
struct tree_iterator_impl<tree_traversal::levelorder, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;

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



template <class T>
struct tree_iterator_impl<tree_traversal::leaves, T, 0>
{
    using node_struct = tree_node_struct<T, 0>;

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



template <class T, tree_traversal Traversal>
class tree_iterator<T, 0, Traversal> : public core::iterator_facade<tree_iterator<T, 0, Traversal>, std::forward_iterator_tag, T>
{
public:
    using data_type = tree_node_struct<T, 0>;
    using base_type = core::iterator_facade<tree_iterator<T, 0, Traversal>, std::forward_iterator_tag, T>;

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
    tree_iterator(const tree_iterator<T, 0, OtherTraversal>& other)
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

    tree_iterator next_sibling() const
    {
        return { _data ? _data->next_sibling() : nullptr };
    }

    tree_iterator prev_sibling() const
    {
        return { _data ? _data->prev_sibling() : nullptr };
    }

    tree_iterator sibling(size_t index) const
    {
        return { _data ? _data->sibling_at(index) : nullptr };
    }

    tree_iterator first_child() const
    {
        return { _data ? _data->first_child() : nullptr };
    }

    tree_iterator last_child() const
    {
        return { _data ? _data->last_child() : nullptr };
    }

    tree_iterator child(size_t index) const
    {
        return { _data ? _data->child_at(index) : nullptr };
    }

    bool is_leaf() const
    {
        return _data && _data->first_child() == nullptr;
    }

    template <tree_traversal TraversalType>
    core::iterator_range<tree_iterator<T, 0, TraversalType>> traverse() const
    {
        tree_iterator<T, 0, TraversalType> begin { _data };
        tree_iterator<T, 0, TraversalType> end {};

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
    tree_iterator_impl<Traversal, T, 0> _impl;
};

} /* namespace detail */

template <class T>
class tree<T, 0>
{
private:
    using data_type = detail::tree_node_struct<T, 0>;

public:
    using value_type = T;

    using children_iterator = detail::children_iterator<T, 0>;
    using siblings_iterator = detail::siblings_iterator<T, 0>;
    using preorder_iterator = detail::preorder_iterator<T, 0>;
    using postorder_iterator = detail::postorder_iterator<T, 0>;
    using levelorder_iterator = detail::levelorder_iterator<T, 0>;
    using leaves_iterator = detail::leaves_iterator<T, 0>;
    using upward_iterator = detail::upward_iterator<T, 0>;

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
        swap(other);
    }

    template <tree_traversal Traversal>
    tree(detail::tree_iterator<T, 0, Traversal> iter)
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

    operator iterator() const
    {
        return begin();
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
        return iterator { _root.get() };
    }

    iterator root(const value_type& data)
    {
        clear();

        return set_root(std::make_unique<data_type>(data));
    }

    iterator root(iterator iter)
    {
        EXPECTS(!!iter);

        clear();

        return iter
            ? set_root(iter._data->clone())
            : iterator {};
    }

    iterator insert(iterator iter, const value_type& data)
    {
        if (!iter)
        {
            if (empty())
            {
                return set_root(std::make_unique<data_type>(data));
            }
            else
            {
                throw std::runtime_error { "empty iterator" };
            }
        }

        return insert_sibling_after(iter, std::make_unique<data_type>(data).release());
    }

    iterator insert_before(iterator iter, const value_type& data)
    {
        return insert_sibling_before(std::make_unique<data_type>(data).release());
    }

    iterator append_child(iterator iter, const value_type& data)
    {
        if (!iter)
        {
            if (empty())
            {
                return set_root(std::make_unique<data_type>(data));
            }
            else
            {
                throw std::runtime_error{ "empty iterator" };
            }
        }

        return append_child(iter, std::make_unique<data_type>(data).release());
    }

    iterator prepend_child(iterator iter, const value_type& data)
    {
        return prepend_child(iter, std::make_unique<data_type>(data).release());
    }

    iterator insert(iterator iter, iterator other)
    {
        EXPECTS(!!other);

        if (!iter)
        {
            if (empty())
            {
                return set_root(other._data->clone());
            }
            else
            {
                throw std::runtime_error { "empty iterator" };
            }
        }

        return insert_sibling_after(iter, other._data->clone());
    }

    iterator insert_before(iterator iter, iterator other)
    {
        EXPECTS(!!other);

        return insert_sibling_before(iter, other._data->clone());
    }

    iterator append_child(iterator iter, iterator other)
    {
        EXPECTS(!!other);

        return append_child(iter, other._data->clone());
    }

    iterator prepend_child(iterator iter, iterator other)
    {
        EXPECTS(!!other);

        return prepend_child(iter, other._data->clone());
    }

    void clear()
    {
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
        return iterator { _root.get() };
    }

    iterator end() const
    {
        return iterator {};
    }

private:
    iterator set_root(std::unique_ptr<data_type> new_data)
    {
        _root = std::move(new_data);
        return root();
    }

    iterator insert_sibling_after(iterator iter, data_type* new_data)
    {
        iter._data->insert_sibling_after(new_data, iter._data);

        return iterator { new_data };
    }

    iterator insert_sibling_before(iterator iter, data_type* new_data)
    {
        iter._data->insert_sibling_before(new_data, iter._data);

        return iterator { new_data };
    }

    iterator append_child(iterator iter, data_type* new_data)
    {
        iter._data->append_child(new_data);

        return iterator { new_data };
    }

    iterator prepend_child(iterator iter, data_type* new_data)
    {
        iter._data->prepend_child(new_data);

        return iterator { new_data };
    }

    std::unique_ptr<data_type> _root;
};

template <class T, class... Children>
tree<T> node(T value, Children... children)
{
    tree<T> result;
    auto it = result.root(std::move(value));

    const auto insert = [&](auto& child)
    {
        result.append_child(it, child.root());
    };

    (..., insert(children));

    return result;
}

} /* namespace cpp_essentials::graphs */

#endif /* CPP_ESSENTIALS_GRAPHS_TREE_HPP_ */

