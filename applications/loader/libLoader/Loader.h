
#pragma once

#include <string>
#include <stream.h>
#include <future>

#include <Frame.h>

#include <QStringListModel>
#include <QObject>
#include <RamIO.h>

#include <QItemSelection>

#include <QTreeView>
#include <QListView>

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

//    std::map<std::string, std::unique_ptr<OutputStream>> m_outputs;
    std::string m_outputPostfixName = "";

//    InputStream * m_inputStream = nullptr;
//    std::map<std::string, InputStream> m_inputStreams;
    std::vector<std::unique_ptr<InputStream>> m_inputStreams;

//    QListView & m_listViewFrame;
//    QTreeView & m_treeViewFile;

//    std::map<std::string, std::unique_ptr<InputStream>> m_inputStreams;
//    std::vector<InputStream> m_inputStreams;

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
    std::map<std::string, std::unique_ptr<OutputStream>> m_outputStreams;
    std::map<std::string, std::unique_ptr<CyclicBuff>> m_outputStreamBuffs;

public:
//    const std::vector<Frame> & getFrames() const;
    bool isPlaying() const;
    bool isLoaded() const;
    const std::string & getLoadedPath() const;
//    int getCurrentFrame() const;
    QStringListModel & getFrameModel();
//    const std::vector<InputStream> &getInputStreams() const;

//    const std::vector<std::unique_ptr<InputStream>> &getInputStreams() const;
//    std::vector<std::unique_ptr<InputStream>> getInputStreams();
//    const std::map<std::string, std::unique_ptr<OutputStream>> & getRamOutputStreams() const;
    const std::map<std::string, std::unique_ptr<CyclicBuff>> &getOutputStreamBuffs() const;
};

