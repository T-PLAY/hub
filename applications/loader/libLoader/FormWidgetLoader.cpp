#include "FormWidgetLoader.h"
#include "ui_FormWidgetLoader.h"

#include <filesystem>
#include <iostream>

#include <QInputDialog>
#include <QItemSelectionModel>

FormWidgetLoader::FormWidgetLoader( QWidget* parent ) :
    QWidget( parent ),
    ui( new Ui::FormWidgetLoader )
//    m_recordLoader( " (record)" ) {
{
    ui->setupUi( this );

    // records view
    {
        // tree view
        QString recordPath = PROJECT_DIR "data/records/";
        if ( !std::filesystem::exists( recordPath.toStdString() ) ) {
            std::filesystem::create_directory( recordPath.toStdString() );
        }
        assert( std::filesystem::exists( recordPath.toStdString() ) );
        m_recordFileModel = new QFileSystemModel( this );
        m_recordFileModel->setReadOnly( true );
        // Set filter
        m_recordFileModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries );
        //    m_recordFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
        // QFileSystemModel requires root path
        m_recordFileModel->setRootPath( recordPath );
        m_recordFileModel->sort( 0, Qt::SortOrder::DescendingOrder );

        // Attach the model to the view
        ui->treeView_record->setModel( m_recordFileModel );
        const auto& rootIndex = m_recordFileModel->index( recordPath );
        ui->treeView_record->setRootIndex( rootIndex );
        ui->treeView_record->expand( rootIndex );
        ui->treeView_record->setColumnHidden( 1, true );
        ui->treeView_record->setColumnHidden( 2, true );
        ui->treeView_record->setColumnHidden( 3, true );
        //    ui->treeView->setItemsExpandable(false);
        //    ui->treeView->setColumnHidden(0, true);

        //        QObject::connect( ui->treeView_record->selectionModel(),
        //                          &QItemSelectionModel::currentChanged,
        //                          this,
        //                          &FormWidgetLoader::onTreeView_record_currentChanged );

        QObject::connect( ui->treeView_record->selectionModel(),
                          &QItemSelectionModel::selectionChanged,
                          this,
                          &FormWidgetLoader::onTreeView_record_selectionChanged );
    }

    //    QObject::connect(
    //        &m_recordLoader, &Loader::pathLoaded, this, &FormWidgetLoader::recordPathLoaded );
}

FormWidgetLoader::~FormWidgetLoader() {
    std::cout << "[FormWidgetLoader] ~FormWidgetLoader()" << std::endl;

    delete m_recordFileModel;
    delete ui;
}

// void FormWidgetLoader::onTreeView_record_currentChanged( const QModelIndex& current,
//                                                          const QModelIndex& previous ) {
//     const std::string& mPath =
//         m_recordFileModel->fileInfo( current ).absoluteFilePath().toStdString();
//     //    std::cout << "[FormWidgetLoader] on_treeView_record_currentChanged : " << mPath <<
//     //    std::endl;

//    //    if (m_recordLoader.isLoaded() && mPath == m_recordLoader.getLoadedPath()) {
//    //        const auto& selectionModel = ui->treeView_record->selectionModel();
//    //        selectionModel->select(current, QItemSelectionModel::Deselect);
//    //        m_recordLoader.unload();
//    //        return;
//    //    }

//    //    if ( m_recordLoader.isLoaded() ) m_recordLoader.unload();

//    //    m_recordLoader.load( mPath );
//}

void FormWidgetLoader::onTreeView_record_selectionChanged( const QItemSelection& selected,
                                                           const QItemSelection& deselected ) {
    //    assert(! selected.empty());
    //    const auto & current = selected.indexes().first();

    const auto& indexes = ui->treeView_record->selectionModel()->selectedIndexes();

    if ( indexes.empty() ) {
        if ( m_recordLoader.isLoaded() ) {
            m_recordLoader.unload();
        }
    }
    else {

        const auto& current = indexes.first();

        const std::string& mPath =
            m_recordFileModel->fileInfo( current ).absoluteFilePath().toStdString();

        std::cout << "[FormWidgetLoader] on_treeView_record_selectionChanged : " << mPath
                  << std::endl;
        if ( m_recordLoader.isLoaded() )
        {
//            emit recordPathUnloaded();
//            m_recordLoader.unload();
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        m_recordLoader.load( mPath );
    }
}

const Loader& FormWidgetLoader::getRecordLoader() const {
    return m_recordLoader;
}
