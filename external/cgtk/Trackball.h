//! @file    Trackball.h
//! @author  Johan Nysjo
//! @date    <2014-03-15 Sat>
//!
//! @brief Header declaring the Trackball class
//!

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace cgtk {
//! @class Trackball Trackball.h Trackball.h
//!
//! @brief Trackball class
//!
//! A virtual 3D trackball that can be used for object or camera
//! rotation.
//!
class Trackball {
public:
    //! Constructor
    //!
    Trackball();

    //! Destructor
    //!
    ~Trackball();

    //! Set the radius of the trackball.
    //!
    //! @param[in] radius A double specifying the trackball radius (in
    //! pixels).
    //!
    void setRadius(double radius);

    //! Get the trackball radius.
    //!
    //! @return A double specifying the trackball radius (in pixels).
    //!
    double getRadius() const;

    //! Set the trackball center.
    //!
    //! @param[in] center A vec2 specifying the trackball center (in pixel
    //! coordinates).
    //!
    void setCenter(glm::vec2 center);

    //! Get the trackball center.
    //!
    //! @return A vec2 specifying the trackball center (in pixel
    //! coordinates).
    //!
    glm::vec2 getCenter() const;

    //! Start tracking.
    //!
    //! @param[in] point A vec2 specifying the current mouse position
    //! (in screen coordinates).
    //!
    void startTracking(glm::vec2 point);

    //! Track the mouse movement.
    //!
    //! @param[in] point A vec2 specifying the current mouse position
    //! (in screen coordinates).
    //!
    void move(glm::vec2 point);

    //! Stop tracking.
    //!
    void stopTracking();

    //! Check whether the trackball is tracking or not.
    //!
    //! @return true if the trackball is tracking, false otherwise.
    //!
    bool tracking() const;

    //! Get the rotation matrix.
    //!
    //! @return A 4-by-4 matrix representing the accumulated rotation
    //! of the trackball.
    //!
    glm::mat4 getRotationMatrix();
private:
    double mRadius;
    glm::vec2 mCenter;
    bool mTracking;
    glm::vec3 mVStart;
    glm::quat mQStart;
    glm::quat mQCurrent;
};
}
