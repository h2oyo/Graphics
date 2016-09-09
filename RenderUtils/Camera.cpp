#include "Input.h"
#include "Timer.h"
#include "Camera.h"

//
//void FlyCamera::lookAt(const glm::vec3 &target)
//{
//	//auto mat =
//	//glm::lookAt(position, normalize(target - position), glm::vec3(0, 1, 0));
//	direction = normalize(target - position);
//
//
//}
void FlyCamera::update(const Input &in, const Time &t)
{
	glm::vec3 moveDir = glm::vec3(0, 0, 0);
	glm::vec3 right = glm::cross(direction, glm::vec3(0, 1, 0));
	glm::vec3 up = glm::cross(right, direction);

	yaw -= in.getMouseAxisHorizontal() * t.getDeltaTime();
	pitch += in.getMouseAxisVertical() * t.getDeltaTime();

	direction = glm::vec3(glm::rotate(yaw, glm::vec3(0, 1, 0))[2]);
	right = glm::cross(direction, glm::vec3(0, 1, 0));

	direction = glm::mat3(glm::rotate(pitch, right)) * direction;

	if (in.getKeyState('W') == Input::DOWN) moveDir += direction;
	if (in.getKeyState('S') == Input::DOWN) moveDir -= direction;
	if (in.getKeyState('A') == Input::DOWN) moveDir -= right;
	if (in.getKeyState('D') == Input::DOWN) moveDir += right;
	if (in.getKeyState('E') == Input::DOWN) moveDir -= up;
	if (in.getKeyState('Q') == Input::DOWN) moveDir += up;



	//direction.x = cos(yaw) * sin(pitch);
	//direction.y = cos(yaw);
	//direction.z = sin(yaw) * sin(pitch);




	if (glm::length(moveDir) >= .7f)
	{
		moveDir = glm::normalize(moveDir);
		position += moveDir * t.getDeltaTime() * speed;
	}
}