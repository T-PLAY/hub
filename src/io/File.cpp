#include "io/File.hpp"

#include <cassert>
#include <iostream>
#include <memory>

namespace hub {
namespace io {

//File::File( std::fstream&& file ) : m_file( new std::fstream( std::move( file ) ) ) {
////File::File( std::fstream&& file ) {
////File::File( std::fstream&& file ) : m_file(std::move(file)) {
////File::File( std::fstream&& file ) : m_file( std::make_unique<std::fstream>(std::move(file)) ) {

//    assert( m_file->is_open() );
//}

//File::File(std::basic_ios<char> &&file)
//    :m_file(std::make_unique<std::basic_ios<char>>(std::move(file)))
//{

//}

File::File(const std::string &filePath)
    : m_filePath(filePath)
{

}

File::File(const char *filePath)
    : m_filePath(filePath)
{

}

//File::File( File&& file ) : m_file( std::move( file.m_file ) ) {
//    file.m_file = nullptr;
//}

//File::~File() {
//    if ( m_file != nullptr ) {
//        std::cout << "[File:" << this << "] ~File() started" << std::endl;
        // todo fix this bug
//        File::close();
//        assert( !File::isOpen() );
//        std::cout << "[File:" << this << "] ~File() close file" << std::endl;
//        std::cout << "[File:" << this << "] ~File() ended" << std::endl;
//    }
//}


} // namespace io
} // namespace hub
