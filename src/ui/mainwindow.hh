#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QLabel>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    void init_menubar();

    QGraphicsView* display_view_;
    QHBoxLayout* main_layout_;
    QLabel* debug_window_;
    QMenuBar* menubar_;

    QMenu* file_menu_;
    QAction* load_rom_act_;

    QMenu* options_menu_;
};

#endif // MAINWINDOW_HH
