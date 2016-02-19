#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include "graph.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QGraphicsScene;
class QGraphicsView;
QT_END_NAMESPACE

//This class represents a main window of the application.
//It contains current graph and view to draw the graph.
//It also provides ability to create a new graph, save and open a graph.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum MainWindowSize {
        MinimumHeight = 200,
        MinimumWidth = 400
    };

    MainWindow();

    inline const Graph &getGraph() const { return *graph; }

public slots:
    void createNewGraph();
    void saveGraphAsPicture();
    void saveGraphAsJson();
    void openGraph();
    void showPropertiesWindow();
    void showSpanningTree();


private:
    Graph *graph;
    QGraphicsView *view;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    QDockWidget *propertiesWindow;
};

#endif // MAINWINDOW_H
