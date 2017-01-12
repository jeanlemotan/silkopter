#include "Menu_System.h"

namespace silk
{

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu_System::push_page(std::unique_ptr<IMenu_Page> page)
{
    QASSERT(page);
    if (!page)
    {
        return;
    }

    QLOGI("Pushing menu page");
    m_pages.push_back(std::move(page));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu_System::process(Input& input)
{
    if (!m_pages.empty())
    {
        IMenu_Page* page = m_pages.back().get();
        if (!page->process(input, *this))
        {
            QLOGI("Removing menu page");
            auto it = std::remove_if(m_pages.begin(), m_pages.end(), [page](std::unique_ptr<IMenu_Page> const& p)
            {
                return p.get() == page;
            });
            m_pages.erase(it, m_pages.end());
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Menu_System::render(Adafruit_GFX& display)
{
    if (!m_pages.empty())
    {
        m_pages.back()->render(display);
    }
}

}
