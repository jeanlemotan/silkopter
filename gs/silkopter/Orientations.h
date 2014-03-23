#pragma once


class Orientations : public QWidget
{
public:
	Orientations(QWidget* parent = 0);

	void paintEvent(QPaintEvent*);

	math::vec3f m_euler;
};