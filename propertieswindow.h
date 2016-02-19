#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

class PropertiesWindow : public QDockWidget
{
public:
    PropertiesWindow(QWidget *parent);
    PropertiesWindow(const Vertex *vertex, QWidget *parent);
    PropertiesWindow(const Line *line, QWidget *parent);

    void showPropertiesOf(const Vertex *vertex);
    void showPropertiesOf(const Line *line);

private:
    QTreeView *tree;
};

#endif // PROPERTIESWINDOW_H
