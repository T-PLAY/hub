
#pragma once

#include "core/Base.hpp"

namespace hub {
namespace server {

class SRC_API ServerI
{
public:

    virtual void run()   = 0;
    virtual void asyncRun()   = 0;
    virtual void stop()  = 0;

    virtual void printStatus() const  = 0;
    virtual bool running() const  = 0;
    virtual int nStreamer() const  = 0;
    virtual int nClient() const  = 0;


};

} // namespace server
} // namespace hub
