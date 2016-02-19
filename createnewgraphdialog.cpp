#include "createnewgraphdialog.h"
#include "graph.h"
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QLayout>

CreateNewGraphDialog::CreateNewGraphDialog(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("Vertex count")), 0, 0);
    vertexCountSpinBox = new QSpinBox;
    vertexCountSpinBox->setMinimum(Graph::MinVertexCount);
    vertexCountSpinBox->setMaximum(Graph::MaxVertexCount);
    mainLayout->addWidget(vertexCountSpinBox, 0, 1);
    mainLayout->addWidget(new QLabel(tr("Line count")), 1, 0);
    lineCountSpinBox = new QSpinBox;
    lineCountSpinBox->setMinimum(0);
    lineCountSpinBox->setValue(1);
    mainLayout->addWidget(lineCountSpinBox, 1, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout, 2, 0, 1, 2);
    buttonLayout->addStretch();
    QPushButton *okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    buttonLayout->addWidget(okButton);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    buttonLayout->addWidget(cancelButton);
}
