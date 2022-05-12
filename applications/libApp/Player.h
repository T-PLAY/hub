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
};


class Player
{
public:
//    Player();

    void load(const std::string & path);
    void unload();

    void play();
    void stop();


private:

    std::map<std::string, std::unique_ptr<OutputStream>> m_outputs;
    std::set<Snapshot> m_snapshots;
    std::string m_loadedPath = "";

//    std::promise<void> m_exitSignal;
//    std::future<void> m_futureObj;
    std::thread m_thread;
    bool m_isPlaying = false;
//    bool m_exitSignal = false;

public:
    const std::string & getLoadedPath() const;
};

#endif // PLAYER_H
