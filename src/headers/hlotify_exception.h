#ifndef HLOTIFYEXCEPTION_H
#define HLOTIFYEXCEPTION_H

#include <exception>

class HlotifyException : public std::exception {
public:
    HlotifyException() = default;
    virtual ~HlotifyException() noexcept = default;

    const char* what() const noexcept override;
};

class HlConfigException : public HlotifyException {
public:
    HlConfigException() = default;
    virtual ~HlConfigException() noexcept = default;

    const char* what() const noexcept override;
};

class HlConfigLoadException : public HlConfigException {
public:
    HlConfigLoadException() = default;
    virtual ~HlConfigLoadException() noexcept = default;

    const char* what() const noexcept override;
};

class HlConfigSaveException : public HlConfigException {
public:
    HlConfigSaveException() = default;
    virtual ~HlConfigSaveException() noexcept = default;

    const char* what() const noexcept override;
};

class HlConfigCreateException : public HlConfigException {
public:
    HlConfigCreateException() = default;
    virtual ~HlConfigCreateException() noexcept = default;

    const char* what() const noexcept override;
};

#endif //HLOTIFYEXCEPTION_H