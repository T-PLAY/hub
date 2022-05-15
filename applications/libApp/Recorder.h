#ifndef RECORDER_H
#define RECORDER_H

#include <string>
#include <vector>
#include <utility>
#include <thread>

using InputStreamConfigs = std::vector<std::pair<std::string, std::string>>;

class Recorder
{
public:
//    Recorder(const std::string & rootPath);
//    Recorder(std::string rootPath);
    Recorder(const char * rootPath);

    void record(const InputStreamConfigs & inputStreamConfigs);
    void stop();

private:
    std::string m_rootPath = "";
    std::thread * m_thread = nullptr;

    bool m_isRecording = false;
};

#endif // RECORDER_H
