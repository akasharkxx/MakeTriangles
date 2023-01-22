#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

enum CameraMovement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor	
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// using scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		//return glm::lookAt(Position, Position + Front, Up);
		return calculateLookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			Position += Front * velocity;
		}
		if (direction == BACKWARD)
		{
			Position -= Front * velocity;
		}
		if (direction == LEFT)
		{
			Position -= Right * velocity;
		}
		if (direction == RIGHT)
		{
			Position += Right * velocity;
		}
		if (direction == UP)
		{
			Position += WorldUp * velocity;
		}
		if (direction == DOWN)
		{
			Position -= WorldUp * velocity;
		}

		//Position.y = 0.0f;
	}

	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= MouseSensitivity;
		yOffset *= MouseSensitivity;

		Yaw += xOffset;
		Pitch += yOffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
			{
				Pitch = 89.0f;
			}
			if (Pitch < -89.0f)
			{
				Pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yOffset)
	{
		Zoom -= yOffset;

		if (Zoom < 1.0f)
		{
			Zoom = 1.0f;
		}
		if (Zoom > 80.0f)
		{
			Zoom = 80.0f;
		}
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front	= glm::normalize(front);
		Right	= glm::normalize(glm::cross(Front, WorldUp));
		Up		= glm::normalize(glm::cross(Right, Front));
	}

	glm::mat4 calculateLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
	{
		glm::vec3 zAxis = glm::normalize(position - target);
		glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(worldUp), zAxis));
		glm::vec3 yAxis = glm::cross(zAxis, xAxis);

		glm::mat4 translation = glm::mat4(1.0f);
		translation[3][0] = -position.x;
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;

		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xAxis.x;
		rotation[1][0] = xAxis.y;
		rotation[2][0] = xAxis.z;
		rotation[0][1] = yAxis.x;
		rotation[1][1] = yAxis.y;
		rotation[2][1] = yAxis.z;
		rotation[0][2] = zAxis.x;
		rotation[1][2] = zAxis.y;
		rotation[2][2] = zAxis.z;

		return rotation * translation;
	}
};

#endif // !CAMERA_H
