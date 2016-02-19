#include "graph.h"
#include <QJsonArray>
#include <QJsonObject>

Graph::Graph()
    : scene(new QGraphicsScene), vertices(new QList<Vertex *>()),
      lines(new QList<Line *>()), spanningTree(new QList<Line *>()) {}

Graph::Graph(const QJsonObject &json)
    : Graph::Graph()
{
    QJsonArray jsonVertices = json["vertices"].toArray();
    for(int i = 0; i < jsonVertices.size(); i++)
        addVertex(new Vertex(jsonVertices.at(i).toObject()));

    QJsonArray jsonLines = json["lines"].toArray();
    for(int i = 0; i < jsonLines.size(); i++)
        addLine(new Line(jsonLines.at(i).toObject(), *this));
}

Graph::~Graph()
{
    delete vertices;
    delete lines;
    delete spanningTree;
    delete scene;
}

void Graph::addVertex(Vertex *vertex)
{
    QGraphicsItem * item = static_cast<QGraphicsItem *>(vertex);
    if(item != NULL) {
        vertices->append(vertex);
        scene->addItem(item);
    }
}

void Graph::removeVertex(Vertex *vertex)
{
    QGraphicsItem * item = static_cast<QGraphicsItem *>(vertex);
    if(item != NULL) {
        vertices->removeOne(vertex);
        scene->removeItem(item);
        delete vertex;
    }
}

void Graph::addLine(Line *line)
{
    QGraphicsItem * item = static_cast<QGraphicsItem *>(line);
    if(item != NULL) {
        lines->append(line);
        line->getVertex1()->addNearLine(line);
        line->getVertex2()->addNearLine(line);
        scene->addItem(item);
    }
}

void Graph::removeLine(Line *line)
{
    QGraphicsItem * item = static_cast<QGraphicsItem *>(line);
    if(item != NULL) {
        line->getVertex1()->removeNearLine(line);
        line->getVertex2()->removeNearLine(line);
        lines->removeOne(line);
        scene->removeItem(item);
        delete line;
    }
}

bool Graph::contains(const QList<Line *> &list, const Line &line)
{
    foreach (Line *element, list)
        if((element->getVertex1()->getName() == line.getVertex1()->getName()
            && element->getVertex2()->getName() == line.getVertex2()->getName())
                || (element->getVertex1()->getName() == line.getVertex2()->getName()
                    && element->getVertex2()->getName() == line.getVertex1()->getName()))
            return true;
    return false;
}

bool Graph::treeContains(const Vertex &vertex) const
{
    foreach (Line * line, *spanningTree)
        if(line->getVertex1()->getName() == vertex.getName()
                || line->getVertex2()->getName() == vertex.getName())
            return true;
    return false;
}

void Graph::findSpanningTree()
{
    spanningTree->clear();
    for(int i = 0; i < lines->size(); i++)
        if(!treeContains(*(lines->at(i)->getVertex1()))
                || !treeContains(*(lines->at(i)->getVertex2())))
            spanningTree->append(lines->at(i));
}

void Graph::showSpanningTree()
{
    static QVector<QColor> colors;
    if(spanningTree->empty()) {
        findSpanningTree();
        foreach (Line *line, *spanningTree) {
            colors.append(line->pen().color());
        }
    }
    static bool show = true;
    for (int i = 0; i < spanningTree->size(); i++)
        if(show) {
            lines->at(i)->setPen(QPen(Qt::red, LineThickness));
            lines->at(i)->setZValue(Line::LineHighlightedZValue);
        } else {
            lines->at(i)->setPen(QPen(colors[i], LineThickness));
            lines->at(i)->setZValue(Line::LineZValue);
        }
    show = !show;
}

QJsonObject Graph::toJson() const
{
    QJsonObject json;
    QJsonArray jsonVertices;
    for(int i = 0; i < vertices->size(); i++)
        jsonVertices.append(vertices->at(i)->toJson());
    json["vertices"] = jsonVertices;

    QJsonArray jsonLines;
    for (int i = 0; i < lines->size(); i++)
        jsonLines.append(lines->at(i)->toJson());
    json["lines"] = jsonLines;

    return json;
}

const Vertex * Graph::getVertexByName(const QString &name) const
{
    for(int i = 0; i < vertices->size(); i++)
        if(vertices->at(i)->getName() == name)
            return vertices->at(i);
    return NULL;
}
