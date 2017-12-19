#include "Camera.h"

void Camera::Pitch(float angle)
{
	 front = Matrix3::ROTATE(AXIS::X, angle) * front;
}

void Camera::Yaw(float angle)
{
	front = Matrix3::ROTATE(AXIS::Y, angle) * front;
}

Camera::Camera()
{
}

Camera::Camera(Vector3 position)
{
	this->position = position;
	this->front = Vector3(0, 0, 0) - position;
	this->currentCenter = Vector3(0, 0, 0);
	this->currentUp = Vector3(0, 1, 0);
	this->right = Vector3(1, 0, 0);
}

Camera::Camera(Vector3 position, Vector3 center) {
	this->position = position;
	this->currentCenter = center;
	this->front = center - position;
	this->currentUp = Vector3(0, 1, 0);
	this->right = Vector3::crossProduct(front, currentUp);
}

Camera::Camera(Vector3 center, Vector3 eye, Vector3 up)
{
	this->currentCenter = center;
	this->currentEye = eye;
	this->currentUp = up;
}

void Camera::MoveCamera(Direction dir, float velocity)
{
	switch (dir) {
	case Right:
		this->position += this->right * velocity;
		break;
	case Left:
		this->position -= this->right * velocity;
		break;
	case Forward:
		this->position += this->front * velocity;
		break;
	case Backward:
		this->position -= this->front * velocity;
		break;
	case Up:
		this->position += this->currentUp * velocity;
		break;
	case Down:
		this->position -= this->currentUp * velocity;
		break;
	}
}

void Camera::LookAt(float MouseX, float MouseY) {
	
	
	Matrix3 rod1 = Matrix3::ConvertFromMatrix4(Matrix4::RodriguesRotation(this->currentUp, MouseX));
	Matrix3 rod2 = Matrix3::ConvertFromMatrix4(Matrix4::RodriguesRotation(this->right, MouseY));

	this->front = rod2 * rod1 * this->front;

	this->currentUp = rod2 * rod1 * this->currentUp;
	//this->currentUp.coordinates[0] = 0;
	
	this->right = Vector3::crossProduct(this->front, this->currentUp).normalize();
	this->front = this->front.normalize();
	this->currentUp = this->currentUp.normalize();
	
}


void Camera::setProjectionMatrix(Matrix4 projection)
{
	this->ProjectionMatrix = projection;
}

Matrix4 Camera::GetProjectionMatrix()
{
	return this->ProjectionMatrix;
}

Matrix4 Camera::GetCamera()
{
	return Matrix4::ViewMatrix(position+front, position, currentUp);
}

Camera::~Camera()
{
}
void Camera::invertPitch()
{
	//this->front.coordinates[0] = -this->front.coordinates[0];
	//invert pitch
	//this->front.coordinates[1] = -this->front.coordinates[1];
	//Matrix4 rot = Matrix4::RodriguesRotation(Vector3(1, 0, 0), 90);
	//this->front = Matrix3::ConvertFromMatrix4(rot) * this->front;
	this->front.coordinates[1] = -this->front.coordinates[1];
	//this->front.coordinates[0] = -this->front.coordinates[0];
	this->right = -this->right;

	//invert right to left
	//this->right = -this->right;
//	this->currentUp = Vector3::crossProduct(Vector3::crossProduct(this->front, Vector3(0, 1, 0)), this->front);
	//invert down to up?

}
