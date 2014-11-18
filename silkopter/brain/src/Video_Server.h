#pragma once

namespace util
{
    class RUDP;
}

namespace silk
{
	//////////////////////////////////////////////////////////////////////////

	class Video_Server : q::util::Noncopyable
	{
	public:
        Video_Server(util::RUDP& rudp);

        enum class Flag : uint8_t
        {
            FLAG_KEYFRAME = 1 << 0,
        };
        typedef q::util::Flag_Set<Flag, uint8_t> Flags;

        //sends a video frame.
        //The data needs to be alive only for the duration of this call.
        auto send_frame(Flags flags, uint8_t const* data, size_t size) -> bool;

	private:
        util::RUDP& m_rudp;
	};

}

