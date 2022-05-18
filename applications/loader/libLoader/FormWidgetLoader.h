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

    const Loader &getRecordLoader() const;
    const Loader &getSnapshotLoader() const;

signals:
//    void recordFrames_selectionChanged();
//    void snapshotFrames_selectionChanged();

    void recordPathLoaded();
    void snapshotPathLoaded();



private slots:
    void on_treeView_record_clicked(const QModelIndex &index);
    void on_treeView_snapshot_clicked(const QModelIndex &index);

    void on_treeView_record_doubleClicked(const QModelIndex &index);
    void on_treeView_snapshot_doubleClicked(const QModelIndex &index);

//    void on_treeView_record_keyPressed(QKeyEvent * event);

//    void updateRecordFrames();
//    void updateSnapshotFrames();

//    void on_listView_recordFrames_clicked(const QModelIndex &index);
//    void on_listView_snapshotFrames_clicked(const QModelIndex &index);

    void onRecordFrames_selectionChange(const QItemSelection &selected, const QItemSelection & deselected);
    void onSnapshotFrames_selectionChange(const QItemSelection &selected, const QItemSelection & deselected);

    void onRecordFrames_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void onSnapshotFrames_currentChanged(const QModelIndex &current, const QModelIndex &previous);

//    void on_listView_frames_activated(const QModelIndex &index);
//    void on_listView_frames_indexesMoved(const QModelIndexList &indexes);


//    void on_listView_recordFrames_indexesMoved(const QModelIndexList &indexes);


    void on_treeView_record_pressed(const QModelIndex &index);

    void on_treeView_record_activated(const QModelIndex &index);

    void on_treeView_record_entered(const QModelIndex &index);

    void on_treeView_record_viewportEntered();


private:
    Ui::FormWidgetLoader *ui;

    QFileSystemModel * m_recordFileModel = nullptr;
//    QStringListModel * m_recordFrameModel = nullptr;
    Loader m_recordLoader;
//    std::vector<Frame> m_selectedRecordFrames;
//    OutputStream * m_recordOutputStream = nullptr;

    QFileSystemModel * m_snapshotFileModel = nullptr;
//    QStringListModel * m_snapshotFrameModel = nullptr;
    Loader m_snapshotLoader;
//    std::vector<Frame> m_selectedSnapshotFrames;
//    OutputStream * m_snapshotOutputStream = nullptr;

//    std::string m_frameNameSelected = "";

//    bool m_autoPlay = false;
//    Player m_recordPlayer;
//    Player m_snapShotPlayer;
//    Player * m_currentPlayer = nullptr;
};

#endif // FORMWIDGETLOADER_H
