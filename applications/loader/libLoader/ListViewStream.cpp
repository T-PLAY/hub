#include "ListViewStream.h"

// ListViewStream::ListViewStream()
//{

//}
#include <QKeyEvent>
#include <iostream>

ListViewStream::ListViewStream( QWidget* parent ) : QListView( parent ) {}

void ListViewStream::keyPressEvent( QKeyEvent* event ) {

    const auto& index          = this->currentIndex();
    const auto& selectionModel = this->selectionModel();

    switch ( event->key() ) {
    case Qt::Key_Escape:

        selectionModel->select( index, QItemSelectionModel::Deselect );
        std::cout << "deselect" << std::endl;
        break;
    default:
        QListView::keyPressEvent( event );
    }
}
