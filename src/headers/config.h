#include <string>
#include <unordered_map>
#include <vector>

#include "SimpleIni.h"

class HlotifyConfig {
public:
    HlotifyConfig();
    void reparseConfig();
    void saveConfig();

    std::string getValue(const std::string&, const std::string&, const std::string& = "") const;
    std::vector<std::string> getSections() const;

    void setValue(const std::string&, const std::string&, const std::string&, const std::string& = "", bool = false);

    void deleteSection(const std::string&);
    void deleteKey(const std::string&, const std::string&);
    
    std::string treeView() const;

private:
    CSimpleIniA rc;
    SI_Error rcErr;
    std::string configPath;

    void checkOpened() const;
    void loadConfigFile();
    void saveConfigFile();
    static void createDirs(const std::string&);
    static void createConfigFile(const std::string&);
    static bool isExist(const std::string&);
    static std::string globalConfigPath();
    static std::string findConfig();

    __attribute__((noreturn)) static void configError(const std::string&);
    __attribute__((noreturn)) static void configErrorErrno(const std::string&);
};