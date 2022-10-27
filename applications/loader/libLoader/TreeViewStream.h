#ifndef TREEVIEWSTREAM_H
#define TREEVIEWSTREAM_H

#include <QTreeView>
#include <QWidget>

class TreeViewStream : public QTreeView
{
    Q_OBJECT
  public:
    TreeViewStream( QWidget* parent = nullptr );

signals:
    void onSpace();

  protected:
    void keyPressEvent( QKeyEvent* event ) override;

  private:
};

#endif // TREEVIEWSTREAM_H
