#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <future>

class Notification: public std::enable_shared_from_this<Notification> {
public:
    Notification(const std::string&, const std::string&, int = 0);
    Notification& operator=(Notification&&) noexcept;
    
    operator bool() const;
    bool operator!() const;     
    bool operator==(const Notification&) const;
    bool operator!=(const Notification&) const;
    friend std::ostream& operator<<(std::ostream&, const Notification&);

    void startTimer();
    void join();

    void setTitle(std::string&&);
    void setMessage(std::string&&);

    std::string getTitle() const;
    std::string getMessage() const;
    int getDuration() const;
    bool isExpired() const;
    void display() const;

private:
    std::string title;   
    std::string message;
    std::future<void> future;
    int duration;
    bool expired;
    std::unique_ptr<std::shared_mutex> mutex;
};

#endif // NOTIFICATION_H