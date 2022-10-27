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
    explicit FormWidgetLoader( QWidget* parent = nullptr );
    ~FormWidgetLoader();

    // public slots:
    const Loader& getRecordLoader() const;
    const Loader& getSnapshotLoader() const;

  signals:
//    void recordPathLoaded();
//    void recordPathUnloaded();

  private slots:

//    void onTreeView_record_currentChanged( const QModelIndex& current,
//                                           const QModelIndex& previous );
    void onTreeView_record_selectionChanged( const QItemSelection& selected,
                                             const QItemSelection& deselected );

  private:
    Ui::FormWidgetLoader* ui;

    QFileSystemModel* m_recordFileModel = nullptr;
    Loader m_recordLoader;
};

#endif // FORMWIDGETLOADER_H
