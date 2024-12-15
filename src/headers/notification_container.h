#ifndef NOTIFICATIONCONTAINER_H
#define NOTIFICATIONCONTAINER_H

#include <vector>
#include <memory>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>

template <typename T>
class NotificationContainer {
private:
    std::vector<std::shared_ptr<T>> elements;

public:
    class iterator {
    public:
        typedef iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        iterator(pointer ptr) : ptr_(ptr) {}

        self_type operator++() {
            ptr_++;
            return *this;
        }

        self_type operator++(int) {
            self_type tmp = *this;
            ++(*this);
            return tmp;
        }

        reference operator*() { return *ptr_; }
        pointer operator->() { return ptr_; }

        bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

    private:
        pointer ptr_;
    };

    iterator begin() { return iterator(elements.data()); }
    iterator end() { return iterator(elements.data() + elements.size()); }

    void add(const std::shared_ptr<T>& element) {
        elements.push_back(element);
    }

    bool remove(size_t index) {
        if (index < elements.size()) {
            elements.erase(elements.begin() + index);
            return true;
        }
        return false;
    }

    std::shared_ptr<T> get(size_t index) const {
        if (index < elements.size()) {
            return elements[index];
        }
        return nullptr;
    }

    size_t size() const {
        return elements.size();
    }

    void clear() {
        elements.clear();
    }
};


#endif