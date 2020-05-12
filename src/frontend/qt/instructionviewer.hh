#pragma once

#include "instructionmodel.hh"
#include "instructionview.hh"
#include "../../debugger/debugcore.hh"
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>

class InstructionViewer : public QFrame
{
    Q_OBJECT
public slots:
    void search();
public:
    QLineEdit* search_line = nullptr;
    InstructionView* view = nullptr;
    QPushButton* settings_button = nullptr;

    InstructionViewer(DebugCore* debugger, InstructionModel* model, QWidget* parent = nullptr);
    void update();
};
