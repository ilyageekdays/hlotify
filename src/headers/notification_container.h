#ifndef NOTIFICATIONCONTAINER_H
#define NOTIFICATIONCONTAINER_H

#include <vector>
#include <memory>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>

template <typename T>
class HlIterator {
private:
    std::shared_ptr<T>* ptr_;  // Указатель на std::shared_ptr<T>

public:
    // Конструктор итератора, принимающий указатель на std::shared_ptr<T>
    explicit HlIterator(std::shared_ptr<T>* ptr) : ptr_(ptr) {}

    // Оператор * для доступа к объекту через shared_ptr
    T& operator*() {
        return *ptr_->get();  // Получаем объект, на который указывает shared_ptr
    }

    // Оператор -> для доступа к членам объекта
    T* operator->() {
        return ptr_->get();
    }

    // Оператор ++ для перехода к следующему элементу
    HlIterator& operator++() {
        ++ptr_;
        return *this;
    }

    // Оператор == для сравнения итераторов
    bool operator==(const HlIterator& other) const {
        return ptr_ == other.ptr_;
    }

    // Оператор != для сравнения итераторов
    bool operator!=(const HlIterator& other) const {
        return ptr_ != other.ptr_;
    }
};


template <typename T>
class NotificationContainer {
private:
    std::vector<std::shared_ptr<T>> elements;

public:
    using iterator = HlIterator<T>;

    iterator begin() {
        return iterator(elements.data());
    }

    iterator end() {
        return iterator(elements.data() + elements.size());
    }

    std::shared_ptr<T> back() const {
        return elements.back();
    }

    std::shared_ptr<T>& operator[](size_t index) {
        return elements[index];
    }

    const std::shared_ptr<T>& operator[](size_t index) const {
        return elements[index];
    }

    void add(std::shared_ptr<T> element) {
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