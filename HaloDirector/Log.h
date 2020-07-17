#pragma once
#include <stdarg.h>
#include <conio.h>
#include <iostream>

enum Colour
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGrey = 7,
	DarkGrey = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15,
	Blink = 128,
};

static const Colour logColours[]{ Green, Red, LightCyan };
static const char* channelNames[]{ "INFO", "ERROR", "DEBUG" };

enum logChannel
{
	Info,
	Error,
	Debug
};

enum LogLevel
{
	LogLevel_None = 0,
	LogLevel_Warning = 1,
	LogLevel_Error = 2,
	LogLevel_Debug = 4,
	LogLevel_Info = 8,
	LogLevel_Trace = 16,
	LogLevel_All = 31,
};



class Log
{
public:
	static void DumpHex(const void* data, size_t size);
	static void Info(const char* text, ...);
	static void Error(const char* text, ...);
	static void Debug(const char* text, ...);

private:
	static void SetTextColour(uint8_t colour);
	static void Print(logChannel channel, const char* text, va_list args);
	static uint32_t queue;
};
