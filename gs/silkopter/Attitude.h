#pragma once


class Attitude : public QWidget
{
public:
	Attitude(QWidget* parent = 0);

	void paintEvent(QPaintEvent*);

	math::vec3f m_euler;
};