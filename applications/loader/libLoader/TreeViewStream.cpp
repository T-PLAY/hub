#include "TreeViewStream.h"

#include <QKeyEvent>
#include <iostream>

#include <QDir>
#include <QFileSystemModel>
#include <QInputDialog>
#include <filesystem>

TreeViewStream::TreeViewStream( QWidget* parent ) : QTreeView( parent ) {}

void TreeViewStream::keyPressEvent( QKeyEvent* event ) {
    const auto& index          = this->currentIndex();
    const auto* model          = dynamic_cast<const QFileSystemModel*>( this->model() );
    const auto& selectionModel = this->selectionModel();

    const auto& mPath     = model->fileInfo( index ).absolutePath().toStdString();
    const auto& mFilename = model->fileInfo( index ).fileName().toStdString();
    std::string filepath  = mPath + "/" + mFilename;

    assert( std::filesystem::exists( filepath ) );

    switch ( event->key() ) {

    case Qt::Key_F2: {
        auto newName =
            QInputDialog::getText(
                this, "Rename Record", "Enter a new name", QLineEdit::Normal, mFilename.c_str() )
                .toStdString();
        if ( newName.empty() ) return;

        std::string newFilepath = mPath + "/" + newName;

        std::filesystem::rename( filepath, newFilepath );

    } break;

    case Qt::Key_Delete: {
        std::filesystem::remove_all( filepath );
    } break;

    case Qt::Key_Escape:
        selectionModel->clearSelection();
        break;

    case Qt::Key_Space:
        emit onSpace();
        break;

    default:
        QTreeView::keyPressEvent( event );
    }
}
