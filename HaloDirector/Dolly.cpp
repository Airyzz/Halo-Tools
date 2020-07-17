#include "Dolly.h"
using namespace Halo;

float start_time_real = 0.0f;
float start_time = 0.0f;
CameraMarker Dolly::markers[MAX_DOLLY_MARKERS];
int Dolly::count = 0;


void Dolly::play()
{
	start_time_real = *serverSeconds;
	start_time = *serverTime;

	for (int i = 0; i < Dolly::count; i++)
	{
		float time_diff = Dolly::markers[i].time - Dolly::markers[0].time;
		Dolly::markers[i].time_relative = time_diff;
	}
}

void Dolly::update(int arg)
{
	//for (int i = 1; i < Dolly::count; i++)
	//{
	//	//if between two markers:
	//	if (*serverTime > Dolly::markers[i].time && *serverTime < Dolly::markers[i + 1].time)
	//	{
	//		//Calculate alpha
	//
	//		float current_time_relative = *serverSeconds - start_time - Dolly::markers[i].time_relative;
	//		float a = current_time_relative / (Dolly::markers[i + 1].time_relative - Dolly::markers[i].time_relative);
	//		Log::Info("Current Alpha: %f", a);
	//		break;
	//	}
	//}

		float current_time_relative = *serverSeconds - start_time_real - Dolly::markers[0].time_relative;
		float a = current_time_relative / (Dolly::markers[1].time_relative - Dolly::markers[0].time_relative);		
		Log::Info("Current Alpha: %f", a);


}

void Dolly::addMarker()
{
	if (Dolly::count < MAX_DOLLY_MARKERS)
	{
		Log::Debug("Adding Dolly Marker");
		Dolly::markers[Dolly::count] = CameraMarker();
		Dolly::markers[Dolly::count].time = *serverTime;
		Dolly::markers[Dolly::count].position = (*world)->player->camera.position;
		Dolly::markers[Dolly::count].fov = *fov;
		Dolly::count++;
	}
}

void demo_stop_dolly(int arg)
{
	Dolly::play();
}

void demo_begin_dolly(int arg)
{
	Dolly::play();
}

void demo_add_marker(int arg)
{
	Dolly::addMarker();
}

void demo_remove_marker(int arg)
{
	Dolly::removeMarker(arg);
}

void Dolly::removeMarker(int index)
{
}


bool dollyStarted = false;
DWORD __stdcall Dolly::Loop(LPVOID Param)
{
	while (true) {
		Sleep(1);
		if (Dolly::count > 0)
		{
			if (!dollyStarted && *serverTime > Dolly::markers[0].time)
			{
				dollyStarted = true;
				Dolly::play();
				Dolly::update(0);
			}
			else if (dollyStarted) {
				Dolly::update(0);
			}
		}
	}
}


void Dolly::Initialise() {

	Log::Info("Dolly Cam Markers allocated at: %llx", &Dolly::markers);

	ConsoleCommands::Add("demo_add_marker", &demo_add_marker);
	ConsoleCommands::Add("demo_remove_marker", &demo_remove_marker);
	ConsoleCommands::Add("demo_begin_dolly", &demo_begin_dolly);
	ConsoleCommands::Add("demo_update_dolly", &Dolly::update);

	CreateThread(0, 0, &Dolly::Loop, 0, 0, 0);
}
