#ifndef RECORDER_H
#define RECORDER_H

#include <string>
#include <vector>
#include <utility>
#include <thread>

#include <Frame.h>

using InputStreamParameters = std::vector<std::pair<std::string, std::string>>;

class Recorder
{
public:
//    Recorder(const std::string & rootPath);
//    Recorder(std::string rootPath);
    Recorder(const char * rootPath);

    void record(const InputStreamParameters & inputStreamConfigs);
    void stop();

    void save(const Frame & frame);

private:
    std::string m_rootPath = "";
    std::thread * m_thread = nullptr;

    bool m_isRecording = false;

public:
    bool isRecording() const;
};

#endif // RECORDER_H
