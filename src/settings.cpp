#include "settings.h"

void GetVal(Json::Value &config, int* setting)
{
    if (config.isNull())
        return;

    *setting = config.asInt();
}

void GetVal(Json::Value &config, bool* setting)
{
    if (config.isNull())
        return;

    *setting = config.asBool();
}

void GetVal(Json::Value &config, float* setting)
{
    if (config.isNull())
        return;

    *setting = config.asFloat();
}

/*
void GetVal(Json::Value &config, ImColor* setting)
{
    if (config.isNull())
        return;

    GetVal(config["r"], &setting->Value.x);
    GetVal(config["g"], &setting->Value.y);
    GetVal(config["b"], &setting->Value.z);
    GetVal(config["a"], &setting->Value.w);
}
*/

void GetVal(Json::Value &config, char** setting)
{
    if (config.isNull())
        return;

    *setting = strdup(config.asCString());
}

void GetVal(Json::Value &config, char* setting)
{
    if (config.isNull())
        return;

    strcpy(setting, config.asCString());
}

template <typename Ord, Ord (*lookupFunction)(std::string)>
void GetOrdinal(Json::Value& config, Ord* setting)
{
    if (config.isNull())
        return;

    Ord value;
    if (config.isString())
        value = lookupFunction(config.asString());
    else
        value = (Ord) config.asInt();

    *setting = value;
}

void GetButtonCode(Json::Value &config, enum ButtonCode_t* setting)
{
    GetOrdinal<enum ButtonCode_t, Util::GetButtonCode>(config, setting);
}

/*
void LoadUIColor(Json::Value &config, ImColor color)
{
    config["r"] = color.Value.x;
    config["g"] = color.Value.y;
    config["b"] = color.Value.z;
    config["a"] = color.Value.w;
}
*/

void Settings::LoadDefaultsOrSave(std::string path)
{
    Json::Value settings;
    // inflate settings with currently loaded config 
    
    Json::StyledWriter styledWriter;
    std::ofstream(path) << styledWriter.write(settings);
}

void Settings::LoadConfig(std::string path)
{
    if (!std::ifstream(path).good())
    {
        Settings::LoadDefaultsOrSave(path);
        return;
    }

    Json::Value settings;
    std::ifstream configDoc(path, std::ifstream::binary);
    configDoc >> settings;

    GetVal(settings["Aimbot"]["k"], &Settings::Aimbot::k);
}

void Settings::LoadSettings()
{
    pstring directory = getenv("HOME");
    directory << "/.config";

    if (!DoesDirectoryExist(directory.c_str()))
        mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    directory << "/InsTux/";

    if (!DoesDirectoryExist(directory.c_str()))
        mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void remove_directory(const char* path)
{
    DIR* dir;
    dirent* pdir;

    dir = opendir(path);

    while ((pdir = readdir(dir)))
    {
        if (strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
            continue;

        if (pdir->d_type == DT_DIR)
        {
            pstring _dir;
            _dir << path << "/" << pdir->d_name;

            remove_directory(_dir.c_str());
        }
        else if (pdir->d_type == DT_REG)
        {
            pstring file;
            file << path << "/" << pdir->d_name;

            unlink(file.c_str());
        }
    }

    rmdir(path);
}

void Settings::DeleteConfig(std::string path)
{
    remove_directory(path.c_str());
}
