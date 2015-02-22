#pragma once

//this is responsible for initializing opengl and providing  API to render to FBOs and converting these to QImages

class GL_Widget : public QGLWidget
{
public:
	GL_Widget(QWidget* parent);
	~GL_Widget();

	void initializeGL();
	void paintGL();
};
