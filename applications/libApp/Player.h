#ifndef PLAYER_H
#define PLAYER_H

//#include <string>
//#include <future>
//#include <set>

//#include <Frame.h>
//#include <OutputSensor.hpp>

//class Player
//{
//public:
//    Player(const std::string & outputPostFixName = "");
//    ~Player();

//    void load(const std::string & path, int iSensor = -1);
//    void update(int iSensor);
//    void unload();

//    void play();
//    void stop();

//    void showFrame(int iFrame);

//    void setOutputPostFixName(const std::string & outputPostFixName);


//private:

//    std::map<std::string, std::unique_ptr<hub::OutputSensor>> m_outputs;
//    std::string m_outputPostfixName = "";

//    std::set<Snapshot> m_snapshots;
//    std::vector<Frame> m_frames;
//    std::string m_loadedPath = "";

////    std::promise<void> m_exitSignal;
////    std::future<void> m_futureObj;
//    std::thread * m_thread = nullptr;
//    bool m_isPlaying = false;
////    bool m_exitSignal = false;
//    int m_currentFrame = -1;

//public:
//    const std::vector<Frame> & getFrames() const;
//    bool isPlaying() const;
//    bool isLoaded() const;
//    const std::string & getLoadedPath() const;
//    int getCurrentFrame() const;
//};

#endif // PLAYER_H
