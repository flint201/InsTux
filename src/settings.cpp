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
    // inflate settings with default values
	//settings["UI"]["Fonts"]["ESP"]["family"] = Settings::UI::Fonts::ESP::family;
    settings["Aimbot"]["k"] = Settings::Aimbot::k;
    
    Json::StyledWriter styledWriter;
    std::ofstream(path) << styledWriter.write(settings);
}

void Settings::LoadConfig()
{
    pstring path = getenv("HOME");
    path << "/.instux.cfg";

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

