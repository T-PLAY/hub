
#include <queue>
#include <mutex>

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/Vector.hpp>


//class InputOutput : public hub::Input, public hub::Output
//{
//  public:
//    using hub::Input::read;
////    using hub::Input::operator>>;
//    using hub::Output::write;
////    using hub::Output::operator<<;

//    InputOutput() = default;

//    ~InputOutput() { std::cout << "[InputOutput] ~InputOutput()" << std::endl; }

//    void read( hub::Data_t* data, hub::Size_t size ) override {
//        m_mtxDatas.lock();
//        assert( !m_datas.empty() );
//        const auto& vector = m_datas.front();
//#ifdef HUB_DEBUG_INPUT
//        std::cout << HEADER << "read(Data_t*, Size_t) : data = " << vector << ", read size = " << size << std::endl;
//#endif
//        assert( vector.size() == size );

//        memcpy( data, vector.data(), size );
//        m_datas.pop();
//        m_mtxDatas.unlock();
//    }

//    void write( const hub::Data_t* data, hub::Size_t size ) override {
//        m_mtxDatas.lock();
//        std::vector<hub::Data_t> vector( data, data + size );
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
//#endif
//        m_datas.push( std::move( vector ) );
//        m_mtxDatas.unlock();
//    }

//    void close() override {};
//    //    void close() override { assert( isOpen() );
//    //        m_isOpen = false;
//    //    };

//    bool isOpen() const override { return true; }
//    //    bool isOpen() const override { return m_isOpen; };

//    bool isEnd() const override {
//        //        m_mtxDatas.lock();
//        //        auto ret = m_datas.empty();
//        //        m_mtxDatas.unlock();
//        //        return ret;
//        return m_datas.empty();
//    }

//    void clear() override {
//        while ( !m_datas.empty() )
//            m_datas.pop();
//    }

//    //    template <class T>
//    ////    constexpr typename std::enable_if<getable_v<T>, T>::type get()
//    //    constexpr T get()
//    //    {
//    ////        const auto & vector = m_datas.front();
//    ////        std::cout << HEADER_INPUT_MSG "read(Data_t*, Size_t) : data = " << vector <<
//    ///std::endl; /        assert( vector.size() == size );

//    ////        memcpy( data, vector.data(), size );
//    ////        return
//    ////        return get<T>();
//    //        return T::get(*this);
//    ////            T t = T::get(*this);
//    ////        T t;
//    ////        return t;
//    //    }

//  private:
//    mutable std::mutex m_mtxDatas;
//    std::queue<std::vector<hub::Data_t>> m_datas;
//    //    bool m_isOpen = true;
//};