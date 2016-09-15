#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include <QDockWidget>

QT_BEGIN_NAMESPACE
class QTreeView;
class Vertex;
class Line;
QT_END_NAMESPACE

class PropertiesWindow : public QDockWidget
{
public:
    PropertiesWindow(QWidget *parent);
    PropertiesWindow(Vertex *vertex, QWidget *parent);
    PropertiesWindow(Line *line, QWidget *parent);

    void showPropertiesOf(Vertex *vertex);
    void showPropertiesOf(Line *line);

private:
    QTreeView *tree;
};

#endif // PROPERTIESWINDOW_H
