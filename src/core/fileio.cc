#include "fileio.hh"

#include <QFile>

void load_rom(QString& filepath, uint8_t* memory)
{
    if (filepath.size() == 0) return;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        assert(false && "Couldn't open ROM file");
    }

    QByteArray executable(file.readAll());
    assert(executable.size() != 0);
    memcpy(memory, executable.data(), (size_t)executable.size() - 1);
}
