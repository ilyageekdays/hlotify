#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <cstdlib>

#include "hlotify_logger.h"
#include "SimpleIni.h"
#include "hlotify_exception.h"

#include "config.h"

HlotifyConfig::HlotifyConfig() {
    Logger::getInstance().info("Initializing HlotifyConfig...");

    if (configPath.empty()) {
        configPath = HlotifyConfig::findConfig();
    }
    std::string globalPath;
    if (configPath.empty()) {
        globalPath = HlotifyConfig::globalConfigPath();
        Logger::getInstance().warning("No config file found. Attempting to create global config at: " + globalPath);

        if (geteuid() != 0) {
            Logger::getInstance().error("Permission Denied: This operation requires sudo/root access.");
            exit(EACCES);
        }

        createConfigFile(globalPath);
        configPath = globalPath;
        Logger::getInstance().info("Global config created successfully.");
    }

    loadConfigFile();
    Logger::getInstance().info("HlotifyConfig initialized with config path: " + configPath);
}

std::string HlotifyConfig::findConfig() {
    Logger::getInstance().info("Searching for configuration file...");
    const char* envConfigPath = std::getenv("XDG_CONFIG_HOME");
    std::string configPath;
    if (envConfigPath) {
        configPath = std::string(envConfigPath) + "/hlotify/hlotify.rc";
    }
    if (HlotifyConfig::isExist(configPath)) {
        Logger::getInstance().info("Config found at: " + configPath);
        return configPath;
    }

    envConfigPath = std::getenv("HOME");
    if (!envConfigPath) {
        struct passwd pwd;
        struct passwd *result = nullptr;
        std::string buffer(1024, '\0');

        if (getpwuid_r(getuid(), &pwd, &buffer[0], buffer.size(), &result) == 0) {
            if (result != nullptr) {
                envConfigPath = pwd.pw_dir;
            }
        } else {
            configError("Unable to get user information");
        }
    }

    configPath = std::string(envConfigPath) + "/.config/hlotify/hlotify.rc";
    if (HlotifyConfig::isExist(configPath)) {
        Logger::getInstance().info("Config found at: " + configPath);
        return configPath;
    }

    envConfigPath = std::getenv("XDG_CONFIG_DIRS");
    if (envConfigPath) {
        configPath = std::string(envConfigPath) + "/hlotify/hlotify.rc";
    } else {
        configPath = "/etc/xdg/hlotify/hlotify.rc";
    }

    if (HlotifyConfig::isExist(configPath)) {
        Logger::getInstance().info("Config found at: " + configPath);
        return configPath;
    }

    Logger::getInstance().warning("No configuration file found.");
    return "";
}

void HlotifyConfig::loadConfigFile() {
    Logger::getInstance().info("Loading configuration file from: " + configPath);
    rc.SetUnicode();
    rcErr = rc.LoadFile(configPath.c_str());
    if (rcErr != SI_OK) {
        Logger::getInstance().error("Failed to load configuration file.");
        throw HlConfigLoadException();
    }
    Logger::getInstance().info("Configuration file loaded successfully.");
}

void HlotifyConfig::saveConfigFile() {
    Logger::getInstance().info("Saving configuration file to: " + configPath);
    rcErr = rc.SaveFile(configPath.c_str());
    if (rcErr != SI_OK) {
        Logger::getInstance().error("Failed to save configuration file.");
        throw HlConfigSaveException();
    }
    Logger::getInstance().info("Configuration file saved successfully.");
}

bool HlotifyConfig::isExist(const std::string& path) {
    Logger::getInstance().debug("Checking existence of: " + path);
    return access(path.c_str(), F_OK) != -1;
}

std::string HlotifyConfig::globalConfigPath() {
    const char* envConfigPath = std::getenv("XDG_CONFIG_DIRS");
    std::string configPath;
    if (envConfigPath) {
        configPath = std::string(envConfigPath) + "/hlotify/hlotify.rc";
    } else {
        configPath = "/etc/xdg/hlotify/hlotify.rc";
    }

    Logger::getInstance().debug("Global config path resolved to: " + configPath);
    return configPath;
}

void HlotifyConfig::createDirs(const std::string& path) {
    Logger::getInstance().info("Creating directories for path: " + path);
    std::string dir = "";
    for (const char &ch : path) {
        dir += ch;
        if ((ch == '/' || ch == '\\') &&
            (access(dir.c_str(), F_OK) != 0 &&
            (mkdir(dir.c_str(), S_IRWXU | S_IRWXG) != 0 && errno != EEXIST))) {

            Logger::getInstance().error("Failed to create directory: " + dir);
            configErrorErrno("Unable to create directory");
        }
    }
    Logger::getInstance().info("Directories created successfully.");
}

void HlotifyConfig::createConfigFile(const std::string& path) {
    Logger::getInstance().info("Creating configuration file at: " + path);
    std::string::size_type pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        std::string dirPath = path.substr(0, pos+1);
        HlotifyConfig::createDirs(dirPath);
        int fd = open(path.c_str(), O_WRONLY | O_CREAT, S_IRWXG);
        if (fd == -1) {
            Logger::getInstance().error("Failed to create configuration file at: " + path);
            throw HlConfigCreateException();
        }
        close(fd);
    }
    Logger::getInstance().info("Configuration file created successfully.");
}

std::string HlotifyConfig::getValue(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    Logger::getInstance().info("Fetching value for section: " + section + ", key: " + key);
    checkOpened();
    std::string value = rc.GetValue(section.c_str(), key.c_str(), defaultValue.c_str());
    Logger::getInstance().debug("Value fetched: " + value);
    return value;
}

void HlotifyConfig::setValue(const std::string& section, const std::string& key, const std::string& value, const std::string& comment, bool forceReplace) {
    Logger::getInstance().info("Setting value for section: " + section + ", key: " + key);
    checkOpened();
    rc.SetValue(section.c_str(), key.c_str(), value.c_str(), comment.c_str(), forceReplace);
    saveConfigFile();
    Logger::getInstance().info("Value set successfully.");
}

void HlotifyConfig::deleteSection(const std::string& section) {
    Logger::getInstance().info("Deleting section: " + section);
    checkOpened();
    rc.Delete(section.c_str(), nullptr);
    saveConfigFile();
    Logger::getInstance().info("Section deleted successfully.");
}

void HlotifyConfig::deleteKey(const std::string& section, const std::string& key) {
    Logger::getInstance().info("Deleting key: " + key + " from section: " + section);
    checkOpened();
    rc.Delete(section.c_str(), key.c_str(), NULL);
    saveConfigFile();
    Logger::getInstance().info("Key deleted successfully.");
}

std::vector<std::string> HlotifyConfig::getSections() const {
    Logger::getInstance().info("Fetching all sections...");
    checkOpened();
    CSimpleIniA::TNamesDepend sections;
    rc.GetAllSections(sections);

    std::vector<std::string> sectionVector;
    for (const auto& section : sections) {
        sectionVector.emplace_back(section.pItem);
    }
    Logger::getInstance().info("Sections fetched successfully.");
    return sectionVector;
}

std::string HlotifyConfig::treeView() const {
    Logger::getInstance().info("Generating tree view of configuration...");
    checkOpened();

    std::string resultString = configPath + "\n\n";
    CSimpleIniA::TNamesDepend sections;
    std::ostringstream resultStream;
    rc.GetAllSections(sections);

    for (const auto& section : sections) {
        CSimpleIniA::TNamesDepend keys;
        rc.GetAllKeys(section.pItem, keys);
        resultStream << "\u2022 " << section.pItem << "\n";
        for (const auto& key : keys) {
            resultStream << "\t\"" << key.pItem << "\": \"" << rc.GetValue(section.pItem, key.pItem) << "\"\n";
        }
    }

    resultString = resultStream.str();
    Logger::getInstance().info("Tree view generated successfully.");
    return resultString;
}

void HlotifyConfig::checkOpened() const {
    if (rcErr != SI_OK) {
        Logger::getInstance().error("Configuration file is not opened.");
        configError("Configuration file is not opened");
    }
}

void HlotifyConfig::configError(const std::string& msg) {
    Logger::getInstance().error("Config Error: " + msg);
    std::cerr << "(!) Config Error: " << msg << "\n";
    exit(errno);
}

void HlotifyConfig::configErrorErrno(const std::string& msg) {
    Logger::getInstance().error("Config Error: " + msg + ": " + strerror(errno));
    std::cerr << "(!) Config Error: " << msg << ": " << strerror(errno) << "\n";
    exit(errno);
}
