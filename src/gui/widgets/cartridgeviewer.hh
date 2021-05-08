#pragma once

#include "../../debugger/debugcore.hh"

#include <QFrame>
#include <QLabel>

class CartridgeViewer : public QFrame, iDebugObserver
{
public:
    QLabel* nintendo_logo_ok_label = nullptr;
    QLabel* nintendo_logo_ok_value = nullptr;
    QLabel* title_label = nullptr;
    QLabel* title_value = nullptr;
    QLabel* manufacturer_label = nullptr;
    QLabel* manufacturer_value = nullptr;
    QLabel* cgb_flag_label = nullptr;
    QLabel* cgb_flag_value = nullptr;
    QLabel* new_licensee_label = nullptr;
    QLabel* new_licensee_value = nullptr;
    QLabel* sgb_flag_label = nullptr;
    QLabel* sgb_flag_value = nullptr;
    QLabel* cart_type_label = nullptr;
    QLabel* cart_type_value = nullptr;
    QLabel* rom_size_label = nullptr;
    QLabel* rom_size_value = nullptr;
    QLabel* ram_size_label = nullptr;
    QLabel* ram_size_value = nullptr;
    QLabel* dest_code_label = nullptr;
    QLabel* dest_code_value = nullptr;
    QLabel* old_licensee_label = nullptr;
    QLabel* old_licensee_value = nullptr;
    QLabel* version_number_label = nullptr;
    QLabel* version_number_value = nullptr;
    QLabel* header_checksum_label = nullptr;
    QLabel* header_checksum_value = nullptr;
    QLabel* global_checksum_label = nullptr;
    QLabel* global_checksum_value = nullptr;

    DebugCore* debugger;

    CartridgeViewer(DebugCore* debugger, QWidget* parent = nullptr);

    void update_fields();

    virtual void debug_event(const DebugEvent* event) override;
};
