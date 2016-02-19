#ifndef VERTEX_H
#define VERTEX_H

#include <QPen>
#include <QGraphicsItem>

class Line;

//This class represents a vertex of a graph that will be painted on
//the view as a dot.
class Vertex : public QGraphicsEllipseItem
{
public:
    enum VertexParameters {
        VertexSize = 10,
        VertexZValue = 10
    };
    Vertex();
    Vertex(const QString &name, const QPointF &position,
           const QPen &pen = QPen(), const QBrush &brush = QBrush());
    Vertex(const QJsonObject &json);

    inline void setName(const QString &name) { this->name = name; }
    inline QString getName() const { return name; }
    //This function set the center of a vertex position.
    inline void setPosition(const QPointF &position)
    {
        setRect(position.x() - rect().width() / 2,
                position.y() - rect().height() / 2,
                VertexSize, VertexSize);
    }
    inline QPointF getPosition() const { return rect().center(); }

    inline void addNearLine(Line *line) const { nearLines.append(line); }
    inline void removeNearLine(Line *line) const { nearLines.removeOne(line); }

    //This function converts vertex to JSON object.
    QJsonObject toJson() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;

private:
    //Name of a vertex.
    QString name;
    //Lines that connects to this vertex.
    mutable QList<Line *> nearLines;
};

#endif // VERTEX_H
