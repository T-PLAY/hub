#ifndef TREEVIEWSTREAM_H
#define TREEVIEWSTREAM_H

#include <QTreeView>
#include <QWidget>
#include <Loader.h>

class TreeViewStream : public QTreeView
{
    Q_OBJECT
public:
    TreeViewStream(QWidget * parent = nullptr);

    void setLoader(Loader *loader);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    Loader * m_loader = nullptr;
//    void onStreamRename();
};

#endif // TREEVIEWSTREAM_H
