#include "types.hh"

#include "../core/emulator.hh"
#include <QMainWindow>

namespace antgb
{
class MainWindow : public QMainWindow
{
public:
    MainWindow();
    virtual ~MainWindow() override;
    void start_emulation();

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    void emulate();

    bool           _keep_emulating;
    Emulator*      _emulator;
    DisplayWidget* _displaywidget;
    QThread*       _emulation_thread;
};

} // namespace antgb
