

#include <io/output/Output.hpp>

#include <io/output/OutputFile.hpp>
#include <io/output/OutputMemory.hpp>
#include <io/output/OutputStream.hpp>
//#include <io/output/OutputStreamInterface.hpp>
#include <io/output/OutputStreamServer.hpp>



int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;


//    const hub::Output & output = hub::Output(); // abstract class
    const hub::Output & output2 = hub::output::OutputStream("streamName", ipv4, port);
    const hub::Output & output3 = hub::output::OutputStreamServer("streamName", ipv4, port);
//    const hub::Output & output4 = hub::output::OutputStreamInterface("streamName", ipv4, port); // abstract class
    const hub::Output & output5 = hub::output::OutputFile("filePath");
    std::vector<int> vs;
#if CPLUSPLUS_VERSION < 17
    const hub::Output & output6 = hub::output::OutputMemory<decltype(vs)>(vs);
#else
    const hub::Output & output6 = hub::output::OutputMemory(vs);
#endif


}
