#pragma once


class Attitude : public QWidget
{
public:
	Attitude(QWidget* parent = 0);

	void set_euler(math::vec3f const& euler);

private:
	void paintEvent(QPaintEvent*);

	math::vec3f m_euler;
};