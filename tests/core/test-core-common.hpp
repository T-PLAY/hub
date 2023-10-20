
#include <queue>

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/Vector.hpp>

class InputOutput : public hub::Input, public hub::Output
{
  public:
    using hub::Input::read;
    using hub::Input::operator>>;
    using hub::Output::write;
    using hub::Output::operator<<;

    InputOutput() = default;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        m_mtxDatas.lock();
        assert( !m_datas.empty() );
        auto vector = m_datas.front();
        m_datas.pop();
        assert( vector.size() == size );
        //        std::cout << HEADER_INPUT_MSG "read(Data_t*, Size_t) : data = " << vector <<
        //        std::endl;

        memcpy( data, vector.data(), size );
        m_mtxDatas.unlock();
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        m_mtxDatas.lock();
        std::vector<hub::Data_t> vector( data, data + size );
        //        std::cout << HEADER_OUTPUT_MSG "write(Data_t*, Size_t) : data = " << vector <<
        //        std::endl;
        m_datas.push( vector );
        //        std::cout << "write datas size = " << m_datas.size() << std::endl;
        m_mtxDatas.unlock();
    }

    void close() override {};
    //    void close() override { assert( isOpen() );
    //        m_isOpen = false;
    //    };

    bool isOpen() const override { return true; }
    //    bool isOpen() const override { return m_isOpen; };

    bool isEmpty() const override {
        //        m_mtxDatas.lock();
        //        auto ret = m_datas.empty();
        //        m_mtxDatas.unlock();
        //        return ret;
        return m_datas.empty();
    }

    void clear() override {
        while ( !m_datas.empty() )
            m_datas.pop();
    }

  private:
    mutable std::mutex m_mtxDatas;
    std::queue<std::vector<hub::Data_t>> m_datas;
    //    bool m_isOpen = true;
};
