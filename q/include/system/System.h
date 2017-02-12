#pragma once

#include "Ptr_Fw_Declaration.h"
#include "res/Factory.h"
#include "util/Singleton.h"

namespace q
{
class System : public util::Singleton<System>
{
    friend class util::Singleton<System>;
    friend class std::unique_ptr<System>;
protected:
    System();
public:
    ~System();

public:
    void		init(video::Renderer_ptr renderer);

    video::Renderer_ptr const& get_renderer() const;

    data::File_System&	get_file_system();
    res::Factory&		get_factory();

private:
    video::Renderer_ptr m_renderer;
    data::File_System_ptr m_file_system;
    res::Factory m_factory;
};

inline video::Renderer_ptr const& System::get_renderer() const
{
    return m_renderer;
}
inline data::File_System& System::get_file_system()
{
    return *m_file_system;
}
inline res::Factory& System::get_factory()
{
    return m_factory;
}

}
