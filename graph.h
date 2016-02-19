#ifndef GRAPH_H
#define GRAPH_H

#include <QList>
#include <QGraphicsScene>
#include "vertex.h"
#include "line.h"

//This class represents a graph that contains vertices and lines.
//It provides an ability to change its structure by adding or deleting
//elements to the graph. It can convert graph structure to/from JSON
//format to provide saving/opening the graph. Graph module also provides
//finding a subgraph that includes all vertices of the graph (aka a
//spanning tree).
class Graph
{
public:
    enum Limits {
        VertexBorderThickness = 1,
        MinVertexCount = 2,
        LineThickness = 3,
        FontOffset = 5,
        MaxLineWeight = 10,
        FontSize = 10,
        MaxVertexCount = 1000
    };

    Graph();
    Graph(const QJsonObject &json);
    ~Graph();

    //This function adds a new vertex to scene and to the list of vertices.
    void addVertex(Vertex *vertex);
    //This function removes the vertex.
    void removeVertex(Vertex *vertex);
    //This function adds a new line to the scene and to the list of lines.
    void addLine(Line *line);
    //This function removes the line.
    void removeLine(Line *line);

    //This function returns a list of lines that represent a spanning tree.
    inline const QList<Line *> &getSpanningTree() const { return *spanningTree; }
    //This function highlights the spanning tree.
    void showSpanningTree();

    //This function returns a pointer to current scene.
    inline QGraphicsScene *getScene() { return scene; }
    //This function returns a vertex by index in the list of vertices.
    inline const Vertex &getVertexAt(int index) const { return *(vertices->at(index)); }
    const Vertex * getVertexByName(const QString &name) const;

    //This function converts this graph to JSON object.
    QJsonObject toJson() const;

private:
    //This function finds a spanning tree and fills an appropriate list.
    void findSpanningTree();
    //This function checks if the list contains line where its vertices have same names.
    static bool contains(const QList<Line *> &list, const Line &line);
    //This function checks if the spanning tree contains the vertex.
    bool treeContains(const Vertex &vertex) const;

    QGraphicsScene *scene;
    QList<Vertex *> *vertices;
    QList<Line *> *lines;
    QList<Line *> *spanningTree;
};

#endif // GRAPH_H
