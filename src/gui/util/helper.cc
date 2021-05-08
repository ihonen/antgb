#include "helper.hh"

QString escape_regex_special_chars(const QString& string)
{
    QString copy(string);
    copy.replace("$", "\\$");
    return copy;
}
