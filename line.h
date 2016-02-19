#ifndef LINE_H
#define LINE_H

#include <QGraphicsItem>
#include <QPen>
#include "vertex.h"

class Graph;

//This class represents an edge of a graph that will be painted
//on a view as a line. It also connects two different vertices of a graph.
class Line : public QGraphicsLineItem
{
public:
    enum LineParameters {
        LineZValue = 0,
        LineHighlightedZValue = 1
    };

    Line(int weight, const Vertex &vertex1, const Vertex &vertex2, const QPen &pen = QPen());
    Line(const QJsonObject &json, const Graph &graph);

    //Getter and setter for edge's weight.
    inline void setWeight(int weight) { this->weight = weight; }
    inline int getWeight() const { return weight; }

    //Getter and setter for first vertex of a line.
    inline void setVertex1(const Vertex &vertex1)
    {
        this->vertex1 = &vertex1;
        setLine(QLineF(vertex1.getPosition(), line().p2()));
    }
    inline const Vertex *getVertex1() const { return vertex1; }

    //Getter and setter fot second vertex of a line.
    inline void setVertex2(const Vertex &vertex2)
    {
        this->vertex2 = &vertex2;
        setLine(QLineF(line().p1(), vertex2.getPosition()));
    }
    inline const Vertex *getVertex2() const { return vertex2; }

    //This function converts this line to JSON object.
    QJsonObject toJson() const;

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;

private:
    //Number that represents a weight of an edge.
    int weight;
    //First and second vertices that a line connects.
    const Vertex *vertex1;
    const Vertex *vertex2;
};

#endif // LINE_H
