#ifndef FORMINPUTSTREAMVIEW_H
#define FORMINPUTSTREAMVIEW_H

#include <QLabel>
#include <QStringListModel>
#include <QThread>
#include <QWidget>
#include <stream.h>

class FormInputStreamViews;

class InputStreamThread : public QThread {
    Q_OBJECT
public:
    // constructor

    //    template <class IOStreamT>
    //    InputStreamThread(IOStreamT&& iostream, QObject* parent = nullptr);
    InputStreamThread(std::unique_ptr<InputStream> inputStream, const std::string& sourceType, QObject* parent = nullptr);
    //    InputStreamThread(InputStream& iostream, FormInputStreamViews& formInputStreamViews, int iSensor, QObject* parent = nullptr);

    ~InputStreamThread();

signals:
    void newAcquisition(const std::string& sourceType);
    void streamingStopped(const std::string& sourceType);

public:
    // overriding the QThread's run() method
    void run();

    Stream::Acquisition mAcq;
    //    int mIStream;
    //    InputStream& mInputStream;
    std::unique_ptr<InputStream> mInputStream;
    std::string mSourceType;
    //    FormInputStreamViews& mFormInputStreamViews;

private:
};

// template <class IOStreamT>
// InputStreamThread::InputStreamThread(IOStreamT&& iostream, QObject* parent)
//     : mInputStream(std::make_unique<InputStream>(std::move(iostream)))
//{
// }

///////////////////////////////////////////////////////////////////////////////////

class CounterFpsThread : public QThread {
    Q_OBJECT
public:
    CounterFpsThread(QLabel& label);
    ~CounterFpsThread();
    // constructor
signals:

public:
    // overriding the QThread's run() method
    void run();

    size_t m_counterFrame = 0;

private:
    //    std::string m_sensorName;
    //    double m_fps = 10.0;
    QLabel& m_labelFps;
};

///////////////////////////////////////////////////////////////////////////////////

namespace Ui {
class FormInputStreamView;
}

class FormInputStreamView : public QWidget {
    Q_OBJECT

public:
    explicit FormInputStreamView(const std::string& sensorName, QWidget* parent = nullptr);
    ~FormInputStreamView();

    template <class IOStreamT>
    void add(const std::string streamName, IOStreamT&& iostream);

    void remove(const std::string& sourceType);

    const Stream::Acquisition& getAcquisition(const std::string& sourceType) const;
    const InputStreamThread& getIputStreamThread(const std::string& sourceType) const;

private slots:
    //    void on_comboBox_sourceType_currentTextChanged(const QString& sourceType);
    void onNewAcquisition(const std::string& sourceType);
    void onStreamingStopped(const std::string& sourceType);

signals:
    void newAcquisition(const std::string& sensorName, const std::string& sourceType);
    void init(const std::string& sensorName);

private:
    Ui::FormInputStreamView* ui;

    const std::string m_sensorName;
    //    InputStreamThread* m_activeThreadInputStreams = nullptr;
    //    std::string m_activeStreamNames = "";

    QStringListModel m_sourceTypeModel;

    //    Stream::Acquisition m_lastAcqs;
    CounterFpsThread* m_counterFpsThreads = nullptr;
    //    std::map<std::string, std::list<std::unique_ptr<InputStream>>> m_sourceType2inputStream;
    std::map<std::string, std::list<std::unique_ptr<InputStreamThread>>> m_sourceType2inputStreamThreads;
    //    std::set<std::string> m_streamsKilled;
};

template <class IOStreamT>
void FormInputStreamView::add(const std::string streamName, IOStreamT&& iostream)
{
    const auto sourceType = (m_sensorName.size() == streamName.size())
        ? ("physical")
        : (streamName.substr(m_sensorName.size() + 2,
            streamName.size() - 1 - m_sensorName.size() - 2));
    assert(sourceType == "" || sourceType == "physical" || sourceType == "record");

    std::cout << "[FormInputStreamView] add(" << streamName << ")" << std::endl;

    //    InputStream inputStream(std::move(iostream));
    //    m_sourceType2inputStream[sourceType].push_back(std::make_unique<InputStream>(std::move(iostream)));

    //    assert(m_sourceType2inputStreamThreads.find(sourceType) == m_sourceType2inputStreamThreads.end());
    auto& inputStreamThreads = m_sourceType2inputStreamThreads[sourceType];
    inputStreamThreads.push_back(std::make_unique<InputStreamThread>(std::make_unique<InputStream>(std::move(iostream)), sourceType));

    auto* inputStreamThread = inputStreamThreads.back().get();
    QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamView::onNewAcquisition);
    QObject::connect(inputStreamThread, &InputStreamThread::streamingStopped, this, &FormInputStreamView::onStreamingStopped);
    inputStreamThread->start();

    if (inputStreamThreads.size() == 1) {
        auto stringList = m_sourceTypeModel.stringList();
        stringList.append(sourceType.c_str());
        m_sourceTypeModel.setStringList(stringList);
    }
}

#endif // FORMINPUTSTREAMVIEW_H
