#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/Tema2/camera/camera.h"

using namespace m1;

Camera::Camera()
{
    position = glm::vec3(0, 2, 5);
    forward = glm::vec3(0, 0, -1);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    distanceToTarget = 2;
}

Camera::Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
{
    Set(position, center, up);
}

Camera::~Camera()
{ }

// Update camera
void Camera::Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
{
    this->position = position;
    forward = glm::normalize(center - position);
    right = glm::cross(forward, up);
    this->up = glm::cross(right, forward);
}

// Translates the camera using the `dir` vector computed from
// `forward`. Movement will always keep the camera at the same
// height. For example, if you rotate your head up/down, and then
// walk forward, then you will still keep the same relative
// distance (height) to the ground!
void Camera::MoveForward(float distance)
{
   
    glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}

// Translate the camera using the `forward` vector.
// Changes distance from the ground depending on camera orientation
void Camera::TranslateForward(float distance)
{
    
    position += distance * glm::normalize(forward);
}

void Camera::TranslateUpward(float distance)
{
    position += distance * up;
}

void Camera::TranslateRight(float distance)
{
    glm::vec3 rightProjected = glm::normalize(glm::vec3(right.x, 0, right.z));
    position += rightProjected * distance;
}

void Camera::RotateFirstPerson_OX(float angle)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, right);
    forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::RotateFirstPerson_OY(float angle)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f)));
    right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::RotateFirstPerson_OZ(float angle)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, forward);
    right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::RotateThirdPerson_OX(float angle)
{
    position += forward * distanceToTarget;
    RotateFirstPerson_OX(angle);
    position -= forward * distanceToTarget;
}

void Camera::RotateThirdPerson_OY(float angle)
{
    position += forward * distanceToTarget;
    RotateFirstPerson_OY(angle);
    position -= forward * distanceToTarget;
}

void Camera::RotateThirdPerson_OZ(float angle)
{
    position += forward * distanceToTarget;
    RotateFirstPerson_OZ(angle);
    position -= forward * distanceToTarget;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}

glm::vec3 Camera::GetTargetPosition()
{
    return position + forward * distanceToTarget;
}