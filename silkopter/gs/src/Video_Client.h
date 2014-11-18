#pragma once

namespace util
{
class RUDP;
}

class Video_Client
{
public:
    Video_Client(util::RUDP& rudp);
	~Video_Client();

public:
	void process();

    bool get_frame(uint32_t& frame_idx, std::vector<uint8_t>& data) const;

	void set_max_bitrate(uint32_t max_bitrate);
	void set_shutter_speed(std::chrono::microseconds speed);
	void set_quantization(uint32_t quantization);
	void set_intra_period(uint32_t period);
	void set_iso(uint32_t iso);
	void set_mtu(size_t size);

private:
    util::RUDP& m_rudp;

};


