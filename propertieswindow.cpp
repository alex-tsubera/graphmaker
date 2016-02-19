#include "propertieswindow.h"
#include <QTreeView>

PropertiesWindow::PropertiesWindow(QWidget *parent)
    : QDockWidget("Properties", parent)
{
    tree = new QTreeView(this);
}

PropertiesWindow::PropertiesWindow(const Vertex *vertex, QWidget *parent)
    : PropertiesWindow(parent)
{

}

