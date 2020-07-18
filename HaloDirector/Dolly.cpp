#include "Dolly.h"
using namespace Halo;

float start_time_real = 0.0f;
float start_time = 0.0f;
CameraMarker Dolly::markers[MAX_DOLLY_MARKERS];
int Dolly::count = 0;

bool dollyStarted = false;
float lastTime = 0;
float alpha = 0.0f;

void Dolly::play()
{
	start_time_real = *serverSeconds;
	start_time = *serverTime;

	Log::Info("Beginning Dolly Cam");

	for (int i = 0; i < Dolly::count; i++)
	{
		float time_diff = Dolly::markers[i].time - Dolly::markers[0].time;
		Dolly::markers[i].time_relative = time_diff;
	}
}

void Dolly::update(int arg)
{
	for (int i = 0; i < Dolly::count; i++)
	{
		//if between two markers:
		if (*serverTime > Dolly::markers[i].time && *serverTime < Dolly::markers[i + 1].time)
		{
			CameraMarker* m0;
			CameraMarker* m1;
			CameraMarker* m2;
			CameraMarker* m3;

			//these are just for debugging
			int i0, i1, i2, i3;

			//surely theres a better way to do this
			//however, im stupid and this works.
			//i got that cave man brain
			if (i > 0)
			{
				m0 = &Dolly::markers[i - 1];
				i0 = i - 1;

				m1 = &Dolly::markers[i];
				i1 = i;
			}
			else {
				m0 = &Dolly::markers[i];
				m1 = &Dolly::markers[i];

				i0 = i;
				i1 = i;
			}
			
			if ((Dolly::count - i) > 2)
			{
				m2 = &Dolly::markers[i + 1];
				i2 = i + 1;

				m3 = &Dolly::markers[i + 2];
				i3 = i + 2;
			}
			else {
				m2 = &Dolly::markers[i + 1];
				i2 = i + 1;
				i3 = i + 1;

				m3 = &Dolly::markers[i + 1];
			}

			//We gotta do it like this to break free from the game's horrible tick rate
			float current_time_relative = *serverSeconds - start_time_real - Dolly::markers[i].time_relative;
			float alpha = current_time_relative / (Dolly::markers[i + 1].time_relative - Dolly::markers[i].time_relative);

			Log::Info("%d] Count: %d Indices: %d, %d, %d, %d,      -     %f",i, Dolly::count, i0, i1, i2, i3, alpha);

			//lerp position
			(*world)->player->camera.position = Math::CatmullRomInterpolate(
				m0->position, 
				m1->position,
				m2->position,
				m3->position,
				alpha, 
				0.0f);

			//lerp forward position and then look at it. idk cod did it like this 
			Vector3 forward = Math::CatmullRomInterpolate(
				m0->forward,
				m1->forward,
				m2->forward,
				m3->forward,
				alpha,
				0.0f);

			(*world)->player->camera.rotation = Math::LookAt((*world)->player->camera.position, forward);

			//we just gonna use cosine interpolation for these because Catmullrom was being fucky. ill come back to this
			(*world)->player->camera.rotation.z =	Math::CosineInterpolate(m1->roll, m2->roll, alpha);
			*fov =									Math::CosineInterpolate(m1->fov, m2->fov, alpha);
			break;
		}
	}
}

void Dolly::addMarker()
{
	if (Dolly::count < MAX_DOLLY_MARKERS)
	{
		Camera c = (*world)->player->camera;
		Log::Debug("Adding Dolly Marker");
		Dolly::markers[Dolly::count] = CameraMarker();
		Dolly::markers[Dolly::count].time = *serverTime;
		Dolly::markers[Dolly::count].position = c.position;
		Dolly::markers[Dolly::count].forward = Math::GetForwardPosition(1.5f, c.position, c.rotation);
		Dolly::markers[Dolly::count].fov = *fov;
		Dolly::markers[Dolly::count].roll = c.rotation.z;
		Dolly::count++;
	}
}

void demo_stop_dolly(int arg)
{
	dollyStarted = false;
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

void Dolly::removeAll()
{
	Dolly::count = 0;
	for (int i = 0; i < MAX_DOLLY_MARKERS; i++)
	{
		Dolly::markers[i] = CameraMarker();
	}
}


void demo_test(int arg) {
	(*world)->player->camera.position = Math::GetForwardPosition(0.5f, (*world)->player->camera.position, (*world)->player->camera.rotation);
}

DWORD __stdcall Dolly::Loop(LPVOID Param)
{
	while (true) {
		Sleep(1);

		if (*serverTime < lastTime)
		{
			float diff = lastTime - *serverTime;
			start_time_real += diff;
			Log::Info("Jump Backwards detected: Offsetting start time by: %f", diff);
		}

		if (Dolly::count > 1)
		{
			if (dollyStarted == false && *serverTime > Dolly::markers[0].time)
			{
				dollyStarted = true;
				Dolly::play();
				Dolly::update(0);
			}
			else if (dollyStarted && *serverTime > Dolly::markers[0].time && *serverTime < Dolly::markers[Dolly::count - 1].time) {
				Dolly::update(0);
			}
			else if (*serverTime < Dolly::markers[0].time && dollyStarted)
			{
				Log::Info("Now before first marker. Stopping dolly");
				dollyStarted = false;
			}
		}

		lastTime = *serverTime;
	}
}

void Dolly::Initialise() {

	Log::Info("Dolly Cam Markers allocated at: %llx", &Dolly::markers);
	Log::Info("Alpha allocated at: %llx", &alpha);
	Log::Info("Start Time Real: %llx", &start_time_real);

	ConsoleCommands::Add("demo_add_marker", &demo_add_marker);
	ConsoleCommands::Add("demo_remove_marker", &demo_remove_marker);
	ConsoleCommands::Add("demo_begin_dolly", &demo_begin_dolly);
	ConsoleCommands::Add("demo_stop_dolly", &demo_stop_dolly);
	ConsoleCommands::Add("demo_update_dolly", &Dolly::update);
	ConsoleCommands::Add("demo_test", &demo_test);

	CreateThread(0, 0, &Dolly::Loop, 0, 0, 0);
}
