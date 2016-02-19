#ifndef CREATENEWGRAPHDIALOG_H
#define CREATENEWGRAPHDIALOG_H

#include <QDialog>
#include <QSpinBox>

//This class represents a window where user can get started with creating a new graph
//by entering count of lines and vertices.
class CreateNewGraphDialog : public QDialog
{
public:
    CreateNewGraphDialog(QWidget *parent);

    inline int getVertexCount() const { return vertexCountSpinBox->value(); }
    inline int getLineCount() const { return lineCountSpinBox->value(); }

private:
    QSpinBox *vertexCountSpinBox;
    QSpinBox *lineCountSpinBox;
};

#endif // CREATENEWGRAPHDIALOG_H
