#include "settings.h"

bool Settings::Aimbot::enable = true;
ButtonCode_t Settings::Aimbot::key = ButtonCode_t::KEY_LSHIFT;
float Settings::Aimbot::sensitivity = 1.1;
bool Settings::Aimbot::limit_aim_speed = true;
float Settings::Aimbot::ku = 9;
float Settings::Aimbot::tu = 0.1;
float Settings::Aimbot::fov = 10.0;
float Settings::Aimbot::cont_thresh = 5.0;

float Settings::Aimbot::silent_fov = 0.5;
float Settings::Aimbot::silent_fov_hip = 60;
float Settings::Aimbot::recoilx = 2.0;
float Settings::Aimbot::recoily = 2.0;

bool Settings::Cham::only_on_key_down = false;
ButtonCode_t Settings::Cham::key = KEY_CAPSLOCK;
Color Settings::Cham::color_visible = Color(0, 255, 0, 255);
Color Settings::Cham::color_hidden = Color(255, 0, 0, 255);

bool Settings::ESP::enable = true;
ButtonCode_t Settings::ESP::key = KEY_CAPSLOCK;
bool Settings::ESP::show_bone = true;
bool Settings::ESP::show_name = true;
bool Settings::ESP::draw_behind = true;

bool Settings::Radar::enable = true;
int Settings::Radar::size = 100;
float Settings::Radar::range = 60;
float Settings::Radar::dot_radius = 4;
Color Settings::Radar::color_friendly = Color(50, 255, 50, 180);
Color Settings::Radar::color_hostile = Color(255, 108, 10, 250);
bool Settings::Radar::draw_level = true;

float Settings::NoFlash::alpha = 80;

bool Settings::NoSmoke::enable = true;

ButtonCode_t Settings::FakeLag::key = ButtonCode_t::KEY_BACKSLASH;
bool Settings::FakeLag::automatic = true;
int Settings::FakeLag::value = 12;

Color Settings::GUI::color_main = Color(165, 38, 114, 200);
Color Settings::GUI::color_main2 = Color(219, 0, 131, 180);
Color Settings::GUI::color_hl = Color(255, 140, 209, 200);

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

void GetVal(Json::Value &config, Color* setting)
{
    if (config.isNull())
        return;

    GetVal(config["r"], &setting->r);
    GetVal(config["g"], &setting->g);
    GetVal(config["b"], &setting->b);
    GetVal(config["a"], &setting->a);
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

void LoadColor(Json::Value &config, Color color)
{
    config["r"] = color.r;
    config["g"] = color.g;
    config["b"] = color.b;
    config["a"] = color.a;
}

void Settings::SaveToFile(std::string path)
{
    Json::Value settings;

    // inflate settings with current values
    settings["Aimbot"]["enable"] = Settings::Aimbot::enable;
    settings["Aimbot"]["key"] = Util::GetButtonName(Settings::Aimbot::key);
    settings["Aimbot"]["sensitivity"] = Settings::Aimbot::sensitivity;
    settings["Aimbot"]["limit_aim_speed"] = Settings::Aimbot::limit_aim_speed;
    settings["Aimbot"]["fov"] = Settings::Aimbot::fov;
    settings["Aimbot"]["tu"] = Settings::Aimbot::tu;
    settings["Aimbot"]["ku"] = Settings::Aimbot::ku;
    settings["Aimbot"]["cont_thresh"] = Settings::Aimbot::cont_thresh;

    settings["Aimbot"]["silent_fov"] = Settings::Aimbot::silent_fov;
    settings["Aimbot"]["silent_fov_hip"] = Settings::Aimbot::silent_fov_hip;

    settings["Aimbot"]["recoilx"] = Settings::Aimbot::recoilx;
    settings["Aimbot"]["recoily"] = Settings::Aimbot::recoily;

    settings["Cham"]["only_on_key_down"] = Settings::Cham::only_on_key_down;
    settings["Cham"]["key"] = Util::GetButtonName(Settings::Cham::key);
    LoadColor(settings["Cham"]["color_visible"], Settings::Cham::color_visible);
    LoadColor(settings["Cham"]["color_hidden"], Settings::Cham::color_hidden);

    settings["ESP"]["enable"] = Settings::ESP::enable;
    settings["ESP"]["key"] = Util::GetButtonName(Settings::ESP::key);
    settings["ESP"]["show_bone"] = Settings::ESP::show_bone;
    settings["ESP"]["show_name"] = Settings::ESP::show_name;
    settings["ESP"]["draw_behind"] = Settings::ESP::draw_behind;

    settings["Radar"]["enable"] = Settings::Radar::enable;
    settings["Radar"]["size"] = Settings::Radar::size;
    settings["Radar"]["range"] = Settings::Radar::range;
    settings["Radar"]["dot_radius"] = Settings::Radar::dot_radius;
    LoadColor(settings["Radar"]["color_friendly"], Settings::Radar::color_friendly);
    LoadColor(settings["Radar"]["color_hostile"], Settings::Radar::color_hostile);
    settings["Radar"]["draw_level"] = Settings::Radar::draw_level;

    settings["NoFlash"]["alpha"] = Settings::NoFlash::alpha;

    settings["NoSmoke"]["enable"] = Settings::NoSmoke::enable;

    settings["FakeLag"]["key"] = Util::GetButtonName(Settings::FakeLag::key);
    settings["FakeLag"]["automatic"] = Settings::FakeLag::automatic;
    settings["FakeLag"]["value"] = Settings::FakeLag::value;

    LoadColor(settings["GUI"]["color_main"], Settings::GUI::color_main);
    LoadColor(settings["GUI"]["color_main2"], Settings::GUI::color_main2);
    LoadColor(settings["GUI"]["color_hl"], Settings::GUI::color_hl);
    
    Json::StyledWriter styledWriter;
    //std::ofstream(path) << styledWriter.write(settings);
    std::string strJson = styledWriter.write(settings);
    Log << strJson.c_str() << endl;
    Log << strJson.length() << endl;
    FILE* file = fopen(path.c_str(), "w+");
    if (file)
    {
        fwrite(strJson.c_str(), 1, strJson.length(), file);
        fclose(file);
    }
}

void Settings::LoadConfig()
{
    pstring path = getenv("HOME");
    path << "/.instux.cfg";

    FILE* infile = fopen(path.c_str(), "r");
    if (!infile)
    {
        Settings::SaveToFile(path);
        return;
    }

    fseek(infile, 0, SEEK_END);
    long filesize = ftell(infile);
    char* buf = new char[filesize+1];
    fseek(infile, 0, SEEK_SET);
    fread(buf, 1, filesize, infile);
    buf[filesize] = '\0';
    fclose(infile);

    std::stringstream ss;
    ss.str(buf);

    Json::Value settings;
    ss >> settings;

    GetVal(settings["Aimbot"]["enable"], &Settings::Aimbot::enable);
    GetButtonCode(settings["Aimbot"]["key"], &Settings::Aimbot::key);
    GetVal(settings["Aimbot"]["sensitivity"], &Settings::Aimbot::sensitivity);
    GetVal(settings["Aimbot"]["limit_aim_speed"], &Settings::Aimbot::limit_aim_speed);
    GetVal(settings["Aimbot"]["fov"], &Settings::Aimbot::fov);
    GetVal(settings["Aimbot"]["tu"], &Settings::Aimbot::tu);
    GetVal(settings["Aimbot"]["ku"], &Settings::Aimbot::ku);
    GetVal(settings["Aimbot"]["cont_thresh"], &Settings::Aimbot::cont_thresh);

    GetVal(settings["Aimbot"]["silent_fov"], &Settings::Aimbot::silent_fov);
    GetVal(settings["Aimbot"]["silent_fov_hip"], &Settings::Aimbot::silent_fov_hip);
    GetVal(settings["Aimbot"]["recoilx"], &Settings::Aimbot::recoilx);
    GetVal(settings["Aimbot"]["recoily"], &Settings::Aimbot::recoily);

    GetVal(settings["Cham"]["only_on_key_down"], &Settings::Cham::only_on_key_down);
    GetButtonCode(settings["Cham"]["key"], &Settings::Cham::key);
    GetVal(settings["Cham"]["color_visible"], &Settings::Cham::color_visible);
    GetVal(settings["Cham"]["color_hidden"], &Settings::Cham::color_hidden);

    GetVal(settings["ESP"]["enable"], &Settings::ESP::enable);
    GetButtonCode(settings["ESP"]["key"], &Settings::ESP::key);
    GetVal(settings["ESP"]["show_bone"], &Settings::ESP::show_bone);
    GetVal(settings["ESP"]["show_name"], &Settings::ESP::show_name);
    GetVal(settings["ESP"]["draw_behind"], &Settings::ESP::draw_behind);

    GetVal(settings["Radar"]["enable"], &Settings::Radar::enable);
    GetVal(settings["Radar"]["size"], &Settings::Radar::size);
    GetVal(settings["Radar"]["range"], &Settings::Radar::range);
    GetVal(settings["Radar"]["dot_radius"], &Settings::Radar::dot_radius);
    GetVal(settings["Radar"]["color_friendly"], &Settings::Radar::color_friendly);
    GetVal(settings["Radar"]["color_hostile"], &Settings::Radar::color_hostile);
    GetVal(settings["Radar"]["draw_level"], &Settings::Radar::draw_level);

    GetVal(settings["NoFlash"]["alpha"], &Settings::NoFlash::alpha);

    GetVal(settings["NoSmoke"]["enable"], &Settings::NoSmoke::enable);

    GetButtonCode(settings["FakeLag"]["key"], &Settings::FakeLag::key);
    GetVal(settings["FakeLag"]["automatic"], &Settings::FakeLag::automatic);
    GetVal(settings["FakeLag"]["value"], &Settings::FakeLag::value);

    GetVal(settings["GUI"]["color_main"], &Settings::GUI::color_main);
    GetVal(settings["GUI"]["color_main2"], &Settings::GUI::color_main2);
    GetVal(settings["GUI"]["color_hl"], &Settings::GUI::color_hl);
}
