#pragma once

#include "core/Any.hpp"
#include "Measure.hpp"

namespace hub {
namespace data {

///
/// \brief The UserData class
/// is a custom data, using for event sensor.
///
class SRC_API UserData : public Measure
{
  public:
    ///
    /// \brief UserData
    /// \param measure
    ///
    explicit UserData( const Measure& measure );
    ///
    /// \brief UserData
    /// \param name
    /// \param any
    ///
    UserData( const std::string& name, const Any& any );

    ///
    /// \brief getName
    /// \return
    ///
    const std::string& getName() const;
    ///
    /// \brief getValue
    /// \return
    ///
    const Any& getValue() const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param userData
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const UserData& userData );

  private:
    std::string m_name;
    Any m_value;
};

} // namespace data
} // namespace hub
