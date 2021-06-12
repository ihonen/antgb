#include "debugger/ifrontend.hh"

class SerialFrontend : public iFrontend
{
public:
    virtual void render_callback(const iFrontend::Pixels&)
	{	
	}
	
    virtual void serial_callback(uint8_t byte)
	{
		received_ += static_cast<char>(byte);
	}

    const std::string& get_received() { return received_; }
protected:
    std::string received_;
};
