#include "TreeViewStream.h"

//#include <QtDebug>
#include <QKeyEvent>
#include <iostream>

#include <QFileSystemModel>
#include <QInputDialog>
#include <filesystem>

TreeViewStream::TreeViewStream( QWidget* parent ) : QTreeView( parent ) {}

void TreeViewStream::keyPressEvent( QKeyEvent* event ) {
    assert(m_loader != nullptr);

    //    QTreeView::keyPressEvent(event);
    const auto& index = this->currentIndex();
    const auto* model = dynamic_cast<const QFileSystemModel*>( this->model() );
    const auto& selectionModel = this->selectionModel();

    //    std::cout << "[FormWidgetLoader] on_treeView_record_clicked : " << mPath << std::endl;
    const auto& mPath     = model->fileInfo( index ).absolutePath().toStdString();
    const auto& mFilename = model->fileInfo( index ).fileName().toStdString();

//    std::cout << "[FormWidgetLoader] on_treeView_record_doubleClicked : " << mPath << std::endl;

    switch ( event->key() ) {

    case Qt::Key_F2: {
        // rename current stream
        //        std::cout << "F2" << std::endl;
        //        onStreamRename();
        auto newName =
            QInputDialog::getText( this, "Rename Record", "Enter a new name" ).toStdString();
        if ( newName.empty() ) return;

        //    m_recordFileModel->fileRenamed(mPath, mFilename, newName);

        std::filesystem::rename( mPath + "/" + mFilename, mPath + "/" + newName );
    } break;

    case Qt::Key_Escape:
        std::cout << "Escape" << std::endl;
//        const auto& selectionModel = ui->treeView_snapshot->selectionModel();
        selectionModel->select( index, QItemSelectionModel::Deselect );
        assert(m_loader->isLoaded());
        m_loader->unload();
//        m_snapshotLoader.unload();
        break;

    default:
        QTreeView::keyPressEvent( event );
    }
}

void TreeViewStream::setLoader(Loader *loader)
{
    m_loader = loader;
}

// void TreeViewStream::onStreamRename()
//{

////    const std::string& mPath =
/// m_recordFileModel->fileInfo(index).absoluteFilePath().toStdString();

////    ui->treeView_record->edit(index);
////    QString name = QInput
////    ui->treeView_record->editTriggers();

////    QFile file("test.txt");
////    if(file.exists() && file.open(QIODevice::ReadWrite))
////    {
////      if(file.rename("text1.txt"))
////      {
////        qDebug() << "renamed";
////      }
////      file.close();
////    }

//}
