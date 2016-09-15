#include "propertieswindow.h"
#include <QTreeView>
#include "vertex.h"
#include "line.h"

PropertiesWindow::PropertiesWindow(QWidget *parent)
    : QDockWidget("Properties", parent)
{
    tree = new QTreeView(this);
}

PropertiesWindow::PropertiesWindow(Vertex *vertex, QWidget *parent)
    : PropertiesWindow(parent)
{

}

