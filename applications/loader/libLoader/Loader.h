
#pragma once

#include <string>
//#include <stream.h>
#include <future>

#include <QStringListModel>
#include <QObject>
#include <QItemSelection>
#include <QTreeView>
#include <QListView>

#include <Frame.h>
#include <IO/Stream.hpp>
#include <OutputSensor.hpp>
#include <IO/Ram.hpp>

class Loader : public QObject
{
        Q_OBJECT
public:
    Loader(const std::string & outputPostFixName = "", bool autoPlay = true);

    ~Loader();

    void load(const std::string & path);
    void update();
    void unload();

    void play();
    void stop();


signals:
    void pathLoaded();


//    void showFrame(int iFrame);

//    void setOutputPostFixName(const std::string & outputPostFixName);

public slots:
//    void onFrame_selectionChange(const QItemSelection &selected, const QItemSelection & deselected);
    void onFrame_selectionChange(const QModelIndexList &selectedRows);


private:

//    std::map<std::string, std::unique_ptr<hub::OutputSensor>> m_outputs;
    std::string m_outputPostfixName = "";

//    hub::InputSensor * m_inputStream = nullptr;
//    std::map<std::string, hub::InputSensor> m_inputStreams;
    std::vector<std::unique_ptr<hub::InputSensor>> m_inputStreams;

//    QListView & m_listViewFrame;
//    QTreeView & m_treeViewFile;

//    std::map<std::string, std::unique_ptr<hub::InputSensor>> m_inputStreams;
//    std::vector<hub::InputSensor> m_inputStreams;

    std::map<std::string, std::vector<hub::Acquisition>> m_sensorName2acquisitions;
    std::set<Snapshot> m_snapshots;
    std::vector<Frame> m_frames;
    std::string m_loadedPath = "";

    QStringListModel m_frameModel;

//    std::promise<void> m_exitSignal;
//    std::future<void> m_futureObj;
    std::thread * m_thread = nullptr;
    bool m_isPlaying = false;
    bool m_autoPlay;

//    bool m_exitSignal = false;
//    int m_currentFrame = -1;
    std::map<std::string, std::unique_ptr<hub::OutputSensor>> m_outputStreams;
    std::map<std::string, std::unique_ptr<hub::io::CyclicBuff>> m_outputStreamBuffs;

    size_t iFrame = 0;

public:
//    const std::vector<Frame> & getFrames() const;
    bool isPlaying() const;
    bool isLoaded() const;
    const std::string & getLoadedPath() const;
//    int getCurrentFrame() const;
    QStringListModel & getFrameModel();
//    const std::vector<hub::InputSensor> &getInputStreams() const;

//    const std::vector<std::unique_ptr<hub::InputSensor>> &getInputStreams() const;
//    std::vector<std::unique_ptr<hub::InputSensor>> getInputStreams();
//    const std::map<std::string, std::unique_ptr<hub::OutputSensor>> & getRamOutputStreams() const;
    const std::map<std::string, std::unique_ptr<hub::io::CyclicBuff>> &getOutputStreamBuffs() const;
    const std::vector<hub::Acquisition> &getAcquisitions(const std::string & sensorName) const;
};

