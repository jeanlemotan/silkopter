#include "stdafx.h"
#include "Attitude.h"

Attitude::Attitude(QWidget* parent /* = 0 */)
	: QWidget(parent)
{

}

void Attitude::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);

	const float k_radius = 40.f;

	auto coss = math::cos(m_euler);
	auto sinn = math::sin(m_euler);

	float x_center = width() / 2.f;
	float y_center = k_radius + k_radius*0.2f;

	//////////////////////////////////////////////////////////////////////////

	float x1 = x_center + k_radius * coss.x;
	float y1 = y_center + k_radius * sinn.x;
	float x2 = x_center - k_radius * coss.x;
	float y2 = y_center - k_radius * sinn.x;
	painter.setPen(QPen(Qt::red));
	painter.drawLine(QPointF(x1, height() - y1), QPointF(x2, height() - y2));

	y_center += k_radius*2 + k_radius*0.2f;
	x1 = x_center + k_radius * coss.y;
	y1 = y_center + k_radius * sinn.y;
	x2 = x_center - k_radius * coss.y;
	y2 = y_center - k_radius * sinn.y;
	painter.setPen(QPen(Qt::green));
	painter.drawLine(QPointF(x1, height() - y1), QPointF(x2, height() - y2));
}



