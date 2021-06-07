#include "ppu.hh"

#include "debugger/ifrontend.hh"


#include "addresses.hh"
#include "bitmanip.hh"
#include "memorybus.hh"
#include "ppuregisters.hh"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>

//----------------------------------------------------------------------------
// Tile
//----------------------------------------------------------------------------

struct Tile
{
    Tile();
    uint8_t get_pixel(uint8_t tile_x, uint8_t tile_y);

    static const uint8_t WIDTH  = 8;
    static const uint8_t HEIGHT = 8;

    uint8_t data[16];
};

uint8_t Tile::get_pixel(uint8_t tile_x, uint8_t tile_y)
{
    if (tile_x > 7 || tile_y > 7)
    {
        return 0x00;
    }

    uint8_t* row_low  = &data[tile_y * 2];
    uint8_t* row_high = row_low + 1;
    uint8_t  low_bit  = (*row_low & (0x01 << (7 - tile_x))) >> ((7 - tile_x));
    uint8_t  high_bit = (*row_high & (0x01 << (7 - tile_x))) >> ((7 - tile_x));
    return (high_bit << 1) | low_bit;
}

//----------------------------------------------------------------------------
// Background
//----------------------------------------------------------------------------

class Background
{
public:
    enum class Type
    {
        Background,
        Window
    };

    static const size_t BG_HEIGHT_PIXELS = 256;
    static const size_t BG_WIDTH_PIXELS  = 256;
    static const size_t HEIGHT_TILES     = 32;
    static const size_t WIDTH_TILES      = 32;

    static const memaddr_t TILE_DATA_BLOCK0_BASE = 0x8000;
    static const memaddr_t TILE_DATA_BLOCK2_BASE = 0x9000;
    static const memaddr_t TILE_DATA_BASE[2];

    static const memaddr_t TILE_MAP0_BASE = 0x9800;
    static const memaddr_t TILE_MAP1_BASE = 0x9C00;
    static const memaddr_t TILE_MAP_BASE[2];

    Background(Type type, MemoryBus* memory);
    void     set_memory(uint8_t* memory);
    uint8_t  get_pixel_at(size_t display_x, size_t display_y);
    uint8_t  get_pixel(size_t background_x, size_t background_y);
    memaddr_t   tile_map_address();
    uint8_t* tile_map_base();
    memaddr_t   tile_data_address();
    Tile*    tile_data_base();
    bool     includes(size_t display_x, size_t display_y);
    size_t   top();
    size_t   bottom();
    size_t   left();
    size_t   right();
    bool     is_enabled();

    MemoryBus* mem;
    Type type;
};

const memaddr_t Background::TILE_DATA_BASE[2] = {Background::TILE_DATA_BLOCK2_BASE,
                                              Background::TILE_DATA_BLOCK0_BASE};

const memaddr_t Background::TILE_MAP_BASE[2] = {Background::TILE_MAP0_BASE,
                                             Background::TILE_MAP1_BASE};

Background::Background(Type type_, MemoryBus* memory)
{
    mem  = memory;
    type = type_;
}

uint8_t Background::get_pixel_at(size_t display_x, size_t display_y)
{
    assert(includes(display_x, display_y));
    return get_pixel(display_x - left(), display_y - top());
}

uint8_t Background::get_pixel(size_t background_x, size_t background_y)
{
    if (type == Type::Background)
    {
        background_x %= BG_HEIGHT_PIXELS;
        background_y %= BG_WIDTH_PIXELS;
    }

    uint8_t tile_x      = background_x / Tile::WIDTH;
    uint8_t tile_y      = background_y / Tile::HEIGHT;
    uint8_t tile_number = tile_map_base()[tile_y * WIDTH_TILES + tile_x];

    Tile* tile = nullptr;
    switch (tile_data_address())
    {
    case TILE_DATA_BLOCK0_BASE: tile = &tile_data_base()[tile_number]; break;
    case TILE_DATA_BLOCK2_BASE:
        tile = &tile_data_base()[(int8_t)tile_number];
        break;
    default: assert(false && "ILLEGAL TILE DATA BASE ADDRESS"); break;
    }

    return tile->get_pixel(background_x % Tile::WIDTH,
                           background_y % Tile::HEIGHT);
}

memaddr_t Background::tile_map_address()
{
    if (type == Type::Background)
    {
        return TILE_MAP_BASE[get_bit(mem->get(LCDC_ADDR), PpuRegisters::BgTileMapDisplaySelect)];
    }

    return TILE_MAP_BASE[get_bit(mem->get(LCDC_ADDR), PpuRegisters::WindowTileMapDisplaySelect)];
}

uint8_t* Background::tile_map_base()
{
    return mem->get(tile_map_address());
}

memaddr_t Background::tile_data_address()
{
    return TILE_DATA_BASE[get_bit(mem->get(LCDC_ADDR), PpuRegisters::BgAndWindowTileDataSelect)];
}

Tile* Background::tile_data_base()
{
    return (Tile*)mem->get(tile_data_address());
}

bool Background::includes(size_t display_x, size_t display_y)
{
    return left() <= display_x && right() >= display_x && top() <= display_y
           && bottom() >= display_y;
}

size_t Background::top()
{
    if (type == Type::Background)
    {
        return *mem->get(SCY_ADDR);
    }

    return *mem->get(WY_ADDR);
}

size_t Background::bottom()
{
    if (type == Type::Background)
    {
        return (*mem->get(SCY_ADDR) + 144 - 1) % BG_HEIGHT_PIXELS;
    }

    return 144;
}

size_t Background::left()
{
    if (type == Type::Background)
    {
        return *mem->get(SCX_ADDR);
    }

    return *mem->get(WX_ADDR);
}

size_t Background::right()
{
    if (type == Type::Background)
    {
        return (*mem->get(SCX_ADDR) + 160 - 1) % BG_WIDTH_PIXELS;
    }

    return 160;
}

bool Background::is_enabled()
{
    if (type == Type::Background)
    {
        return get_bit(mem->get(LCDC_ADDR), PpuRegisters::BgAndWindowDisplayEnable);
    }

    return get_bit(mem->get(LCDC_ADDR), PpuRegisters::BgAndWindowDisplayEnable)
           && get_bit(mem->get(LCDC_ADDR), PpuRegisters::WindowDisplayEnable);
}

//----------------------------------------------------------------------------
// Sprite
//----------------------------------------------------------------------------

class Sprite
{
public:
    typedef struct __attribute__((packed))
    {
        uint8_t y_pos;
        uint8_t x_pos;
        uint8_t tile_number;
        struct __attribute__((packed))
        {
            uint8_t __unused_in_dmg_1 : 1;
            uint8_t __unused_in_dmg_2 : 3;
            uint8_t palette_number : 1;
            uint8_t x_flip : 1;
            uint8_t y_flip : 1;
            uint8_t obj_to_bg_priority : 1;
        } flags;
    } Attribute;

    uint8_t top();
    uint8_t bottom();
    uint8_t left();
    uint8_t right();
    bool    includes(uint8_t display_row, uint8_t display_column);
    bool    includes_row(uint8_t display_row);
    bool    includes_column(uint8_t display_column);
    bool    is_offscreen();
    uint8_t get_pixel_at(uint8_t display_x, uint8_t display_y);
    uint8_t get_pixel(uint8_t sprite_x, uint8_t sprite_y);
    bool    is_above_background();
    bool    is_x_mirrored();
    bool    is_y_mirrored();
    uint8_t palette_number();

    uint8_t    width;
    uint8_t    height;
    Tile*      data;
    Attribute* attribute;
};

uint8_t Sprite::top()
{
    return attribute->y_pos - 16;
}

uint8_t Sprite::bottom()
{
    return top() + height - 1;
}

uint8_t Sprite::left()
{
    return attribute->x_pos - 8;
}

uint8_t Sprite::right()
{
    return left() + width - 1;
}

bool Sprite::includes(uint8_t display_row, uint8_t display_column)
{
    return includes_row(display_row) && includes_column(display_column);
}

bool Sprite::includes_row(uint8_t display_row)
{
    return display_row >= top() && display_row <= bottom();
}

bool Sprite::includes_column(uint8_t display_column)
{
    return display_column >= left() && display_column <= right();
}

bool Sprite::is_offscreen()
{
    return left() >= 160 && top() >= 144;
}

uint8_t Sprite::get_pixel_at(uint8_t display_x, uint8_t display_y)
{
    if (!includes_row(display_y) || !includes_column(display_x)
        || is_offscreen())
    {
        return 0x00;
    }

    return get_pixel(display_x - left(), display_y - top());
}

uint8_t Sprite::get_pixel(uint8_t sprite_x, uint8_t sprite_y)
{
    if (sprite_x >= width || sprite_y >= height)
    {
        return 0x00;
    }

    if (is_x_mirrored())
    {
        sprite_x = (width - 1) - sprite_x;
    }

    if (is_y_mirrored())
    {
        sprite_y = (height - 1) - sprite_y;
    }

    uint8_t tile_y       = sprite_y;
    Tile*   pointed_tile = data;

    if (sprite_y >= 8)
    {
        tile_y -= 8;
        ++pointed_tile;
    }

    return pointed_tile->get_pixel(sprite_x, tile_y);
}

bool Sprite::is_above_background()
{
    return attribute->flags.obj_to_bg_priority == 0;
}

bool Sprite::is_x_mirrored()
{
    return attribute->flags.x_flip != 0;
}

bool Sprite::is_y_mirrored()
{
    return attribute->flags.y_flip != 0;
}

uint8_t Sprite::palette_number()
{
    return attribute->flags.palette_number;
}

//----------------------------------------------------------------------------
// Sprites
//----------------------------------------------------------------------------

class Sprites
{
public:
    Sprites(MemoryBus* memory);
    virtual ~Sprites();
    void               set_memory(uint8_t* memory);
    void               refresh();
    memaddr_t             sprite_attributes_address();
    Sprite::Attribute* sprite_attributes_base();
    memaddr_t             sprite_data_address();
    Tile*              sprite_data_base();
    uint8_t            sprite_height();
    bool               enabled();
    void    assemble_sprite_info(Sprite& sprite, Sprite::Attribute* attributes);
    void    sort();
    Sprite* get_sprite_at_x(size_t display_x);

    std::array<Sprite, 10> sprite_buffer;
    size_t                 sprite_buffer_size;

    MemoryBus* mem;
};

static bool sprite_priority_comp(const Sprite& a, const Sprite& b)
{
    if (a.attribute->x_pos != b.attribute->x_pos)
    {
        return a.attribute->x_pos < b.attribute->x_pos;
    }

    return a.attribute->tile_number <= b.attribute->tile_number;
}

Sprites::Sprites(MemoryBus* memory)
{
    mem                = memory;
    sprite_buffer_size = 0;
}

Sprites::~Sprites() {}

void Sprites::refresh()
{
    sprite_buffer_size = 0;

    for (size_t i = 0; i < 40; ++i)
    {
        auto sprite_attributes = &sprite_attributes_base()[i];

        /*
        if (sprite_attributes->y_pos != 0)
        {
            assert(!sprite_buffer_size);
            cout << sprite_buffer_size << endl;
            cout << std::hex << (int)*ppureg->ly << endl;
            cout << std::hex << (int)sprite_attributes->y_pos << endl;
            cout << endl;
        }
        */

        if (sprite_attributes->y_pos - 16 <= mem->read(LY_ADDR)
            && sprite_attributes->y_pos - 9 /*- 16 + sprite_height()*/ >= mem->read(LY_ADDR))
        {
            Sprite& sprite = sprite_buffer[sprite_buffer_size];
            assemble_sprite_info(sprite, sprite_attributes);
            ++sprite_buffer_size;
        }
    }

    //sort();
}

memaddr_t Sprites::sprite_attributes_address()
{
    return 0xFE00;
}

Sprite::Attribute* Sprites::sprite_attributes_base()
{
    return (Sprite::Attribute*)(mem->get(sprite_attributes_address()));
}

memaddr_t Sprites::sprite_data_address()
{
    return 0x8000;
}

Tile* Sprites::sprite_data_base()
{
    return (Tile*)(mem->get(sprite_data_address()));
}

uint8_t Sprites::sprite_height()
{
    return get_bit(mem->get(LCDC_ADDR), PpuRegisters::ObjSize) ? 16 : 8;
}

bool Sprites::enabled()
{
    return get_bit(mem->get(LCDC_ADDR), PpuRegisters::ObjDisplayEnable);
}

void Sprites::assemble_sprite_info(Sprite& sprite, Sprite::Attribute* attributes)
{
    sprite.attribute = attributes;
    sprite.width     = 8;
    sprite.height    = sprite_height();
    sprite.data      = &sprite_data_base()[sprite.attribute->tile_number];
}

void Sprites::sort()
{
    std::sort(sprite_buffer.begin(), sprite_buffer.begin() + sprite_buffer_size,
              sprite_priority_comp);
}

Sprite* Sprites::get_sprite_at_x(size_t display_x)
{
    for (size_t i = 0; i < sprite_buffer_size; ++i)
    {
        Sprite& sprite = sprite_buffer[i];
        if (sprite.includes_column(display_x))
        {
            return &sprite;
        }
    }

    return nullptr;
}

//----------------------------------------------------------------------------
// Renderer
//----------------------------------------------------------------------------

class Renderer
{
public:
    Renderer(MemoryBus* mem, iFrontend* frontend);
    void set_frontend(iFrontend* frontend);
    void set_memory(uint8_t* mem);
    void render_frame();

    uint32_t* get_frame_buffer();

private:
    uint16_t TILE_DATA_BASE = 0x8000;

    iFrontend* frontend;

    MemoryBus* mem;
    iFrontend::Pixels pixels;

    Background background;
    Background window;
    Sprites sprites;
};

Renderer::Renderer(MemoryBus* memory, iFrontend* renderer_)
    : frontend(renderer_), mem(memory),
      background(Background(Background::Type::Background, mem)),
      window(Background(Background::Type::Window, mem)),
      sprites(Sprites(memory))
{
    memset(pixels.data(), 0x00, 160 * 144);
}

void Renderer::set_frontend(iFrontend* renderer)
{
    frontend = renderer;
}

void Renderer::render_frame()
{
    auto ly_backup = *mem->get(LY_ADDR);

    for (size_t y = 0; y < 144; ++y)
    {
        *mem->get(LY_ADDR) = y;
        sprites.refresh();
        for (size_t x = 0; x < 160; ++x)
        {
            if (background.includes(x, y))
            {
                pixels[x][y] = background.get_pixel_at(x, y);
            }

            /*
            Sprite* sprite = sprites.get_sprite_at_x(x);
            if (sprite)
            {
                frame_buffer[y][x] = sprite->get_pixel_at(x, y);
            }
            */
        }
    }

    *mem->get(LY_ADDR) = ly_backup;

    if (frontend)
    {
        frontend->render(pixels);
    }
}

//----------------------------------------------------------------------------
// Ppu
//----------------------------------------------------------------------------

Ppu::Ppu(MemoryBus* mmu_, PpuRegisters& reg, Cpu* cpu, iFrontend* renderer_)
    : reg(reg), cpu(cpu), mem(mmu_)
{
    renderer = new Renderer(mem, renderer_);
    hard_reset();
}

Ppu::~Ppu()
{
    delete renderer;
}

void Ppu::set_frontend(iFrontend* frontend)
{
    renderer->set_frontend(frontend);
}

void Ppu::hard_reset()
{
    frame_ready              = false;
    cpu_cycles_spent_in_mode = CPU_CYCLES_PER_VBLANK_MODE - 60;
    unemulated_cpu_cycles    = 0;
    // BGB:
    // Cycles until mode changes: 60
    // Cycles until the first LY update: 96.
    cpu_cycles_left_in_mode = 60;
    cpu_cycles_until_ly     = 96;

    current_mode = OamScan;
    clocksum     = 0;
    scanline     = 0;
}

void Ppu::launch_dma(memaddr_t src_address)
{
    reg.write(DMA_ADDR, 0x00);
    mem->launch_oam_dma(0xFE00, src_address, 160);
}

void Ppu::step(uint64_t cpu_cycles)
{
    clocksum += cpu_cycles;

    static uint64_t total_cycles = 0;
    total_cycles += cpu_cycles;

    bool stop = false;

    // Launch DMA if a request has been made.
    if (has_dma_request())
        launch_dma(dma_src_address());

    /*
    mem->hff41_stat &= ~(current_mode & MODE_FLAG_MASK);
    mem->hff41_stat |= current_mode & MODE_FLAG_MASK;
    mode_task_complete = false;

    */


    while (!stop)
    {
        // Update registers.
        reg.write(STAT_ADDR, reg.read(STAT_ADDR) & ~MODE_FLAG_MASK);
        reg.write(STAT_ADDR, reg.read(STAT_ADDR) | (current_mode & MODE_FLAG_MASK));

        // Check or clear LYC interrupt condition.
        if (reg.read(LY_ADDR) == reg.read(LYC_ADDR) && get_bit(reg.get(STAT_ADDR), PpuRegisters::LycInt))
        {
            // TODO: Register interface violations.
            set_bit(reg.get(STAT_ADDR), PpuRegisters::LycCoincidence);
            cerr << "sent LCD STAT IRQ" << endl;
            cpu->request_interrupt(Cpu::LcdStatInt);
        }
        else
            clear_bit(reg.get(STAT_ADDR), PpuRegisters::LycCoincidence);

        switch (current_mode)
        {
        case OamScan:
            if (clocksum >= 80)
            {
                clocksum -= 80;
                current_mode = LineScan;
            }
            else
                stop = true;
            break;

        case LineScan:
            if (clocksum >= 291)
            {
                clocksum -= 291;
                current_mode = Hblank;
                // Hblank interrupt
                if (get_bit(reg.get(STAT_ADDR), PpuRegisters::HBlankInterrupt))
                    cpu->request_interrupt(Cpu::LcdStatInt);
            }
            else
                stop = true;
            break;

        case Hblank:
        {
            if (clocksum >= 85)
            {
                clocksum -= 85;
                if (reg.read(LY_ADDR) < 144)
                {
                    current_mode = OamScan;
                    // OAM interrupt
                    if (get_bit(reg.get(STAT_ADDR), PpuRegisters::OamInt))
                        cpu->request_interrupt(Cpu::LcdStatInt);
                }
                else
                {
                    current_mode = Vblank;
                    renderer->render_frame();
                    cpu->request_interrupt(Cpu::VBlankInterrupt);
                    if (get_bit(reg.get(STAT_ADDR), PpuRegisters::VBlankInterrupt))
                        cpu->request_interrupt(Cpu::LcdStatInt);
                }
            }
            else
                stop = true;
            break;
        }

        case Vblank:
            if (clocksum >= 4560)
            {
                //cerr << "vblank begin @ " << total_cycles - clocksum << endl;

                clocksum -= 4560;
                current_mode = OamScan;
                // OAM interrupt
                if (get_bit(reg.get(STAT_ADDR), PpuRegisters::OamInt))
                    cpu->request_interrupt(Cpu::LcdStatInt);

                //cerr << "vblank end @ " << total_cycles - clocksum << endl;

                total_cycles = clocksum;

                auto start = chrono::high_resolution_clock::now();
                while ((chrono::high_resolution_clock::now() - start)
                           / chrono::milliseconds(1)
                       < 10)
                    ;
            }
            else
                stop = true;
            break;
        }
    }

    return;
}

bool Ppu::has_dma_request()
{
    return reg.read(DMA_ADDR);
}

memaddr_t Ppu::dma_src_address()
{
    return reg.read(DMA_ADDR) * 0x100;
}
