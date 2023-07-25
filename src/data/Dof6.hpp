#pragma once

#include "Measure.hpp"

namespace hub {
namespace data {

///
/// \brief The Dof6 class
/// represents a 6 degree of freedom measure that can be acquired with some tracking solution.
///
class SRC_API Dof6 : public Measure
{
  public:
    ///
    /// \brief Dof6
    /// copy constructor.
    /// \param measure
    /// to copy.
    ///
    explicit Dof6( const Measure& measure );

    ///
    /// \brief Dof6
    /// corresponds to a position (x, y, z) and an orientation quaternion (w0, w1, w2, w3 | w, x, y,
    /// z). \param x position \param y position \param z position \param w0 orientation \param w1
    /// orientation
    /// \param w2
    /// orientation
    /// \param w3
    /// orientation
    ///
    explicit Dof6( float x  = 0.0,
                   float y  = 0.0,
                   float z  = 0.0,
                   float w0 = 1.0,
                   float w1 = 0.0,
                   float w2 = 0.0,
                   float w3 = 0.0 );

    ///
    /// \brief slerp
    /// \param left
    /// \param right
    /// \param t
    /// \return
    ///
    static Dof6 slerp( const Dof6& left, const Dof6& right, double t );

    ///
    /// \brief operator <<
    /// \param os
    /// \param dof6
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Dof6& dof6 );

    ///
    /// \brief x
    /// \return
    ///
    inline float get_x() const;

    ///
    /// \brief y
    /// \return
    ///
    inline float get_y() const;

    ///
    /// \brief z
    /// \return
    ///
    inline float get_z() const;

    ///
    /// \brief w0
    /// \return
    ///
    inline float get_w0() const;

    ///
    /// \brief w1
    /// \return
    ///
    inline float get_w1() const;

    ///
    /// \brief w2
    /// \return
    ///
    inline float get_w2() const;

    ///
    /// \brief w3
    /// \return
    ///
    inline float get_w3() const;

  private:
    const float m_x = 0.0, m_y = 0.0, m_z = 0.0;                // vec3
    const float m_w0 = 1.0, m_w1 = 0.0, m_w2 = 0.0, m_w3 = 0.0; // quat : w, x, y, z
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Dof6::get_x() const {
    return m_x;
}

inline float Dof6::get_y() const {
    return m_y;
}

inline float Dof6::get_z() const {
    return m_z;
}

inline float Dof6::get_w0() const {
    return m_w0;
}

inline float Dof6::get_w1() const {
    return m_w1;
}

inline float Dof6::get_w2() const {
    return m_w2;
}

inline float Dof6::get_w3() const {
    return m_w3;
}

} // namespace data
} // namespace hub
