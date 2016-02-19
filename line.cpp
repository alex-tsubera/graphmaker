#include "line.h"
#include "graph.h"
#include <QPainter>
#include <QJsonObject>
#include <QGraphicsView>

Line::Line(int weight, const Vertex &vertex1, const Vertex &vertex2, const QPen &pen)
    : QGraphicsLineItem(QLineF(vertex1.getPosition(), vertex2.getPosition())),
      weight(weight), vertex1(&vertex1), vertex2(&vertex2)
{
    setPen(pen);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(LineZValue);
}

Line::Line(const QJsonObject &json, const Graph &graph)
    : QGraphicsLineItem()
{
    weight = json["weight"].toInt();
    const Vertex *v =  graph.getVertexByName(json["vertex1"].toString());
    if(v != NULL)
        setVertex1(*v);
    v = graph.getVertexByName(json["vertex2"].toString());
    if(v != NULL)
        setVertex2(*v);
    setPen(QPen(QBrush(QColor(json["color"].toString())), Graph::LineThickness));
}

QRectF Line::boundingRect() const
{
    //Additional region to draw the weight.
    int offset = Graph::FontSize * 2 + Graph::FontOffset + Graph::LineThickness;

    QRectF rect = QGraphicsLineItem::boundingRect();

    //Increase width or height if its half less than region needed.
    if(rect.width() / 2 < offset) {
        rect.setWidth(rect.width() + offset);
    }
    if(rect.height() / 2 < offset) {
        rect.setHeight(rect.height() + offset);
    }
    return rect;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Draw line.
    QGraphicsLineItem::paint(painter, option, widget);

    //Draw line weight.
    QFont font("Times", Graph::FontSize);
    painter->setFont(font);
    painter->setPen(Qt::darkGray);
    //Offset from middle of the line.
    int offset = Graph::FontOffset + Graph::LineThickness + Graph::FontSize;
    int x = (line().p1().x() + line().p2().x()) / 2 + offset;
    int y = (line().p1().y() + line().p2().y()) / 2 + offset;
    painter->drawText(x, y, QString("%1").arg(weight));
}

QJsonObject Line::toJson() const
{
    QJsonObject json;
    json["weight"] = weight;
    json["color"] = pen().color().name();
    json["vertex1"] = vertex1->getName();
    json["vertex2"] = vertex2->getName();
    return json;
}
