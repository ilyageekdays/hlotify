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
    : title(title), message(message), duration(duration) {
        
}

void Notification::startTimer() {
    auto self = shared_from_this();
    future = std::async(std::launch::async, [self]() {
        std::lock_guard lock(*self->mutex);
        std::this_thread::sleep_for(std::chrono::seconds(self->duration));
        self->expired = true;
        std::cout << "\nNotification expired: " << self->title << std::endl ;
    });
}

explicit(false) Notification::operator bool() const {
    std::shared_lock lock(*mutex);
    return !expired;
}

bool Notification::operator!() const {
    std::shared_lock lock(*mutex);
    return !(*this);
}

bool operator==(const Notification& ln, const Notification& rn) {
    std::scoped_lock lock {*ln.mutex, *rn.mutex};
    return (ln.title == rn.title && ln.message == rn.message && ln.duration == rn.duration);
}

std::ostream& operator<<(std::ostream& os, const Notification& n) {
    std::shared_lock lock(*n.mutex);
    os << n.title << ": \"" << n.message << "\", " << n.duration << ", "  << n.expired << "\n";
    return os;
}

void Notification::setTitle(std::string&& t) {
    std::shared_lock lock(*mutex);
    title = std::move(t);
}

void Notification::setMessage(std::string&& m) {
    std::shared_lock lock(*mutex);
    message = std::move(m);
}

std::string Notification::getTitle() const {
    std::shared_lock lock(*mutex);
    return title;
}

std::string Notification::getMessage() const {
    std::shared_lock lock(*mutex);
    return message;
}

int Notification::getDuration() const {
    std::shared_lock lock(*mutex);
    return duration;
}

bool Notification::isExpired() const {
    std::shared_lock lock(*mutex);
    return bool(*this);
}

void Notification::display() const {
    std::shared_lock lock(*mutex);
    std::cout << "Title: " << title << "\nMessage: " << message << "\nDuration: " << duration << " seconds\n";
}
