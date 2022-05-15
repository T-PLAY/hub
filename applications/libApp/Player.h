#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <stream.h>
#include <future>


struct Snapshot {
    Stream::Acquisition mAcq;
    std::string mSensorName;

    bool operator<(const Snapshot& snapshot) const
    {
        return mAcq.mBackendTimestamp < snapshot.mAcq.mBackendTimestamp;
    }
    bool operator==(const Snapshot& snapshot) const
    {
        return mSensorName == snapshot.mSensorName && mAcq.mBackendTimestamp == snapshot.mAcq.mBackendTimestamp;
    }
};

struct Frame {
//    std::vector<Snapshot> m_snapshots;
    std::vector<const Snapshot*> m_snapshots;
};


class Player
{
public:
//    Player();
    ~Player();

    void load(const std::string & path);
    void unload();

    void play();
    void stop();

    void showFrame(int iFrame);



private:

    std::map<std::string, std::unique_ptr<OutputStream>> m_outputs;
    std::set<Snapshot> m_snapshots;
    std::vector<Frame> m_frames;
    std::string m_loadedPath = "";

//    std::promise<void> m_exitSignal;
//    std::future<void> m_futureObj;
    std::thread * m_thread = nullptr;
    bool m_isPlaying = false;
//    bool m_exitSignal = false;
    int m_currentFrame = -1;

public:
    const std::vector<Frame> & getFrames() const;
    bool isPlaying() const;
    bool isLoaded() const;
    const std::string & getLoadedPath() const;
    int getCurrentFrame() const;
};

#endif // PLAYER_H
