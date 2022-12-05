#ifndef POLHEMUS_ORIENTATION_H_
#define POLHEMUS_ORIENTATION_H_

namespace Polhemus {
class Orientation
{
  private:
    float azimuth, elevation, roll;

  public:
    Orientation( float azimuth, float elevation, float roll ) {
        this->azimuth   = azimuth;
        this->elevation = elevation;
        this->roll      = roll;
    }

    float getAzimuth() const { return azimuth; }

    float getElevation() const { return elevation; }

    float getRoll() const { return roll; }
};
} // namespace Polhemus

#endif /* POLHEMUS_ORIENTATION_H_ */
