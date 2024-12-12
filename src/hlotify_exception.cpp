#include "hlotify_exception.h"

const char* HlotifyException::what() const noexcept {
    return "Hlotify exception occured";
}

const char* HlConfigException::what() const noexcept {
    return "Config exception occured";
}

const char* HlConfigSaveException::what() const noexcept {
    return "Config saving exception occured";
}

const char* HlConfigLoadException::what() const noexcept {
    return "Config loading exception occured";
}

const char* HlConfigCreateException::what() const noexcept {
    return "Config file creation exception occured";
}
