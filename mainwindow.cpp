#include "mainwindow.h"
#include "createnewgraphdialog.h"

#include <QtWidgets>

MainWindow::MainWindow()
{
    graph = new Graph;

    //Setting up the view.
    view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing, false);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setScene(graph->getScene());

    //Adding File menu.
    menuFile = menuBar()->addMenu(tr("File"));
    QAction *newGraphAction = menuFile->addAction(tr("New graph"));
    connect(newGraphAction, SIGNAL(triggered()), this, SLOT(createNewGraph()));
    QAction *openGraphAction = menuFile->addAction(tr("Open"));
    connect(openGraphAction, SIGNAL(triggered(bool)), this, SLOT(openGraph()));
    QMenu *saveAs = menuFile->addMenu(tr("Save as"));
    QAction *saveGraphAsPictureAction = saveAs->addAction(tr("Picture"));
    QAction *saveGraphAsJsonAction = saveAs->addAction(tr("JSON"));
    connect(saveGraphAsPictureAction, SIGNAL(triggered(bool)), this, SLOT(saveGraphAsPicture()));
    connect(saveGraphAsJsonAction, SIGNAL(triggered(bool)), this, SLOT(saveGraphAsJson()));
    QAction *exitAction = menuFile->addAction(tr("Exit"));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    //Adding Edit menu.
    menuEdit = menuBar()->addMenu(tr("Edit"));
    QAction *addVertexAction = menuEdit->addAction(tr("Add vertex"));
    connect(addVertexAction, SIGNAL(triggered(bool)), this, SLOT(addVertex()));
    QAction *removeVertexAction = menuEdit->addAction(tr("Remove vertex"));
    connect(removeVertexAction, SIGNAL(triggered(bool)), this, SLOT(removeVertex()));
    menuEdit->addSeparator();
    QAction *addLineAction = menuEdit->addAction(tr("Add line"));
    connect(addLineAction, SIGNAL(triggered(bool)), this, SLOT(lineVertex()));
    QAction *removeLineAction = menuEdit->addAction(tr("Remove line"));
    connect(removeLineAction, SIGNAL(triggered(bool)), this, SLOT(removeLine()));

    //Adding View menu.
    menuView = menuBar()->addMenu(tr("View"));
    QAction *showPropertiesAction = menuView->addAction(tr("Show Properties window"));
    showPropertiesAction->setCheckable(true);
    connect(showPropertiesAction, SIGNAL(toggled(bool)), this, SLOT(showPropertiesWindow()));
    menuView->addSeparator();
    QAction *showSpanningTreeAction = menuView->addAction(tr("Show Spanning tree"));
    showSpanningTreeAction->setCheckable(true);
    connect(showSpanningTreeAction, SIGNAL(toggled(bool)), this, SLOT(showSpanningTree()));

    //Adding Help menu.
    menuHelp = menuBar()->addMenu(tr("Help"));
    QAction *showAboutAction = menuHelp->addAction(tr("About"));
    connect(showAboutAction, SIGNAL(triggered(bool)), this, SLOT(showAbout()));
    QAction *showHowToUseAction = menuHelp->addAction(tr("How to use"));
    connect(showHowToUseAction, SIGNAL(triggered(bool)), this, SLOT(showHowToUse()));

    //Setting up the main window.
    setCentralWidget(view);
    setWindowTitle(tr("Graph maker"));
    setWindowIcon(QIcon(":/icons/ic_timeline_black.png"));
    setMinimumHeight(MinimumHeight);
    setMinimumWidth(MinimumWidth);
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::createNewGraph()
{
    //Creating dialog to allow user to enter graph's data.
    CreateNewGraphDialog dialog(this);
    int ret = dialog.exec();
    if(ret == QDialog::Rejected)
        return;
    //Uncheck Show spanning tree.
    menuView->actions().at(2)->setChecked(false);
    //Create new graph.
    delete graph;
    graph = new Graph();
    quint16 vertexCount = dialog.getVertexCount();
    quint16 lineCount = dialog.getLineCount();
    int x = 0;
    int y = 0;
    int dx = 100;
    QPen penForVertex(QBrush(Qt::blue), Graph::VertexBorderThickness);
    QPen penForLine(QBrush(Qt::blue), Graph::LineThickness);
    QBrush brush(Qt::yellow);
    char rightChar = 'a';
    char leftChar = 'a';
    QString vertexName;
    static const quint16 alphabetSize = 26;
    qsrand(QTime(0,0).msecsTo(QTime::currentTime()));
    //Adding vertices.
    for(int i = 0; i < vertexCount; i++) {
        if(i && !(i % alphabetSize)) {
            vertexName = leftChar++;
        }
        graph->addVertex(new Vertex(vertexName + (rightChar + (i % alphabetSize)),
                                    QPointF(x, y), penForVertex, brush));
        x += (i % 2) * dx;
        y = (qrand() % (height() - dx)) + dx / 2;
    }
    x = 1;
    y = 0;
    //Adding lines while lineCount and while connections are available.
    while((x < vertexCount) && (y < lineCount)) {
        for(int k = 0; k + x < vertexCount && y < lineCount; k++) {
            graph->addLine(new Line((qrand() % Graph::MaxLineWeight) + 1, graph->getVertexAt(k),
                                    graph->getVertexAt(k + x), penForLine));
            y++;
        }
        x++;
    }
    view->setScene(graph->getScene());
}

void MainWindow::saveGraphAsPicture()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as picture"), "/",
                                                    tr("Image files (*.png *.jpg *.bmp)"));
    QPixmap pixmap = QPixmap::grabWidget(view);
    QMessageBox result;
    if(pixmap.save(fileName)) {
        result.setText("The picture has been saved.");
    } else {
        result.setText("Couldn't save the picture.");
    }
    result.exec();
}

void MainWindow::saveGraphAsJson()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as JSON"), "/",
                                                    tr("JSON file (*.json)"));
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject jsonGraph(graph->toJson());
    QJsonDocument jsonDocument(jsonGraph);
    file.write(jsonDocument.toJson());
    file.close();
    QMessageBox result;
    result.setText(tr("JSON file has been saved."));
    result.exec();
}

void MainWindow::openGraph()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Open graph from JSON"), "/",
                                            tr("JSON files (*json)")));
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open the file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(data));
    //delete graph;
    graph = new Graph(jsonDocument.object());
    view->setScene(graph->getScene());
}

void MainWindow::showPropertiesWindow()
{
    static bool show = true;
    if(show) {
        propertiesWindow = new QDockWidget(tr("Properties"));

        addDockWidget(Qt::LeftDockWidgetArea, propertiesWindow);
    } else {
        removeDockWidget(propertiesWindow);
        delete propertiesWindow;
    }
    show = !show;
}

void MainWindow::showSpanningTree()
{
    graph->showSpanningTree();
}
