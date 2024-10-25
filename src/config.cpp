#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <cstdlib>

#include "SimpleIni.h"

#include "config.h"

HlotifyConfig::HlotifyConfig() {
    if (configPath.empty()) {
        configPath = HlotifyConfig::findConfig();
    }
    std::string globalPath;
    if (configPath.empty()) {
        globalPath = HlotifyConfig::globalConfigPath();
        std::cerr << "(!) Config Error: No config file founded!\n\tTrying to create new global config in " << globalPath << "\n";
        if (geteuid() != 0) {
            std::cerr << "(!) Permission Denied: This operation requires sudo/root access.\n\t" \
                         "Please run as root or create config file manualy.\n";
            exit(EACCES);
        }
        createConfigFile(globalPath);
        configPath = globalPath;
        std::cout << "(!) Info: Global config created successfully!\n";
    }
	loadConfigFile();
}

std::string HlotifyConfig::findConfig() {
    const char* envConfigPath = std::getenv("XDG_CONFIG_HOME");
    std::string configPath;
    if (envConfigPath) {
        configPath = std::string(envConfigPath) + "/hlotify/hlotify.rc";
    }
    if (HlotifyConfig::isExist(configPath)) {
        return configPath;
    }

    envConfigPath = std::getenv("HOME");
    if (!envConfigPath) {
        envConfigPath = getpwuid(getuid())->pw_dir;
    }
    configPath = std::string(envConfigPath) + "/.config/hlotify/hlotify.rc";
    if (HlotifyConfig::isExist(configPath)) {
        return configPath;
    }

    envConfigPath = std::getenv("XDG_CONFIG_DIRS");
    if (envConfigPath) {
        configPath = std::string(envConfigPath) + "/hlotify/hlotify.rc";
    } else configPath = "/etc/xdg/hlotify/hlotify.rc";
    if (HlotifyConfig::isExist(configPath)) {
        return configPath;
    }

    return "";
}

void HlotifyConfig::loadConfigFile() {
    rc.SetUnicode();
    rcErr = rc.LoadFile(configPath.c_str());
    if (rcErr != SI_OK) {
        configErrorErrno("Unable to load config file");
        exit(EIO);
    }
}

void HlotifyConfig::saveConfigFile() {
    SI_Error rcErr = rc.SaveFile(configPath.c_str());
    if (rcErr != SI_OK) {
        configErrorErrno("Unable to save config file");
        exit(EIO);
    }
}

bool HlotifyConfig::isExist(const std::string& path) {
    return access(path.c_str(), F_OK) != -1;
}

std::string HlotifyConfig::globalConfigPath() {
    const char* envConfigPath = std::getenv("XDG_CONFIG_DIRS");
    std::string configPath;
    if (envConfigPath) {
        configPath = std::string(envConfigPath) + "/hlotify/hlotify.rc";
    } else configPath = "/etc/xdg/hlotify/hlotify.rc";
    
    return configPath;
}

void HlotifyConfig::createDirs(const std::string& path) {
    std::string dir = "";
    for (const char &ch : path) {
        dir += ch;
        if (ch == '/' || ch == '\\') {
            if (access(dir.c_str(), F_OK) != 0) {
                if (mkdir(dir.c_str(), 0755) != 0 && errno != EEXIST) {
                    configErrorErrno("Unable create directory");
                }
            }
        }
    }
}

void HlotifyConfig::createConfigFile(const std::string& path) {
    std::string::size_type pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        std::string dirPath = path.substr(0, pos+1);
        HlotifyConfig::createDirs(dirPath);
        int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
        if (fd == -1) {
            configErrorErrno("Unable to create config file");
            exit(EACCES);
        }
    }
}

std::string HlotifyConfig::getValue(const std::string& section, const std::string& key, const std::string& defaultValue) {
    checkOpened();
    return std::string(rc.GetValue(section.c_str(), key.c_str(), defaultValue.c_str()));
}

void HlotifyConfig::setValue(const std::string& section, const std::string& key, const std::string& value, const std::string& comment, bool forceReplace) {
    checkOpened();
    rc.SetValue(section.c_str(), key.c_str(), value.c_str(), comment.c_str(), forceReplace);
    saveConfigFile();
}

void HlotifyConfig::deleteSection(const std::string& section) {
    checkOpened();
    rc.Delete(section.c_str(), NULL);
    saveConfigFile();
}

void HlotifyConfig::deleteKey(const std::string& section, const std::string& key) {
    checkOpened();
    rc.Delete(section.c_str(), key.c_str(), NULL);
    saveConfigFile();
}

std::vector<std::string> HlotifyConfig::getSections() {
    checkOpened();
    CSimpleIniA::TNamesDepend sections;
    rc.GetAllSections(sections);

    std::vector<std::string> sectionVector;
    for (CSimpleIniA::TNamesDepend::iterator it = sections.begin(); it != sections.end(); ++it) {
        sectionVector.push_back(it->pItem);
    }
    return sectionVector;
}

std::string HlotifyConfig::treeView() {
    checkOpened();

    std::string resultString = configPath + "\n\n";
    CSimpleIniA::TNamesDepend sections;
    rc.GetAllSections(sections);

    for (CSimpleIniA::TNamesDepend::iterator it = sections.begin(); it != sections.end(); ++it) {
        CSimpleIniA::TNamesDepend keys;
        rc.GetAllKeys(it->pItem, keys);

        resultString += std::string("• ") + it->pItem + "\n";
        for (CSimpleIniA::TNamesDepend::iterator kIt = keys.begin(); kIt != keys.end(); ++kIt) {
            resultString += std::string("\t\"") + kIt->pItem + "\": \"" + rc.GetValue(it->pItem, kIt->pItem)+ "\"" + "\n";
        }
    }
    return resultString;
}

void HlotifyConfig::checkOpened() {
    if (rcErr != SI_OK) {
        configError("Config file is not opened");
    }
}

void HlotifyConfig::configError(const std::string& msg) {
    std::cerr << "(!) Config Error: " << msg << "\n";
    exit(errno);
}

void HlotifyConfig::configErrorErrno(const std::string& msg) {
    std::cerr << "(!) Config Error: " << msg << ": " << strerror(errno) << "\n";
    exit(errno);
}