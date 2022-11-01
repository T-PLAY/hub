#ifndef POLHEMUS_POSITION_H_
#define POLHEMUS_POSITION_H_

namespace Polhemus {
class Position
{
  private:
    float x, y, z;

  public:
    Position( float x, float y, float z ) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float getXpos() const { return x; }

    float getYpos() const { return y; }

    float getZpos() const { return z; }
};
} // namespace Polhemus

#endif /* POLHEMUS_POSITION_H_ */
