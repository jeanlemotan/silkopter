/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qneconnection.h"

#include "qneport.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

QNEConnection::QNEConnection(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
    setPen(QPen(QColor(QRgb(0x2c3e50)), 3));
	setBrush(Qt::NoBrush);
	setZValue(-1);
}

QNEConnection::~QNEConnection()
{
	if (m_port1)
    {
        m_port1->connections().remove(m_port1->connections().indexOf(this));
    }
    if (m_port2)
    {
        m_port2->connections().remove(m_port2->connections().indexOf(this));
    }
}

void QNEConnection::setPos1(const QPointF &p)
{
    m_pos1 = p;
}

void QNEConnection::setPos2(const QPointF &p)
{
    m_pos2 = p;
}

void QNEConnection::setPort1(QNEPort *p)
{
	m_port1 = p;
	m_port1->connections().append(this);
    if (m_port2)
    {
        m_port1->connectedSignal.execute(m_port2);
        m_port2->connectedSignal.execute(m_port1);
    }
}

void QNEConnection::setPort2(QNEPort *p)
{
	m_port2 = p;
	m_port2->connections().append(this);
    if (m_port1)
    {
        m_port1->connectedSignal.execute(m_port2);
        m_port2->connectedSignal.execute(m_port1);
    }
}

void QNEConnection::updatePosFromPorts()
{
    m_pos1 = m_port1->scenePos();
    m_pos2 = m_port2->scenePos();
}

void QNEConnection::updatePath()
{
	QPainterPath p;

	//QPointF pos1(m_port1->scenePos());
	//QPointF pos2(m_port2->scenePos());

    p.moveTo(m_pos1);

    QPointF ctrl1, ctrl2;

    ctrl1.setX(m_pos1.x() + (m_port1 ? m_port1->dir().x() : -m_port2->dir().x()) * 50.f);
    ctrl1.setY(m_pos1.y());

    ctrl2.setX(m_pos2.x() + (m_port2 ? m_port2->dir().x() : -m_port1->dir().x()) * 50.f);
    ctrl2.setY(m_pos2.y());

    p.cubicTo(ctrl1, ctrl2, m_pos2);

	setPath(p);
}

QNEPort* QNEConnection::port1() const
{
	return m_port1;
}

QNEPort* QNEConnection::port2() const
{
	return m_port2;
}
