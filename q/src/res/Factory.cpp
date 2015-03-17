#include "QStdAfx.h"
#include "q.h"

#include "Impl/Soil_Loader.h"
#include "Impl/PVR_Loader.h"

#include "Impl/Default_Technique_Loader.h"

#include "Impl/QModel_Base_Loader.h"
#include "Impl/QAnimation_Loader.h"

#include "Impl/Free_Type_Font_Loader.h"
#include "Impl/Zup_Sprite_Loader.h"

using namespace q;
using namespace res;

Factory::Factory(data::File_System_ptr const& file_system)
	: m_file_system(file_system)
{
	QASSERT(m_file_system);

	register_loader<video::Texture>(q::make_unique<impl::Soil_Texture_Loader>());
	register_loader<video::Texture>(q::make_unique<impl::Soil_Texture_Loader>());
	register_loader<video::Texture>(q::make_unique<impl::Soil_Texture_Loader>());
	register_loader<video::Texture>(q::make_unique<impl::Soil_Texture_Loader>());
	register_loader<video::Texture>(q::make_unique<impl::Soil_Texture_Loader>());
	register_loader<video::Texture>(q::make_unique<impl::PVR_Texture_Loader>());

	register_loader<video::Dynamic_Image>(q::make_unique<impl::Soil_Dynamic_Image_Loader>());
	register_loader<video::Dynamic_Image>(q::make_unique<impl::PVR_Dynamic_Image_Loader>());

	register_loader<video::Technique>(q::make_unique<impl::DefaultTechniqueLoader>());

	register_loader<scene::Model_Base>(q::make_unique<impl::QModel_Base_Loader>());
	register_loader<anim::Animation>(q::make_unique<impl::QAnimation_Loader>());

	register_loader<text::Font>(q::make_unique<impl::Free_Type_Font_Loader>());

	register_loader<draw::SpriteBase>(q::make_unique<impl::ZupSpriteLoader>());

 	set_streamer<video::Texture>(q::make_unique<Simple_Share_Streamer<video::Texture>>());
 	set_streamer<scene::Model_Base>(q::make_unique<Simple_Share_Streamer<scene::Model_Base>>());
	set_streamer<video::Dynamic_Image>(q::make_unique<Simple_Share_Streamer<video::Dynamic_Image>>());
	set_streamer<anim::Animation>(q::make_unique<Simple_Share_Streamer<anim::Animation>>());
 	set_streamer<video::Technique>(q::make_unique<Simple_Share_Streamer<video::Technique>>());
 	set_streamer<text::Font>(q::make_unique<Simple_Share_Streamer<text::Font>>());
 	set_streamer<draw::SpriteBase>(q::make_unique<Simple_Share_Streamer<draw::SpriteBase>>());
}
Factory::~Factory()
{
	;
}

