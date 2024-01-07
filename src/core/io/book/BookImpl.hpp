#pragma once

// #include <vector>

// #include "core/Macros.hpp"
// //#include "core/io/InputOutputBase.hpp"
// //#include "core/io/InputOutputImpl.hpp"
// #include "core/io/InputOutput.hpp"
// //#include "core/io/BasicInputOutputI.hpp"
// #include "core/serializer/SerializerImpl.hpp"


// namespace hub {
// namespace io {
// namespace book {

// //template <Size_t NumberOfCharacter = 1'000'000>
// //          class Characters         = std::array<unsigned char, NumberOfCharacter>>
// //class BookImpl : public InputOutputBase
// template <Size_t NumberOfCharacter = MAX_STACK_SIZE, class Characters = std::array<unsigned char, NumberOfCharacter>, class InputOutputT = InputOutput<serializer::SerializerImpl>>
// //template <Size_t NumberOfCharacter = 1'000'000, class Characters = std::array<unsigned char, NumberOfCharacter>, class InputOutputT = InputOutput<serializer::SerializerZppBits>>
// //class BookImpl : public InputOutputImpl
// class BookImpl : public InputOutputT
// //class BookImpl : public BasicInputOutputI
// {
//   public:
//     using InputOutputT::read;
//     using InputOutputT::write;

//     BookImpl( Characters& characters ) : m_characters( characters ), m_data{m_characters.data()} {}
//     BookImpl() : m_characters { *new Characters }, m_data{m_characters.data()} {}
// //    BookImpl() : m_characters { 0 }, m_data( m_characters.data() ) {}

//     void read( hub::Data_t* data, hub::Size_t size ) override {
//         memcpy( data, m_data + m_iRead, size );
//         m_iRead += size;
// //#ifdef HUB_DEBUG_INPUT
// //        std::cout << "\t" << HEADER << "read(Data_t*, Size_t = " << size << ")"  << std::endl;
// //#endif
//     }

//     void write( const hub::Data_t* data, hub::Size_t size ) override {
// //#ifdef HUB_DEBUG_INPUT
// //        std::cout << "\t" << HEADER << "write(Data_t*, Size_t = " << size << ")"  << std::endl;
// //#endif
//         memcpy( m_data + m_iWrite, data, size );
//         m_iWrite += size;
//     }

//     void close() override {};

//     bool isOpen() const override { return true; }

//     bool isEnd() const override { return m_iRead == m_iWrite; }

//     void clear() override {}

//     void setRetain(bool retained) override {}

//   private:
// //    size_t m_size;
//     Characters& m_characters;
// //    std::array<unsigned char, NumberOfCharacter> m_characters;
//     unsigned char* m_data = nullptr;
//     size_t m_iRead         = 0;
//     size_t m_iWrite        = 0;
// };

// } // namespace book
// } // namespace io
// } // namespace hub
