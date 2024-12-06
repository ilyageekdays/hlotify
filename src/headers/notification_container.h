#ifndef NOTIFICATIONCONTAINER_H
#define NOTIFICATIONCONTAINER_H

#include <vector>
#include <memory>
#include <functional>

template <typename T>
class NotificationContainer {
private:
    std::vector<std::shared_ptr<T>> elements;

public:    
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

    void forEach(const std::function<void(const std::shared_ptr<T>&)>& action) const {
        for (const auto& element : elements) {
            action(element);
        }
    }

    size_t size() const {
        return elements.size();
    }

    void clear() {
        elements.clear();
    }
};


#endif