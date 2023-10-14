

#include <queue>
#include <string>

#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/Vector.hpp>


class InputOutput : public hub::Input, public hub::Output
{
  public:
    using hub::Input::read;
    using hub::Output::write;

    InputOutput() = default;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( !m_datas.empty() );
        auto vector = m_datas.front();
        m_datas.pop();
        assert( vector.size() == size );
//        std::cout << HEADER_INPUT_MSG "read(Data_t*, Size_t) : data = " << vector << std::endl;

        memcpy( data, vector.data(), size );
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        std::vector<hub::Data_t> vector( data, data + size );
//        std::cout << HEADER_OUTPUT_MSG "write(Data_t*, Size_t) : data = " << vector << std::endl;
        m_datas.push( vector );
    }

    void close() override {};

    bool isOpen() const override { return true; };

    bool isEnd() const override { return m_datas.empty(); }

    std::queue<std::vector<hub::Data_t>> m_datas;
};


int main() {

    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };

    InputOutput inputOutput;

    inputOutput.write(5);
    int v;
    inputOutput.read(v);
    assert(v == 5);
//    inputOutput.write(data1);

    assert( inputOutput.isEnd() );
}
