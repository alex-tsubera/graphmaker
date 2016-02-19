#include "vertex.h"
#include "line.h"
#include "graph.h"
#include <QPainter>
#include <QFont>
#include <QJsonArray>
#include <QJsonObject>

Vertex::Vertex()
    : QGraphicsEllipseItem(QRect(0, 0, VertexSize, VertexSize)), name(), nearLines()
{
    setFlags(QGraphicsItem::ItemIsSelectable
             | QGraphicsItem::ItemIsMovable
             | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(VertexZValue);
}

Vertex::Vertex(const QString &name, const QPointF &position,
                      const QPen &pen, const QBrush &brush)
    : QGraphicsEllipseItem(), name(name), nearLines()
{
    setPosition(position);
    setBrush(brush);
    setPen(pen);
    setFlags(QGraphicsItem::ItemIsSelectable
             | QGraphicsItem::ItemIsMovable
             | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(VertexZValue);
}

Vertex::Vertex(const QJsonObject &json)
    : QGraphicsEllipseItem(), name(json["name"].toString()), nearLines()
{
    QJsonArray pos = json["position"].toArray();
    QPointF p = mapFromScene(pos[0].toDouble(), pos[1].toDouble());
    setPosition(p);
    setBrush(QBrush(QColor(json["color"].toString())));
    setPen(QPen(QBrush(QColor(json["borderColor"].toString())), Graph::VertexBorderThickness));
    setFlags(QGraphicsItem::ItemIsSelectable
             | QGraphicsItem::ItemIsMovable
             | QGraphicsItem::ItemSendsGeometryChanges);
    setZValue(VertexZValue);
}

QVariant Vertex::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionHasChanged) {
        QPointF point1;
        QPointF point2;
        //For each line connected to this vertex set up a new coordinates.
        for(int i = 0; i < nearLines.size(); i++) {
            if(nearLines[i]->getVertex1()->getName() == this->name) {
                //Converting to scene coordinates.
                point1 = this->mapToScene(this->getPosition());
                point2 = nearLines[i]->getVertex2()->mapToScene(nearLines[i]->getVertex2()->getPosition());
            } else {
                point2 = this->mapToScene(this->getPosition());
                point1 = nearLines[i]->getVertex1()->mapToScene(nearLines[i]->getVertex1()->getPosition());
            }
            //Converting to local coordinates of current line and setting up new line.
            nearLines[i]->setLine(QLineF(nearLines[i]->mapFromScene(point1),
                                         nearLines[i]->mapFromScene(point2)));
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Vertex::boundingRect() const
{
    int offset = Graph::FontSize + Graph::FontOffset;
    return QRectF(this->rect().x() - offset,
                  this->rect().y() - offset,
                  this->rect().width() + offset,
                  this->rect().height() + offset);
}

void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Draw vertex.
    QGraphicsEllipseItem::paint(painter, option, widget);
    //Draw vertex name.
    QFont font("Times", Graph::FontSize);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(rect().x() - Graph::FontOffset, rect().y() - Graph::FontOffset, name);
}

QJsonObject Vertex::toJson() const
{
    QJsonObject json;
    json["name"] = name;

    QJsonArray position;
    QPointF p = mapToScene(getPosition());
    position.append(QJsonValue(p.x()));
    position.append(QJsonValue(p.y()));
    json["position"] = position;

    json["borderColor"] = pen().color().name();

    json["color"] = brush().color().name();

    return json;
}
