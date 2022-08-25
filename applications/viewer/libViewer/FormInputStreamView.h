#ifndef FORMINPUTSTREAMVIEW_H
#define FORMINPUTSTREAMVIEW_H

//#include <stack>
#include <queue>

#include <QLabel>
#include <QStringListModel>
#include <QThread>
#include <QWidget>
#include <QMdiArea>
#include <WidgetStreamView.h>
#include <QMdiSubWindow>
#include <constants.h>

#include <InputSensor.hpp>

class FormInputStreamViews;

class InputStreamThread : public QThread {
    Q_OBJECT
public:
    // constructor

    //    template <class IOStreamT>
    //    InputStreamThread(IOStreamT&& iostream, QObject* parent = nullptr);
    InputStreamThread(std::unique_ptr<hub::InputSensor> inputStream, const std::string& sourceType, QObject* parent = nullptr);
    //    InputStreamThread(hub::InputSensor& iostream, FormInputStreamViews& formInputStreamViews, int iSensor, QObject* parent = nullptr);

    ~InputStreamThread();

signals:
    void newAcquisition(const std::string& sourceType);
    void streamingStopped(const std::string& sourceType);

public:
    // overriding the QThread's run() method
    void run();

    //    Stream::hub::Acquisition mAcq;
    std::queue<hub::Acquisition> mAcqs;

    //    int mIStream;
    //    hub::InputSensor& mInputStream;
    std::unique_ptr<hub::InputSensor> mInputStream;
    std::string mSourceType;
    //    FormInputStreamViews& mFormInputStreamViews;

private:
};

// template <class IOStreamT>
// InputStreamThread::InputStreamThread(IOStreamT&& iostream, QObject* parent)
//     : mInputStream(std::make_unique<hub::InputSensor>(std::move(iostream)))
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

using Acquisitions = std::queue<hub::Acquisition>;

class FormInputStreamView : public QWidget {
    Q_OBJECT

public:
    explicit FormInputStreamView(const std::string& sensorName, QMdiArea& mdiArea, QWidget* parent = nullptr);
    ~FormInputStreamView();

    template <class IOStreamT>
    void add(const std::string streamName, IOStreamT&& iostream);

    void remove(const std::string& sourceType);

    //    const Stream::hub::Acquisition& getAcquisition(const std::string& sourceType) const;
    Acquisitions& getAcquisitions(const std::string& sourceType);
    const InputStreamThread& getIputStreamThread(const std::string& sourceType) const;

    const hub::InputSensor& getInputStream(const std::string& sourceType) const;

private slots:
    //    void on_comboBox_sourceType_currentTextChanged(const QString& sourceType);
    void onNewAcquisition(const std::string& sourceType);
    void onStreamingStopped(const std::string& sourceType);

    void on_comboBox_sourceType_currentTextChanged(const QString& sourceType);

signals:
    void newAcquisition(const std::string& sensorName, const std::string& sourceType);
    void init(const std::string& sensorName);

    void isEmpty(const std::string& sensorName);

    void selectedSourceChanged(const std::string& sensorName, const std::string& sourceType);

private:
    Ui::FormInputStreamView* ui;

    const std::string m_sensorName;
    //    InputStreamThread* m_activeThreadInputStreams = nullptr;
    //    std::string m_activeStreamNames = "";

    QStringListModel m_sourceTypeModel;

    //    Stream::hub::Acquisition m_lastAcqs;
    CounterFpsThread* m_counterFpsThreads = nullptr;
    //    std::map<std::string, std::list<std::unique_ptr<hub::InputSensor>>> m_sourceType2inputStream;
    std::map<std::string, std::list<std::unique_ptr<InputStreamThread>>> m_sourceType2inputStreamThreads;
    //    std::set<std::string> m_streamsKilled;

    WidgetStreamView* m_widgetStreamView = nullptr;
    QMdiArea& m_mdiArea;
};

template <class IOStreamT>
void FormInputStreamView::add(const std::string streamName, IOStreamT&& iostream)
{
    const auto sourceType = (m_sensorName.size() == streamName.size())
        ? ("physical")
        : (streamName.substr(m_sensorName.size() + 2,
            streamName.size() - 1 - m_sensorName.size() - 2));
    assert(sourceType == "" || sourceType == "physical" || sourceType == "record" || sourceType == "simulator");

    std::cout << "[FormInputStreamView] add(" << streamName << ", " << sourceType << ")" << std::endl;

    //    hub::InputSensor inputStream(std::move(iostream));
    //    m_sourceType2inputStream[sourceType].push_back(std::make_unique<hub::InputSensor>(std::move(iostream)));

    //    assert(m_sourceType2inputStreamThreads.find(sourceType) == m_sourceType2inputStreamThreads.end());
    auto& inputStreamThreads = m_sourceType2inputStreamThreads[sourceType];
    inputStreamThreads.push_back(std::make_unique<InputStreamThread>(std::make_unique<hub::InputSensor>(std::move(iostream)), sourceType));

    auto* inputStreamThread = inputStreamThreads.back().get();
    QObject::connect(inputStreamThread, &InputStreamThread::newAcquisition, this, &FormInputStreamView::onNewAcquisition);
    QObject::connect(inputStreamThread, &InputStreamThread::streamingStopped, this, &FormInputStreamView::onStreamingStopped);
    inputStreamThread->start();

    auto stringList = m_sourceTypeModel.stringList();
    stringList.append(sourceType.c_str());
    m_sourceTypeModel.setStringList(stringList);

    if (m_sourceType2inputStreamThreads.size() == 1) {

        assert(m_widgetStreamView == nullptr);

        const auto& inputStream = inputStreamThread->mInputStream;
        const auto& header = inputStream->m_spec;
        const auto& dims = header.m_dims;
        if (dims.size() == 1) {
            m_widgetStreamView = new WidgetStreamView1D(this);
            m_widgetStreamView->setMinimumSize(400, 35);

        } else if (dims.size() == 2) {
            m_widgetStreamView = new WidgetStreamView2D(this);
            m_widgetStreamView->setMinimumWidth(dims.at(0));
            m_widgetStreamView->setMinimumHeight(dims.at(1));
        }
        //        if (m_widgetStreamView != nullptr) {
        assert(m_widgetStreamView != nullptr);

        auto* subWindow = m_mdiArea.addSubWindow(m_widgetStreamView);
        subWindow->setVisible(true);
        subWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowTitleHint);

        std::string dimText = "(dim: ";
        for (int i = 0; i < dims.size(); ++i) {
            dimText += std::to_string(dims.at(i));
            if (i != dims.size() - 1) {
                dimText += " x ";
            }
        }
        dimText += ")";

        std::string formatText = std::string("(format: ") + hub::SensorSpec::format2string(header.m_format) + ")";
        subWindow->setWindowTitle((m_sensorName + "   " + dimText + "   " + formatText).c_str());
        //        }
    }
}

#endif // FORMINPUTSTREAMVIEW_H
