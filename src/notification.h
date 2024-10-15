#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <thread>

class Notification {
public:
    Notification(const std::string &title, const std::string &message, int duration = 0);
    ~Notification();
    Notification(Notification&&) noexcept;
    Notification& operator=(Notification&&) noexcept;
    void join();

    std::string getTitle() const;
    std::string getMessage() const;
    int getDuration() const;
    void display() const;

private:
    std::string title;   
    std::string message;
    std::thread thread;
    int duration;
};

#endif // NOTIFICATION_H