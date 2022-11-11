#include "FormWidgetLoader.h"
#include "ui_FormWidgetLoader.h"

#include <filesystem>
#include <iostream>

#include <QInputDialog>
#include <QItemSelectionModel>

FormWidgetLoader::FormWidgetLoader( const std::string& ipv4, const int& port, QWidget* parent ) :
    QWidget( parent ), ui( new Ui::FormWidgetLoader ), m_recordLoader( ipv4, port ) {
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

//        ui->treeView_record->setSelectionMode(QAbstractItemView::MultiSelection);

        QObject::connect(
            ui->treeView_record, &TreeViewStream::onSpace, this, &FormWidgetLoader::onSpace );

        QObject::connect( ui->treeView_record->selectionModel(),
                          &QItemSelectionModel::selectionChanged,
                          this,
                          &FormWidgetLoader::onTreeView_record_selectionChanged );
    }

    m_recordLoader.setAutoLoop( ui->checkBox_autoLoop->isChecked() );

    QObject::connect( &m_recordLoader, &Loader::acqChanged, this, &FormWidgetLoader::onAcqChanged );
    QObject::connect( &m_recordLoader, &Loader::playEnded, this, &FormWidgetLoader::onPlayEnded );
    m_recordLoader.setAutoPlay( ui->pushButton_playPause->text() == "Pause" );
}

FormWidgetLoader::~FormWidgetLoader() {
    std::cout << "[FormWidgetLoader] ~FormWidgetLoader()" << std::endl;

    delete m_recordFileModel;
    delete ui;
}

//void FormWidgetLoader::onTreeView_record_currentChanged(const QModelIndex &current, const QModelIndex &previous)
//{

//        const std::string& mPath =
//            m_recordFileModel->fileInfo( current ).absoluteFilePath().toStdString();
//        std::cout << "[FormWidgetLoader] onTreeView_record_currentChanged : " << mPath << std::endl;
//}

void FormWidgetLoader::onTreeView_record_selectionChanged( const QItemSelection& selected,
                                                           const QItemSelection& deselected ) {

    const auto& indexes = ui->treeView_record->selectionModel()->selectedIndexes();
    //    std::cout << "start selected" << std::endl;
    //    for (const auto & index : indexes) {
    //        const std::string& mPath =
    //            m_recordFileModel->fileInfo( index ).absoluteFilePath().toStdString();
    //        std::cout << mPath << std::endl;
    ////        std::cout << index.data() << std::endl;
    //    }
    //    std::cout << "end selected" << std::endl;

    if ( indexes.empty() ) {
                if ( m_recordLoader.isLoaded() ) {
                    m_recordLoader.unload();
                }
    }
    else {

//        assert( indexes.size() == 1 );
//        const auto& current = indexes.first();
//        for ( const auto& index : selected.indexes() ) {
        std::set<std::string> paths;
        for ( const auto& index : indexes ) {
            //            std::cout << "index : " << index << std::endl;
            const std::string& mPath =
                m_recordFileModel->fileInfo( index ).absoluteFilePath().toStdString();
//            std::cout << mPath << std::endl;
            paths.insert(mPath);
        }
        for (const auto & path : paths) {
            std::cout << path << std::endl;
        }
//        return;

//        const std::string& mPath =
//            m_recordFileModel->fileInfo( current ).absoluteFilePath().toStdString();

//        std::cout << "[FormWidgetLoader] on_treeView_record_selectionChanged : " << mPath
//                  << std::endl;

        //        if ( m_recordLoader.isLoaded() ) {
        //        }

                m_recordLoader.load( paths );
                const int nAcq = m_recordLoader.getNAcq();
                ui->label_nAcq->setText( "/ " + QString::number( nAcq ) );
                ui->horizontalSlider_iAcq->setMaximum( nAcq - 1 );
                ui->spinBox_iAcq->setMaximum( nAcq - 1 );
    }
}

const Loader& FormWidgetLoader::getRecordLoader() const {
    return m_recordLoader;
}

void FormWidgetLoader::onAcqChanged( int iAcq ) {
    ui->horizontalSlider_iAcq->setValue( iAcq );
    ui->spinBox_iAcq->setValue( iAcq );
}

void FormWidgetLoader::onSpace() {
    ui->pushButton_playPause->click();
}

void FormWidgetLoader::onPlayEnded() {
    ui->pushButton_playPause->click();
}


void FormWidgetLoader::on_checkBox_autoLoop_toggled( bool checked ) {
    m_recordLoader.setAutoLoop( checked );
}

void FormWidgetLoader::on_pushButton_playPause_clicked() {
    if ( ui->pushButton_playPause->text() == "Play" ) {
        if ( m_recordLoader.isLoaded() ) { m_recordLoader.play(); }
        ui->pushButton_playPause->setText( "Pause" );
        m_recordLoader.setAutoPlay( true );
    }
    else {
        assert( ui->pushButton_playPause->text() == "Pause" );
        if ( m_recordLoader.isLoaded() ) { m_recordLoader.stop(); }
        ui->pushButton_playPause->setText( "Play" );
        m_recordLoader.setAutoPlay( false );
    }
}

void FormWidgetLoader::on_horizontalSlider_iAcq_valueChanged( int value ) {
    if ( ui->pushButton_playPause->text() == "Play" ) {
        m_recordLoader.setIAcq( value );
        ui->spinBox_iAcq->setValue( value );
    }
}

void FormWidgetLoader::on_spinBox_iAcq_valueChanged( int arg1 ) {}
