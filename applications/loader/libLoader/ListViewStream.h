#ifndef LISTVIEWSTREAM_H
#define LISTVIEWSTREAM_H

#include <QListView>

class ListViewStream : public QListView
{
    Q_OBJECT
public:
    ListViewStream(QWidget * parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent * event) override;
};

#endif // LISTVIEWSTREAM_H
