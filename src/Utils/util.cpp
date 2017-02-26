#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#include "../logger.h"

std::string Util::ReplaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;

	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}

	return subject;
}

void Util::StdReplaceStr(std::string& replaceIn, const std::string& replace, const std::string& replaceWith)
{
	size_t const span = replace.size();
	size_t const step = replaceWith.size();
	size_t index = 0;

	while (true)
	{
		index = replaceIn.find(replace, index);

		if (index == std::string::npos)
			break;

		replaceIn.replace(index, span, replaceWith);
		index += step;
	}
}

const char* Util::PadStringRight(std::string text, size_t value)
{
	text.insert(text.length(), value - text.length(), ' ');

	return text.c_str();
}

void Util::ProtectAddr(void* addr, int prot)
{
	long pagesize = sysconf(_SC_PAGESIZE);
	void* address = (void *)((long)(uintptr_t)addr & ~(pagesize - 1));

	mprotect(address, sizeof(address), prot);
}

bool Util::Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string Util::ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int (*)(int))std::tolower);

	return str;
}

std::string Util::ToUpper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int (*)(int))std::toupper);

	return str;
}

std::string Util::WstringToString(std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	try
	{
		return converter.to_bytes(wstr);
	}
	catch (std::range_error)
	{
		std::stringstream s;
		s << wstr.c_str();
		return s.str();
	}
}

std::wstring Util::StringToWstring(std::string str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	try
	{
		return converter.from_bytes(str);
	}
	catch (std::range_error)
	{
		std::wostringstream s;
		s << str.c_str();
		return s.str();
	}
}

long Util::GetEpochTime()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();

	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}


void Util::MoveMouse(int x, int y)
{
    Log << " @ MoveMouse" << std::endl;
    Display *display = XOpenDisplay(NULL);
    XWarpPointer(display, None, None, 0, 0, 0, 0, x, y);
    XCloseDisplay(display);
}

bool Util::KeyDown(KeySym keySym)
{
    static Display *display = XOpenDisplay(NULL);
    char keyState[32];
    XQueryKeymap(display, keyState);
    int keyCode = (int)XKeysymToKeycode(display, keySym);
    return keyState[keyCode/8] & (0x1 << keyCode%8);
}

bool Util::MouseButtonDown(KeySym keySym)
{
    static Display *display = XOpenDisplay(NULL);
    Window root_return, child_return;
    int root_x_return, root_y_return;
    int win_x_return, win_y_return;
    unsigned int mask_return = 0;
    XQueryPointer(display, DefaultRootWindow(display),
            &root_return, &child_return,
            &root_x_return, &root_y_return,
            &win_x_return, &win_y_return,
            &mask_return);
    if (mask_return != 0)
        Log << (unsigned) mask_return << endl;
    return mask_return & keySym;
}
