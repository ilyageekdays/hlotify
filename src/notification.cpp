#include "notification.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <memory>
#include <shared_mutex>

Notification::Notification(const std::string &title, const std::string &message, int duration)
    : title(title), message(message), duration(duration), expired(false), mutex(std::make_unique<std::shared_mutex>()) {
        
}

void Notification::startTimer() {
    auto self = shared_from_this();
    future = std::async(std::launch::async, [self]() {
        std::lock_guard<std::shared_mutex> lock(*self->mutex);
        std::this_thread::sleep_for(std::chrono::seconds(self->duration));
        self->expired = true;
        std::cout << "\nNotification expired: " << self->title << std::endl ;
    });
}

Notification& Notification::operator=(Notification&& other) noexcept {
    if (this != &other) {
        std::lock_guard<std::shared_mutex> lock(*mutex);
        std::lock_guard<std::shared_mutex> lock_other(*other.mutex);
        title = std::move(other.title);
        message = std::move(other.message);
        future = std::move(other.future);
        duration = other.duration;
        expired = other.expired;
        other.duration = 0;
        other.expired = true;
    }
    return *this;
}

Notification::operator bool() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    return !expired;
}

bool Notification::operator!() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    return !(*this);
}

bool Notification::operator==(const Notification& other) const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    std::shared_lock<std::shared_mutex> other_lock(*other.mutex);
    return (title == other.title && message == other.message && duration == other.duration);
}

bool Notification::operator!=(const Notification& other) const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    std::shared_lock<std::shared_mutex> other_lock(*other.mutex);
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Notification& n) {
    std::shared_lock<std::shared_mutex> lock(*n.mutex);
    os << n.title << ": \"" << n.message << "\", " << n.duration << ", "  << n.expired << "\n";
    return os;
}

void Notification::setTitle(std::string&& t) {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    title = std::move(t);
}

void Notification::setMessage(std::string&& m) {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    message = std::move(m);
}

std::string Notification::getTitle() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    return title;
}

std::string Notification::getMessage() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    return message;
}

int Notification::getDuration() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    return duration;
}

bool Notification::isExpired() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    return bool(*this);
}

void Notification::display() const {
    std::shared_lock<std::shared_mutex> lock(*mutex);
    std::cout << "Title: " << title << "\nMessage: " << message << "\nDuration: " << duration << " seconds\n";
}
