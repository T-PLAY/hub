#include "FormWidgetLoader.h"
#include "ui_FormWidgetLoader.h"

#include <filesystem>
#include <iostream>

#include <QInputDialog>
#include <QItemSelectionModel>

FormWidgetLoader::FormWidgetLoader(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormWidgetLoader)
    ,
    //    , m_recordPlayer(" (record)")
    //    , m_snapShotPlayer(" (snapshot)")
    //    ,
    m_recordLoader(" (record)")
    , m_snapshotLoader(" (snapshot)", false)
{
    ui->setupUi(this);

    //    ui->treeView_record->setLoader(&m_recordLoader);
    //    ui->treeView_snapshot->setLoader(&m_snapshotLoader);

    // records view
    {
        // tree view
        QString recordPath = PROJECT_DIR "data/records/";
        if(! std::filesystem::exists(recordPath.toStdString())) {
            std::filesystem::create_directory(recordPath.toStdString());
        }
        assert(std::filesystem::exists(recordPath.toStdString()));
        m_recordFileModel = new QFileSystemModel(this);
        m_recordFileModel->setReadOnly(true);
        // Set filter
        m_recordFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
        //    m_recordFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
        // QFileSystemModel requires root path
        m_recordFileModel->setRootPath(recordPath);
        m_recordFileModel->sort(0, Qt::SortOrder::DescendingOrder);

        // Attach the model to the view
        ui->treeView_record->setModel(m_recordFileModel);
        const auto& rootIndex = m_recordFileModel->index(recordPath);
        ui->treeView_record->setRootIndex(rootIndex);
        ui->treeView_record->expand(rootIndex);
        ui->treeView_record->setColumnHidden(1, true);
        ui->treeView_record->setColumnHidden(2, true);
        ui->treeView_record->setColumnHidden(3, true);
        //    ui->treeView->setItemsExpandable(false);
        //    ui->treeView->setColumnHidden(0, true);

        QObject::connect(ui->treeView_record->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &FormWidgetLoader::onTreeView_record_currentChanged);

        QObject::connect(ui->treeView_record->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &FormWidgetLoader::onTreeView_record_selectionChanged);

        //        QObject::connect(ui->treeView_record,
        //            &QWidget::event,
        //            this,
        //            &FormWidgetLoader::onTreeView_record_keyPressed);
        // frame view
        //        m_recordFrameModel = new QStringListModel( this );
        //    ui->tableView_acqs->setModel(m_recordFrameModel);
        //        ui->listView_recordFrames->setModel( m_recordFrameModel );
        ui->listView_recordFrames->setModel(&m_recordLoader.getFrameModel());
        ui->listView_recordFrames->selectionModel();
        QObject::connect(ui->listView_recordFrames->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &FormWidgetLoader::onRecordFrames_selectionChange);

        QObject::connect(ui->listView_recordFrames->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &FormWidgetLoader::onRecordFrames_currentChanged);

        //                          &m_recordLoader,
        //                          &Loader::onFrame_selectionChange );

        //                connect( ui->listView_recordFrames->selectionModel(),
        //                         SIGNAL( currentRowChanged( QModelIndex, QModelIndex ) ),
        //                         this,
        //                         SLOT( on_listView_recordFrames_selectionChanged( QModelIndex,
        //                         QModelIndex ) ) );
    }

    // snapshots view
    {
        // tree view
        QString snapshotPath = PROJECT_DIR "data/snapshots/";
        if (! std::filesystem::exists(snapshotPath.toStdString())) {
            std::filesystem::create_directory(snapshotPath.toStdString());
        }
        assert(std::filesystem::exists(snapshotPath.toStdString()));
        m_snapshotFileModel = new QFileSystemModel(this);
        m_snapshotFileModel->setReadOnly(true);
        // Set filter
        m_snapshotFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
        //    m_snapshotFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
        // QFileSystemModel requires root path
        m_snapshotFileModel->setRootPath(snapshotPath);

        // Attach the model to the view
        ui->treeView_snapshot->setModel(m_snapshotFileModel);
        const auto& rootIndex2 = m_snapshotFileModel->index(snapshotPath);
        ui->treeView_snapshot->setRootIndex(rootIndex2);
        ui->treeView_snapshot->expand(rootIndex2);
        ui->treeView_snapshot->setColumnHidden(1, true);
        ui->treeView_snapshot->setColumnHidden(2, true);
        ui->treeView_snapshot->setColumnHidden(3, true);
        //    ui->treeView->setItemsExpandable(false);
        //    ui->treeView->setColumnHidden(0, true);

        QObject::connect(ui->treeView_snapshot->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &FormWidgetLoader::onTreeView_snapshot_currentChanged);

        // frame view
        //        m_snapshotFrameModel = new QStringListModel( this );
        //        ui->listView_snapshotFrames->setModel( m_snapshotFrameModel );
        ui->listView_snapshotFrames->setModel(&m_snapshotLoader.getFrameModel());
        //        connect( ui->listView_snapshotFrames->selectionModel(),
        //                 SIGNAL( currentRowChanged( QModelIndex, QModelIndex ) ),
        //                 this,
        //                 SLOT( on_listView_snapshotFrames_selectionChanged( QModelIndex,
        //                 QModelIndex ) ) );
        QObject::connect(ui->listView_snapshotFrames->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &FormWidgetLoader::onSnapshotFrames_selectionChange);
        //                          &m_snapshotLoader,
        QObject::connect(ui->listView_snapshotFrames->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &FormWidgetLoader::onSnapshotFrames_currentChanged);
        //                          &Loader::onFrame_selectionChange );
    }

    QObject::connect(
        &m_recordLoader, &Loader::pathLoaded, this, &FormWidgetLoader::recordPathLoaded);
    QObject::connect(
        &m_snapshotLoader, &Loader::pathLoaded, this, &FormWidgetLoader::snapshotPathLoaded);

    //    QObject::connect(ui->listView_frames, &QListView::currentChanged, this,
    //    &FormWidgetLoader::on_listView_frames_selectionChanged); connect(ui->listView_frames,
    //    SIGNAL(currentChanged(QModelIndex, QModelIndex)), this,
    //    SLOT(on_listView_frames_selectionChanged(QModelIndex, QModelIndex)));
}

FormWidgetLoader::~FormWidgetLoader()
{
    std::cout << "[FormWidgetLoader] ~FormWidgetLoader()" << std::endl;

    //    if (!m_recordPlayer.isPlaying() && m_recordPlayer.isLoaded()) // unload player due of
    //    viewer waiting for a new frame
    //        m_recordPlayer.unload();
    //    if (!m_snapShotPlayer.isPlaying() && m_snapShotPlayer.isLoaded()) // unload player due of
    //    viewer waiting for a new frame
    //        m_snapShotPlayer.unload();
    delete m_recordFileModel;
    //    delete m_recordFrameModel;
    delete m_snapshotFileModel;
    //    delete m_snapshotFrameModel;
    delete ui;
}

// void FormWidgetLoader::on_treeView_record_clicked( const QModelIndex& index ) {
//     const std::string& mPath =
//         m_recordFileModel->fileInfo( index ).absoluteFilePath().toStdString();

//    if ( m_recordLoader.isLoaded() && mPath == m_recordLoader.getLoadedPath() ) {
//        const auto& selectionModel = ui->treeView_record->selectionModel();
//        selectionModel->select( index, QItemSelectionModel::Deselect );
//        m_recordLoader.unload();
//        return;
//    }

////    if ( m_recordLoader.isLoaded() ) m_recordLoader.unload();
////    m_recordLoader.load( mPath );
//}

// void FormWidgetLoader::on_treeView_snapshot_clicked( const QModelIndex& index ) {
//     //    std::cout << "[FormWidgetLoader] on_treeView_snapshot_doubleClicked" << std::endl;

//    const std::string& mPath =
//        m_snapshotFileModel->fileInfo( index ).absoluteFilePath().toStdString();

//    // click twice -> unselect
//    if ( m_snapshotLoader.isLoaded() && mPath == m_snapshotLoader.getLoadedPath() ) {
//        const auto& selectionModel = ui->treeView_snapshot->selectionModel();
//        selectionModel->select( index, QItemSelectionModel::Deselect );
//        m_snapshotLoader.unload();
//        return;
//    }

////    if ( m_snapshotLoader.isLoaded() ) m_snapshotLoader.unload();
////    m_snapshotLoader.load( mPath );
//}

void FormWidgetLoader::onTreeView_record_currentChanged(const QModelIndex& current,
    const QModelIndex& previous)
{
    const std::string& mPath = m_recordFileModel->fileInfo(current).absoluteFilePath().toStdString();

    //    std::cout << "[FormWidgetLoader] on_treeView_record_currentChanged : " << mPath << std::endl;

    //    if (m_recordLoader.isLoaded() && mPath == m_recordLoader.getLoadedPath()) {
    //        const auto& selectionModel = ui->treeView_record->selectionModel();
    //        selectionModel->select(current, QItemSelectionModel::Deselect);
    //        m_recordLoader.unload();
    //        return;
    //    }

    //    if ( m_recordLoader.isLoaded() ) m_recordLoader.unload();

    //    m_recordLoader.load( mPath );
}

void FormWidgetLoader::onTreeView_snapshot_currentChanged(const QModelIndex& current,
    const QModelIndex& previous)
{

    const std::string& mPath = m_snapshotFileModel->fileInfo(current).absoluteFilePath().toStdString();

    std::cout << "[FormWidgetLoader] on_treeView_snapshot_currentChanged : " << mPath << std::endl;

    // click twice -> unselect
    //    if (m_snapshotLoader.isLoaded() && mPath == m_snapshotLoader.getLoadedPath()) {
    //        const auto& selectionModel = ui->treeView_snapshot->selectionModel();
    //        selectionModel->select(current, QItemSelectionModel::Deselect);
    //        m_snapshotLoader.unload();
    //        return;
    //    }

    if (m_snapshotLoader.isLoaded())
        m_snapshotLoader.unload();

    m_snapshotLoader.load(mPath);
}

void FormWidgetLoader::onTreeView_record_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    //    assert(! selected.empty());
    //    const auto & current = selected.indexes().first();

    const auto& indexes = ui->treeView_record->selectionModel()->selectedIndexes();

    if (indexes.empty()) {
        if (m_recordLoader.isLoaded())
            m_recordLoader.unload();
    } else {

        const auto& current = indexes.first();

        const std::string& mPath = m_recordFileModel->fileInfo(current).absoluteFilePath().toStdString();

        std::cout << "[FormWidgetLoader] on_treeView_record_selectionChanged : " << mPath << std::endl;
        if (m_recordLoader.isLoaded())
            m_recordLoader.unload();
        m_recordLoader.load(mPath);
    }
}

// void FormWidgetLoader::on_treeView_record_doubleClicked(const QModelIndex &index)
//{

//    const std::string& mPath =
//    m_recordFileModel->fileInfo(index).absoluteFilePath().toStdString();
////    const auto& mPath = m_recordFileModel->fileInfo(index).absolutePath().toStdString();
////    const auto& mFilename = m_recordFileModel->fileInfo(index).fileName().toStdString();

//    std::cout << "[FormWidgetLoader] on_treeView_record_doubleClicked : " << mPath << std::endl;

////    ui->treeView_record->edit(index);
////    QString name = QInput
////    auto newName = QInputDialog::getText(this, "Rename Record", "Enter a new
/// name").toStdString(); /    if (newName.empty()) return;

////    m_recordFileModel->fileRenamed(mPath, mFilename, newName);

////    std::filesystem::rename(mPath + "/" + mFilename, mPath + "/" + newName);

////    m_recordFileModel.

//    // click twice -> unselect
//}

// void FormWidgetLoader::on_treeView_snapshot_doubleClicked(const QModelIndex &index)
//{
//    std::cout << "[FormWidgetLoader] on_treeView_snapshot_doubleClicked" << std::endl;

//    const std::string& mPath =
//    m_snapshotFileModel->fileInfo(index).absoluteFilePath().toStdString();

//    // click twice -> unselect
//    if (m_snapshotLoader.isLoaded() && mPath == m_snapshotLoader.getLoadedPath()) {
//        const auto& selectionModel = ui->treeView_snapshot->selectionModel();
//        selectionModel->select(index, QItemSelectionModel::Deselect);
//        m_snapshotLoader.unload();
//        return;
//    }

//    if (m_snapshotLoader.isLoaded())
//        m_snapshotLoader.unload();

//    m_snapshotLoader.load(mPath);

//}

// void FormWidgetLoader::on_treeView_record_keyPressed(QKeyEvent *event)
//{

//}

// void FormWidgetLoader::updateRecordFrames() {

//    //    assert( m_currentPlayer != nullptr );
//    //    std::cout << "[FormWidgetLoader] updateAcquisitionView" << std::endl;
//}

// void FormWidgetLoader::updateSnapshotFrames() {

//    //    assert( m_currentPlayer != nullptr );
//    //    std::cout << "[FormWidgetLoader] updateAcquisitionView" << std::endl;
//    const auto& frames = m_snapshotLoader.getFrames();

//    //    //    auto & listModel = ui->listView_acqs->model();
//    //    //    auto & view = ui->tableView_acqs;
//    //    //    listView->model().re

//    QStringList stringList;
//    for ( int i = 0; i < frames.size(); ++i ) {
//        //        ui->listView_acqs.add
//        //        view->addAction((std::string("Frame ") + std::to_string(i)).c_str());
//        stringList << ( "Frame " + std::to_string( i ) ).c_str();
//    }
//    m_snapshotFrameModel->setStringList( stringList );
//    //        ui->listView_recordFrames->show();
//    //        view->show();
//}

// void FormWidgetLoader::on_listView_recordFrames_clicked( const QModelIndex& index ) {

//    const int iFrame = index.row();
//    std::cout << "[FormWidgetLoader] on_listView_recordFrames_clicked : " << iFrame << std::endl;

//    //        if (iFrame == m_recordLoader->getCurrentFrame()) {

//    //            const auto& selectionModel = ui->listView_recordFrames->selectionModel();
//    //            selectionModel->select(index, QItemSelectionModel::Deselect);
//    //        }
//}

// void FormWidgetLoader::on_listView_snapshotFrames_clicked( const QModelIndex& index ) {
//     std::cout << "[FormWidgetLoader] on_listView_snapshotFrames_clicked" << std::endl;
// }

void FormWidgetLoader::onRecordFrames_selectionChange(const QItemSelection& selected,
    const QItemSelection& deselected)
{

    const auto& selectedRows = ui->listView_recordFrames->selectionModel()->selectedRows();

    std::cout << "[FormWidgetLoader] onRecordFrames_selectionChanged : "
              << selectedRows.size() << std::endl;

    m_recordLoader.onFrame_selectionChange(selectedRows);



    //    m_selectedRecordFrames.clear();
    //    m_selectedRecordFrames.reserve(selectedRows.size());

    //    const auto & frames = m_recordLoader.getFrames();

    //    for ( QModelIndex index : selectedRows ) {
    //        m_selectedRecordFrames.push_back(frames[index.row()]);
    //    }

    //    emit recordFrames_selectionChanged();
}

void FormWidgetLoader::onRecordFrames_currentChanged(const QModelIndex& current,
    const QModelIndex& previous)
{
    std::cout << "[FormWidgetLoader] onRecordFrames_currentChanged : "
              << current.row() << std::endl;

    auto selectedRows = ui->listView_recordFrames->selectionModel()->selectedRows();
    selectedRows.append(current);

    //    std::cout << "[FormWidgetLoader] on_listView_recordFrames_selectionChanged : "
    //              << selectedRows.size() << std::endl;

    m_recordLoader.onFrame_selectionChange(selectedRows);
}


void FormWidgetLoader::onSnapshotFrames_selectionChange(const QItemSelection& selected,
    const QItemSelection& deselected)
{

    const auto& selectedRows = ui->listView_snapshotFrames->selectionModel()->selectedRows();

    std::cout << "[FormWidgetLoader] on_listView_snapshotFrames_selectionChanged : "
              << selectedRows.size() << std::endl;

    m_snapshotLoader.onFrame_selectionChange(selectedRows);

    //    m_selectedSnapshotFrames.clear();
    //    m_selectedSnapshotFrames.reserve(selectedRows.size());

    //    const auto & frames = m_snapshotLoader.getFrames();

    //    for ( QModelIndex index : selectedRows ) {
    //        m_selectedSnapshotFrames.push_back(frames[index.row()]);
    //    }

    //    emit snapshotFrames_selectionChanged();
}

void FormWidgetLoader::onSnapshotFrames_currentChanged(const QModelIndex& current,
    const QModelIndex& previous)
{
    auto selectedRows = ui->listView_snapshotFrames->selectionModel()->selectedRows();
    selectedRows.append(current);

    m_snapshotLoader.onFrame_selectionChange(selectedRows);
}

const Loader& FormWidgetLoader::getSnapshotLoader() const
{
    return m_snapshotLoader;
}

const Loader& FormWidgetLoader::getRecordLoader() const
{
    return m_recordLoader;
}

// void FormWidgetLoader::on_listView_frames_clicked( const QModelIndex& index ) {
////    assert( m_currentPlayer != nullptr );
//    std::cout << "[FormWidgetLoader] on_listView_frames_clicked" << std::endl;

//    //    const int iFrame = index.row();

//    //    if (iFrame == m_currentPlayer->getCurrentFrame()) {

//    //        const auto& selectionModel = ui->listView_frames->selectionModel();
//    //        selectionModel->select(index, QItemSelectionModel::Deselect);
//    //    }
//}

// void FormWidgetLoader::on_listView_frames_activated(const QModelIndex &index)
//{
//     std::cout << "[FormWidgetLoader] on_listView_frames_activated" << std::endl;
//     //    const auto & itemData = m_recordFrameModel->itemData(index);

//}

// void FormWidgetLoader::on_listView_frames_indexesMoved(const QModelIndexList &indexes)
//{
//     std::cout << "[FormWidgetLoader] on_listView_frames_indexesMoved" << std::endl;

//}

// void FormWidgetLoader::on_listView_frames_selectionChanged( const QModelIndex& selected,
//                                                            const QModelIndex& deselected ) {
////    assert( m_currentPlayer != nullptr );
//    std::cout << "[FormWidgetLoader] on_listView_frames_selectionChanged" << std::endl;

//    const std::string& frameName =
//        m_recordFrameModel->stringList().at( selected.row() ).toStdString();
//    std::cout << "frame clicked " << frameName << std::endl;

////    const int iFrame = selected.row();
////    if ( m_currentPlayer->isPlaying() ) m_currentPlayer->stop();

////    if ( iFrame != m_currentPlayer->getCurrentFrame() ) {

////        //        std::cout << "stop frame " << frameName << std::endl;
////        //        const auto& selectionModel = ui->listView_frames->selectionModel();
////        //        selectionModel->select(selected, QItemSelectionModel::Deselect);

////        //    } else {

////        m_currentPlayer->showFrame( iFrame );
////    }
//}

// void FormWidgetLoader::on_listView_recordFrames_indexesMoved(const QModelIndexList &indexes)
//{
//    std::cout << "indexes moved" << std::endl;

//}

// void FormWidgetLoader::on_treeView_record_pressed(const QModelIndex &index)
//{
//    std::cout << "pressed" << std::endl;

//}

// void FormWidgetLoader::on_treeView_record_activated(const QModelIndex &index)
//{
//    std::cout << "activated" << std::endl;

//////    const std::string& mPath =
/// m_recordFileModel->fileInfo(index).absoluteFilePath().toStdString();

//////    std::cout << "[FormWidgetLoader] on_treeView_record_clicked : " << mPath << std::endl;
////    const auto& mPath = m_recordFileModel->fileInfo(index).absolutePath().toStdString();
////    const auto& mFilename = m_recordFileModel->fileInfo(index).fileName().toStdString();

////    std::cout << "[FormWidgetLoader] on_treeView_record_doubleClicked : " << mPath << std::endl;

//////    ui->treeView_record->edit(index);
//////    QString name = QInput
////    auto newName = QInputDialog::getText(this, "Rename Record", "Enter a new
/// name").toStdString(); /    if (newName.empty()) return;

//////    m_recordFileModel->fileRenamed(mPath, mFilename, newName);

////    std::filesystem::rename(mPath + "/" + mFilename, mPath + "/" + newName);

//////    ui->treeView_record->editTriggers();

//////    QFile file("test.txt");
//////    if(file.exists() && file.open(QIODevice::ReadWrite))
//////    {
//////      if(file.rename("text1.txt"))
//////      {
//////        qDebug() << "renamed";
//////      }
//////      file.close();
//////    }

//}

// void FormWidgetLoader::on_treeView_record_entered(const QModelIndex &index)
//{

//    std::cout << "entered" << std::endl;
//}

// void FormWidgetLoader::on_treeView_record_viewportEntered()
//{
//    std::cout << "viewport entered" << std::endl;

//}
