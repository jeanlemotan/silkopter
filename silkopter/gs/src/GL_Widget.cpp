#include "GL_Widget.h"
#include <QtOpenGL/QGLFunctions>

//////////////////////////////////////////////////////////////////////////

GL_Widget::GL_Widget(QWidget* parent) 
    : QGLWidget(parent)
{
	//setAttribute(Qt::WA_PaintOnScreen);
}

//////////////////////////////////////////////////////////////////////////

GL_Widget::~GL_Widget()
{
	q::System::destroy();
}

//////////////////////////////////////////////////////////////////////////

void GL_Widget::initializeGL()
{
	auto renderer = new q::video::GLES_Renderer();

	q::System::create();
	q::System::inst().init(q::video::Renderer_ptr(renderer));

	renderer->init(math::vec2u32(width() + 1, height()) + 1, 
		q::video::Render_Target::Color_Format::RGBA_8,
		q::video::Render_Target::Depth_Format::FULL,
		q::video::Render_Target::Stencil_Format::FULL);

	renderer->get_render_target()->set_color_clear_value(math::vec4f(0.6f, 0.6f, 0.6f, 1.f));

	q::System::inst().get_file_system().mount("fonts", q::data::Data_Pack_uptr(new q::data::Folder_Data_Pack(q::Path("data/fonts"))));
	q::System::inst().get_file_system().mount("techniques", q::data::Data_Pack_uptr(new q::data::Folder_Data_Pack(q::Path("data/techniques"))));
	q::System::inst().get_file_system().mount("models", q::data::Data_Pack_uptr(new q::data::Folder_Data_Pack(q::Path("data/models"))));
}

//////////////////////////////////////////////////////////////////////////

void GL_Widget::paintGL()
{
}

