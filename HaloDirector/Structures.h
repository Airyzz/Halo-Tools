#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Camera {
	Vector3 position;
	Vector3 rotation;
};

struct CameraMarker {
	float time;
	float time_relative;
	float fov;
	Vector3 position;
	Vector3 forward;
	float roll;
};

struct Player {
	char unk[0x20];
	Camera camera;
};

struct World {
	char unk[0x90];
	Player* player;
};