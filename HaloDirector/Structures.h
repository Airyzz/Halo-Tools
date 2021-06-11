#pragma once

struct Vector3 {
	float x;
	float y;
	float z;

    // Construct with values, 3D
    Vector3(float ax, float ay, float az) {
        x = ax;
        y = ay;
        z = az;
    }

    Vector3() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    inline Vector3 operator+(const Vector3& o) {
        return Vector3(x + o.x, y + o.y, z + o.z);
    }

    inline Vector3& operator+=(const Vector3& o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

    // Subtraction

    inline Vector3 operator-() {
        return Vector3(-x, -y, -z);
    }

    inline Vector3 operator-(const Vector3 o) {
        return Vector3(x - o.x, y - o.y, z - o.z);
    }

    inline Vector3& operator-=(const Vector3 o) {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }

    // Multiplication by scalars

    inline Vector3 operator*(const float s) {
        return Vector3(x * s, y * s, z * s);
    }

    inline Vector3& operator*=(const float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    // Division by scalars

    inline Vector3 operator/(const float s) {
        return Vector3(x / s, y / s, z / s);
    }

    inline Vector3& operator/=(const float s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    // Dot product
    float operator*(const Vector3 o) {
        return (x * o.x) + (y * o.y) + (z * o.z);
    }

    // An in-place dot product does not exist because
    // the result is not a vector.

    // Cross product

    inline Vector3 operator^(const Vector3 o) {
        float nx = y * o.z - o.y * z;
        float ny = z * o.x - o.z * x;
        float nz = x * o.y - o.x * y;
        return Vector3(nx, ny, nz);
    }

    inline Vector3& operator^=(const Vector3 o) {
        float nx = y * o.z - o.y * z;
        float ny = z * o.x - o.z * x;
        float nz = x * o.y - o.x * y;
        x = nx;
        y = ny;
        z = nz;
        return *this;
    }
};

struct Vector2 {
    float x;
    float y;
};

struct Vector4 {
    float x;
    float y;
    float z;
    float w;
};

struct Camera_Old {
	Vector3 position;
	Vector3 rotation;
};

struct Camera {
    char unk[6 * 4];
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
	Camera_Old camera;
};

struct World {
	char unk[0x90];
	Player* player;
};

struct matrix {
    float values[4][4];
};