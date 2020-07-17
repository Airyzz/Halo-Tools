#include "framework.h"
#include "Log.h"
#include <ctime>


void Log::SetTextColour(uint8_t colour) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
}

void Log::Print(logChannel channel, const char* format, va_list args) {

	while (queue > 0)Sleep(1);

	queue++;

	char line[1024];
	vsnprintf(line, 1024, format, args);

	char time[9];
	_strtime_s(time);
	SetTextColour(DarkGrey);

	_cprintf("%s", time);

	SetTextColour(logColours[channel]);

	_cprintf(" %s] ", channelNames[channel]);

	SetTextColour(White);
	_cprintf("%s\r\n", line);

	queue--;
}

void Log::Info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Print(logChannel::Info, format, args);
	va_end(args);
}

void Log::Error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Print(logChannel::Error, format, args);
	va_end(args);
}

void Log::Debug(const char* format, ...) {
	va_list args;
	va_start(args, format);
	Print(logChannel::Debug, format, args);
	va_end(args);
}

void Log::DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		_cprintf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		}
		else {
			ascii[i % 16] = '.';
		}
		if ((i + 1) % 8 == 0 || i + 1 == size) {
			_cprintf(" ");
			if ((i + 1) % 16 == 0) {
				_cprintf("|  %s \n", ascii);
			}
			else if (i + 1 == size) {
				ascii[(i + 1) % 16] = '\0';
				if ((i + 1) % 16 <= 8) {
					_cprintf(" ");
				}
				for (j = (i + 1) % 16; j < 16; ++j) {
					_cprintf("   ");
				}
				_cprintf("|  %s \n", ascii);
			}
		}
	}
}

uint32_t Log::queue = 0;