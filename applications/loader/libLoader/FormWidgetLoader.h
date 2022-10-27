#ifndef FORMWIDGETLOADER_H
#define FORMWIDGETLOADER_H

#include <Loader.h>
#include <QFileSystemModel>
#include <QItemSelection>
#include <QStringListModel>
#include <QWidget>

namespace Ui {
class FormWidgetLoader;
}

class FormWidgetLoader : public QWidget
{
    Q_OBJECT

  public:
    FormWidgetLoader( const std::string & ipv4, const int & port, QWidget* parent = nullptr );
    ~FormWidgetLoader();

    // public slots:
    const Loader& getRecordLoader() const;
    const Loader& getSnapshotLoader() const;

  signals:
//    void recordPathLoaded();
//    void recordPathUnloaded();

  private slots:
    void onAcqChanged(int iAcq);
    void onSpace();
    void onPlayEnded();

//    void onTreeView_record_currentChanged( const QModelIndex& current,
//                                           const QModelIndex& previous );
    void onTreeView_record_selectionChanged( const QItemSelection& selected,
                                             const QItemSelection& deselected );

    void on_checkBox_autoLoop_toggled(bool checked);
    void on_pushButton_playPause_clicked();
    void on_horizontalSlider_iAcq_valueChanged(int value);
    void on_spinBox_iAcq_valueChanged(int arg1);

private:
    Ui::FormWidgetLoader* ui;

    QFileSystemModel* m_recordFileModel = nullptr;
    Loader m_recordLoader;

};

#endif // FORMWIDGETLOADER_H
