//! @file    Trackball.cpp
//! @author  Johan Nysjo
//! @date    <2014-03-15 Sat>
//!
//! @brief Source file with definitions for Trackball.h
//!

#include "Trackball.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/constants.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>

// Unnamed namespace (for helper functions and constants)
//
namespace {
glm::vec3 mapMousePointToUnitSphere(glm::vec2 point, double radius, glm::vec2 center)
{
    float x = point[0] - center[0];
    float y = -point[1] + center[1];
    float z = 0.0f;
    if (x * x + y * y < radius * radius / 2.0f) {
        z = sqrt(radius * radius - (x * x + y * y));
    }
    else {
        z = (radius * radius / 2.0f) / sqrt(x * x + y * y);
    }
    return glm::normalize(glm::vec3(x, y, z));
}
}

using namespace cgtk;

Trackball::Trackball() :
    mRadius(1.0),
    mCenter(glm::vec2(0.0f, 0.0f)),
    mTracking(false),
    mVStart(glm::vec3(0.0f, 0.0f, 1.0f)),
    mQStart(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
    mQCurrent(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
{
    std::cout << "Called Trackball constructor" << std::endl;
}

Trackball::~Trackball()
{
}

void Trackball::setRadius(double radius)
{
    mRadius = radius;
}

double Trackball::getRadius() const
{
    return mRadius;
}

void Trackball::setCenter(glm::vec2 center)
{
    mCenter = center;
}

glm::vec2 Trackball::getCenter() const
{
    return mCenter;
}

void Trackball::startTracking(glm::vec2 point)
{
    mVStart = mapMousePointToUnitSphere(point, mRadius, mCenter);
    mQStart = glm::quat(mQCurrent);
    mTracking = true;
}

void Trackball::move(glm::vec2 point)
{
    glm::vec3 vCurrent = mapMousePointToUnitSphere(point, mRadius, mCenter);
    glm::vec3 rotationAxis = glm::cross(mVStart, vCurrent);
    float dotProduct = std::max(std::min(glm::dot(mVStart, vCurrent), 1.0f), -1.0f);
    float rotationAngle = acos(dotProduct);
    rotationAngle *= 180.0f / glm::pi<float>();
    float eps = 0.01f;
    if (rotationAngle < eps) {
        mQCurrent = glm::quat(mQStart);
    }
    else {
        glm::quat q = glm::angleAxis(rotationAngle, rotationAxis);
        q = glm::normalize(q);
        mQCurrent = glm::normalize(glm::cross(q, mQStart));
    }
}

void Trackball::stopTracking()
{
    mTracking = false;
}

bool Trackball::tracking() const
{
    return mTracking;
}

glm::mat4 Trackball::getRotationMatrix()
{
    return glm::mat4_cast(mQCurrent);
}
