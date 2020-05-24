#include "fileio.hh"

#include <QFile>

namespace antgb
{

void load_rom(QString& filepath, uint8_t* memory)
{
    if (filepath.size() == 0) return;

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        assert(false && "Couldn't open ROM file");
    }

    QByteArray executable = file.readAll();
    memcpy(memory, executable.data(), (size_t)executable.size() - 1);
}

} // namespace antgb
