#ifndef HUBV2_RECORDER_H
#define HUBV2_RECORDER_H

#include <string>
#include <vector>
#include <QDir>
#include <fstream>

#include "../external/toojpeg.h"

class Recorder {
private:


public:
    Recorder() = default;

    bool startSession(const std::string& folder, const std::string& name, int rate);
    void stopSession();

    void record(const std::vector<unsigned char>& image, const std::string& position);

    std::string & message();

private:
    bool _sessionOn = false;
    int _rate;

    std::string _message = "Recording session is off";
    QDir _sessionDir;

    std::vector<std::vector<unsigned char>> _imageBuffer;
    std::vector<std::string> _positionBuffer;


    inline static std::ofstream file;

    static void output(unsigned char byte) {
        file << byte;
    };

};

#endif //HUBV2_RECORDER_H
