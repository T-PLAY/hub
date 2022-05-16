#ifndef FORMWIDGETLOADER_H
#define FORMWIDGETLOADER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QStringListModel>
//#include <Player.h>
#include <Loader.h>
#include <QItemSelection>

namespace Ui {
class FormWidgetLoader;
}

class FormWidgetLoader : public QWidget
{
    Q_OBJECT

public:
    explicit FormWidgetLoader(QWidget *parent = nullptr);
    ~FormWidgetLoader();

//public slots:

signals:
    void recordFrames_selectionChanged();
    void snapshotFrames_selectionChanged();


private slots:
    void on_treeView_record_clicked(const QModelIndex &index);
    void on_treeView_snapshot_clicked(const QModelIndex &index);

//    void updateRecordFrames();
//    void updateSnapshotFrames();

//    void on_listView_recordFrames_clicked(const QModelIndex &index);
//    void on_listView_snapshotFrames_clicked(const QModelIndex &index);

    void recordFrames_selectionChange(const QItemSelection &selected, const QItemSelection & deselected);
    void snapshotFrames_selectionChange(const QItemSelection &selected, const QItemSelection & deselected);
//    void on_listView_frames_activated(const QModelIndex &index);
//    void on_listView_frames_indexesMoved(const QModelIndexList &indexes);


private:
    Ui::FormWidgetLoader *ui;

    QFileSystemModel * m_recordFileModel = nullptr;
//    QStringListModel * m_recordFrameModel = nullptr;
    Loader m_recordLoader;
    std::vector<Frame> m_selectedRecordFrames;

    QFileSystemModel * m_snapshotFileModel = nullptr;
//    QStringListModel * m_snapshotFrameModel = nullptr;
    Loader m_snapshotLoader;
    std::vector<Frame> m_selectedSnapshotFrames;

//    std::string m_frameNameSelected = "";

//    bool m_autoPlay = false;
//    Player m_recordPlayer;
//    Player m_snapShotPlayer;
//    Player * m_currentPlayer = nullptr;
};

#endif // FORMWIDGETLOADER_H
