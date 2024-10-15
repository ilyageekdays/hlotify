#include "notification.h"
#include <iostream>
#include <thread>
#include <chrono>

Notification::Notification(const std::string &title, const std::string &message, int duration)
    : title(title), message(message), duration(duration) {
    if (duration > 0) {
        thread = std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(this->duration));
            std::cout << "Notification expired: " << this->title << std::endl;
        });
    }
}

Notification::Notification(Notification&& other) noexcept
    : title(std::move(other.title)),
      message(std::move(other.message)),
      duration(other.duration),
      thread(std::move(other.thread)) { // Перемещение потока
    other.duration = 0; // Обнуляем duration у перемещаемого объекта
}

void Notification::join() {
    if (thread.joinable()) {
        thread.join();
    }
}

Notification::~Notification() {
    join();
}

Notification& Notification::operator=(Notification&& other) noexcept {
    if (this != &other) {
        title = std::move(other.title);
        message = std::move(other.message);
        duration = other.duration;

        // Перемещение потока
        if (thread.joinable()) {
            thread.join(); // Ждем завершения текущего потока, если он был запущен
        }
        thread = std::move(other.thread); // Перемещаем новый поток

        other.duration = 0; // Обнуляем duration у перемещаемого объекта
    }
    return *this;
}

std::string Notification::getTitle() const {
    return title;
}

std::string Notification::getMessage() const {
    return message;
}

int Notification::getDuration() const {
    return duration;
}

void Notification::display() const {
    std::cout << "Title: " << title << "\nMessage: " << message << "\nDuration: " << duration << " seconds" << std::endl;
}
