#include "cartridgeviewer.hh"

#include "../util/fonts.hh"
#include "../util/cartridge.hh"
#include <QFormLayout>
#include <QStyle>

// static const QString VALID_COLOR("color: #FFFFFF");
static const QString VALID_COLOR("color: #93C663");
static const QString INVALID_COLOR("color: #C4464A");
static const QString NEUTRAL_COLOR("color: #FFFFFF");

CartridgeViewer::CartridgeViewer(DebugCore* debugger, QWidget* parent):
    QFrame(parent),
    debugger(debugger)
{
    debugger->add_observer(this);

    auto widget_title_label = new QLabel("CARTRIDGE HEADER", this);
    widget_title_label->setFont(*Fonts::COURIER);

    auto placeholder_value = new QLabel("", this);

    nintendo_logo_ok_label = new QLabel(" Nintendo logo valid:", this);
    nintendo_logo_ok_value = new QLabel("", this);
    nintendo_logo_ok_label->setFont(*Fonts::COURIER);
    nintendo_logo_ok_value->setFont(*Fonts::COURIER);
    nintendo_logo_ok_value->setAlignment(Qt::AlignRight);

    title_label = new QLabel(" Title:", this);
    title_value = new QLabel("", this);
    title_label->setFont(*Fonts::COURIER);
    title_value->setFont(*Fonts::COURIER);
    title_value->setAlignment(Qt::AlignRight);

    manufacturer_label = new QLabel(" Manufacturer code:", this);
    manufacturer_value = new QLabel("", this);
    manufacturer_label->setFont(*Fonts::COURIER);
    manufacturer_value->setFont(*Fonts::COURIER);
    manufacturer_value->setAlignment(Qt::AlignRight);

    cgb_flag_label = new QLabel(" CGB flag:", this);
    cgb_flag_value = new QLabel("", this);
    cgb_flag_label->setFont(*Fonts::COURIER);
    cgb_flag_value->setFont(*Fonts::COURIER);
    cgb_flag_value->setAlignment(Qt::AlignRight);

    new_licensee_label = new QLabel(" New licensee code:", this);
    new_licensee_value = new QLabel("", this);
    new_licensee_label->setFont(*Fonts::COURIER);
    new_licensee_value->setFont(*Fonts::COURIER);
    new_licensee_value->setAlignment(Qt::AlignRight);

    sgb_flag_label = new QLabel(" SGB flag:", this);
    sgb_flag_value = new QLabel("", this);
    sgb_flag_label->setFont(*Fonts::COURIER);
    sgb_flag_value->setFont(*Fonts::COURIER);
    sgb_flag_value->setAlignment(Qt::AlignRight);

    cart_type_label = new QLabel(" Cartridge type:", this);
    cart_type_value = new QLabel("", this);
    cart_type_label->setFont(*Fonts::COURIER);
    cart_type_value->setFont(*Fonts::COURIER);
    cart_type_value->setAlignment(Qt::AlignRight);

    rom_size_label = new QLabel(" ROM size:", this);
    rom_size_value = new QLabel("", this);
    rom_size_label->setFont(*Fonts::COURIER);
    rom_size_value->setFont(*Fonts::COURIER);
    rom_size_value->setAlignment(Qt::AlignRight);

    ram_size_label = new QLabel(" RAM size:", this);
    ram_size_value = new QLabel("", this);
    ram_size_label->setFont(*Fonts::COURIER);
    ram_size_value->setFont(*Fonts::COURIER);
    ram_size_value->setAlignment(Qt::AlignRight);

    dest_code_label = new QLabel(" Destination code:", this);
    dest_code_value = new QLabel("", this);
    dest_code_label->setFont(*Fonts::COURIER);
    dest_code_value->setFont(*Fonts::COURIER);
    dest_code_value->setAlignment(Qt::AlignRight);

    old_licensee_label = new QLabel(" Old licensee code:", this);
    old_licensee_value = new QLabel("", this);
    old_licensee_label->setFont(*Fonts::COURIER);
    old_licensee_value->setFont(*Fonts::COURIER);
    old_licensee_value->setAlignment(Qt::AlignRight);

    version_number_label = new QLabel(" Version number:", this);
    version_number_value = new QLabel("", this);
    version_number_label->setFont(*Fonts::COURIER);
    version_number_value->setFont(*Fonts::COURIER);
    version_number_value->setAlignment(Qt::AlignRight);

    header_checksum_label = new QLabel(" Header checksum:", this);
    header_checksum_value = new QLabel("", this);
    header_checksum_label->setFont(*Fonts::COURIER);
    header_checksum_value->setFont(*Fonts::COURIER);
    header_checksum_value->setAlignment(Qt::AlignRight);

    global_checksum_label = new QLabel(" Global checksum:", this);
    global_checksum_value = new QLabel("", this);
    global_checksum_label->setFont(*Fonts::COURIER);
    global_checksum_value->setFont(*Fonts::COURIER);
    global_checksum_value->setAlignment(Qt::AlignRight);

    auto layout = new QFormLayout(this);
    layout->addRow(widget_title_label, placeholder_value);
    layout->addRow(nintendo_logo_ok_label, nintendo_logo_ok_value);
    layout->addRow(title_label, title_value);
    layout->addRow(manufacturer_label, manufacturer_value);
    layout->addRow(new_licensee_label, new_licensee_value);
    layout->addRow(old_licensee_label, old_licensee_value);
    layout->addRow(cgb_flag_label, cgb_flag_value);
    layout->addRow(sgb_flag_label, sgb_flag_value);
    layout->addRow(cart_type_label, cart_type_value);
    layout->addRow(rom_size_label, rom_size_value);
    layout->addRow(ram_size_label, ram_size_value);
    layout->addRow(dest_code_label, dest_code_value);
    layout->addRow(version_number_label, version_number_value);
    layout->addRow(header_checksum_label, header_checksum_value);
    layout->addRow(global_checksum_label, global_checksum_value);

    setStyleSheet("background-color: #232629");
}

void CartridgeViewer::update_fields()
{
    Cartridge* cart = static_cast<Emulator*>(debugger->emu())->cartridge;

    QString as_hex = "";

    bool logo_valid = cart->is_nintendo_logo_valid();

    if (logo_valid)
    {
        nintendo_logo_ok_value->setText("Yes");
        nintendo_logo_ok_value->setToolTip("");
        nintendo_logo_ok_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        nintendo_logo_ok_value->setText("No");
        nintendo_logo_ok_value->setToolTip("The Nintendo logo at the beginning"
                                           " of the header is invalid"
                                           "\nA real Game Boy would"
                                           " lock up on boot");
        nintendo_logo_ok_value->setStyleSheet(INVALID_COLOR);
    }

    title_value->setText(cart->title().c_str());
    title_value->setToolTip("");
    title_value->setStyleSheet(VALID_COLOR);

    uint32_t manufacturer_code = cart->manufacturer_code();
    as_hex = QString::number(manufacturer_code, 16);
    manufacturer_value->setText("Unknown (0x" + as_hex + ")");
    manufacturer_value->setToolTip("");
    manufacturer_value->setStyleSheet(NEUTRAL_COLOR);

    uint8_t cgb_flag = cart->cgb_flag();
    as_hex = QString::number(cgb_flag, 16);
    if (!cart->has_cgb_flag())
    {
        cgb_flag_value->setText("-");
        cgb_flag_value->setToolTip("");
        cgb_flag_value->setStyleSheet(NEUTRAL_COLOR);
    }
    else
    {

        if (cgb_flag == Cartridge::CartCgbOnly)
        {
            cgb_flag_value->setText("CGB only (0x" + as_hex + ")");
            cgb_flag_value->setToolTip("");
            cgb_flag_value->setStyleSheet(VALID_COLOR);
        }
        else if (cgb_flag == Cartridge::CartCgbSupported)
        {
            cgb_flag_value->setText("CGB supported (0x" + as_hex + ")");
            cgb_flag_value->setToolTip("");
            cgb_flag_value->setStyleSheet(VALID_COLOR);
        }
        else
        {
            cgb_flag_value->setText("Invalid (0x" + as_hex + ")");
            cgb_flag_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
            cgb_flag_value->setStyleSheet(INVALID_COLOR);
        }
    }

    auto new_licensee_code = static_cast<NewLicensee>(cart->new_licensee_code());
    as_hex = QString::number(static_cast<uint8_t>(new_licensee_code), 16);

    new_licensee_code = static_cast<NewLicensee>(NewLicensee::TokumaShoten);

    if (!cart->has_new_licensee())
    {
        new_licensee_value->setText("-");
        new_licensee_value->setToolTip("This cartridge has an old licensee");
        new_licensee_value->setStyleSheet(NEUTRAL_COLOR);
    }
    else
    {
        if (NEW_LICENSEE.count(new_licensee_code))
        {
            new_licensee_value->setText(QString(NEW_LICENSEE.at(new_licensee_code)) + " (0x" + as_hex + ")");
            new_licensee_value->setToolTip("");
            new_licensee_value->setStyleSheet(VALID_COLOR);
        }
        else
        {
            new_licensee_value->setText("Unknown (0x" + as_hex + ")");
            new_licensee_value->setToolTip("");
            new_licensee_value->setStyleSheet(NEUTRAL_COLOR);
        }
    }

    uint8_t sgb_flag = cart->sgb_flag();
    as_hex = QString::number(sgb_flag);
    if (sgb_flag == Cartridge::CartNoSgbFeatures)
    {
        sgb_flag_value->setText("No SGB features (0x" + as_hex + ")");
        sgb_flag_value->setToolTip("");
        sgb_flag_value->setStyleSheet(VALID_COLOR);
    }
    else if (sgb_flag == Cartridge::CartHasSgbFeatures)
    {
        sgb_flag_value->setText("Has SGB features (0x" + as_hex + ")");
        sgb_flag_value->setToolTip("");
        sgb_flag_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        sgb_flag_value->setText("Invalid (0x" + as_hex + ")");
        sgb_flag_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
        sgb_flag_value->setStyleSheet(INVALID_COLOR);
    }

    auto cart_type = static_cast<Cartridge::CartridgeType>(cart->cart_type());

    as_hex = QString::number(cart_type, 16);
    if (CART_TYPE_STR.count(cart_type))
    {
        cart_type_value->setText(QString(CART_TYPE_STR.at(cart_type)) + " (0x" + as_hex + ")");
        cart_type_value->setToolTip("");
        cart_type_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        cart_type_value->setText("Invalid (0x" + as_hex + ")");
        cart_type_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
        cart_type_value->setStyleSheet(INVALID_COLOR);
    }

    auto rom_size = static_cast<Cartridge::RomSize>(cart->rom_size());
    as_hex = QString::number(static_cast<uint8_t>(rom_size), 16);
    if (ROM_SIZE_STR.count(rom_size))
    {
        rom_size_value->setText(QString(ROM_SIZE_STR.at(rom_size)) + " (0x" + as_hex + ")");
        rom_size_value->setToolTip("");
        rom_size_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        rom_size_value->setText("Invalid (0x" + as_hex + ")");
        rom_size_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
        rom_size_value->setStyleSheet(INVALID_COLOR);
    }

    auto ram_size = static_cast<Cartridge::RamSize>(cart->ram_size());
    as_hex = QString::number(static_cast<uint8_t>(ram_size), 16);
    if (RAM_SIZE_STR.count(ram_size))
    {
        ram_size_value->setText(QString(RAM_SIZE_STR.at(ram_size)) + " (0x" + as_hex + ")");
        ram_size_value->setToolTip("");
        ram_size_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        ram_size_value->setText("Invalid (0x" + as_hex + ")");
        ram_size_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
        ram_size_value->setStyleSheet(INVALID_COLOR);
    }

    auto dest_code = static_cast<Cartridge::DestinationCode>(cart->destination_code());
    as_hex = QString::number(static_cast<uint8_t>(dest_code), 16);
    if (dest_code == Cartridge::DestinationCode::CartJapanese)
    {
        dest_code_value->setText("Japanese (0x" + as_hex + ")");
        dest_code_value->setToolTip("");
        dest_code_value->setStyleSheet(VALID_COLOR);
    }
    else if (dest_code == Cartridge::DestinationCode::CartNonJapanese)
    {
        dest_code_value->setText("Non-Japanese (0x" + as_hex + ")");
        dest_code_value->setToolTip("");
        dest_code_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        dest_code_value->setText("Invalid (0x" + as_hex + ")");
        dest_code_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
        dest_code_value->setStyleSheet(INVALID_COLOR);
    }

    auto old_licensee_code = static_cast<OldLicensee>(cart->old_licensee_code());
    as_hex = QString::number(static_cast<uint8_t>(old_licensee_code), 16);
    if (!cart->has_old_licensee())
    {
        old_licensee_value->setText("-");
        old_licensee_value->setText("This header has a new licensee");
        old_licensee_value->setStyleSheet(NEUTRAL_COLOR);
    }
    else
    {
        if (OLD_LICENSEE.count(old_licensee_code))
        {
            old_licensee_value->setText(QString(OLD_LICENSEE.at(old_licensee_code)) + " (0x" + as_hex + ")");
            old_licensee_value->setToolTip("");
            old_licensee_value->setStyleSheet(VALID_COLOR);
        }
        else
        {
            old_licensee_value->setText("Unknown (0x" + as_hex + ")");
            old_licensee_value->setToolTip("Unrecognized value (0x" + as_hex + ")");
            old_licensee_value->setStyleSheet(NEUTRAL_COLOR);
        }
        old_licensee_value->setStyleSheet(VALID_COLOR);
    }

    uint8_t version_number = cart->version_number();
    as_hex = QString::number(version_number, 16);
    version_number_value->setText(QString::number(version_number) + " (0x" + as_hex + ")");
    version_number_value->setToolTip("");
    version_number_value->setStyleSheet(VALID_COLOR);

    uint8_t header_checksum = cart->compute_header_checksum();
    uint8_t expected_header_checksum = cart->header_checksum();
    as_hex = QString::number(header_checksum, 16);
    if (cart->header_matches_checksum())
    {
        header_checksum_value->setText("0x" + as_hex);
        header_checksum_value->setToolTip("");
        header_checksum_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        header_checksum_value->setText("0x" + as_hex);
        header_checksum_value->setToolTip("The header checksum is invalid (0x" + as_hex + ")"
                                          "\nExpected 0x"
                                          + QString::number(expected_header_checksum, 16)
                                          + "\nA real Game boy would lock up on boot");
        header_checksum_value->setStyleSheet(INVALID_COLOR);
    }

    uint16_t global_checksum = cart->compute_global_checksum();
    uint16_t expected_global_checksum = cart->global_checksum();
    as_hex = QString::number(global_checksum, 16);
    if (global_checksum == expected_global_checksum)
    {
        global_checksum_value->setText("0x" + as_hex);
        global_checksum_value->setToolTip("");
        global_checksum_value->setStyleSheet(VALID_COLOR);
    }
    else
    {
        global_checksum_value->setText("0x" + as_hex);
        global_checksum_value->setToolTip("The global checksum is invalid (0x" + as_hex + ")"
                                          "\nExpected 0x"
                                          + QString::number(expected_global_checksum, 16)
                                          + "\nA real Game Boy would not care");
        global_checksum_value->setStyleSheet(INVALID_COLOR);
    }
}

void CartridgeViewer::debug_event(const DebugEvent* event)
{
    switch (event->type())
    {
        case DbRomLoaded:
            update_fields();
            break;
        default:
            break;
    }
}
