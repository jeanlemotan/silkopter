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

#include "qneblock.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>

#include "qneport.h"

QNEBlock::QNEBlock(QGraphicsItem *parent) : QGraphicsPathItem(parent)
{
	QPainterPath p;
	p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
    setPen(QPen(Qt::black));
	setBrush(Qt::green);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	horzMargin = 20;
	vertMargin = 5;
	width = horzMargin;
	height = vertMargin;
    label = new QGraphicsTextItem(this);
}

void QNEBlock::setName(const QString &n)
{
    m_name = n;
    label->setPlainText(n);
    QFont font(scene()->font());
    font.setBold(true);
    label->setFont(font);

    QFontMetrics fm(scene()->font());
    int w = fm.width(n);
    int h = fm.height();
    // port->setPos(0, height + h/2);
    if (w > width - horzMargin)
    {
        width = w + horzMargin;
    }

    height = vertMargin + h * 2;
    QPainterPath p;
    p.addRoundedRect(0, 0, width, height, 5, 5);
    setPath(p);
}

const QString& QNEBlock::name() const
{
    return m_name;
}


QNEPort* QNEBlock::addPort(const QString &name, bool isOutput)
{
	QNEPort *port = new QNEPort(this);
    port->setIsOutput(isOutput);
    port->setName(name);
	port->setNEBlock(this);
    port->setDir(isOutput ? QVector2D(1, 0) : QVector2D(-1, 0));

    refreshGeometry();

	return port;
}

void QNEBlock::refreshGeometry()
{
    QFontMetrics fm(scene()->font());

    width = 0;
    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != QNEPort::Type)
        {
            continue;
        }
        QNEPort *port = (QNEPort*) port_;
        int w = fm.width(port->name());
        // port->setPos(0, height + h/2);
        if (w > width - horzMargin)
        {
            width = w + horzMargin;
        }
    }

    int h = fm.height();
    label->setPos(0, 0);
    int y = vertMargin;
    y += h * 2;

    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() != QNEPort::Type)
        {
            continue;
        }

        QNEPort *port = (QNEPort*) port_;
        if (port->isOutput())
        {
            port->setPos(width + port->radius(), y);
        }
        else
        {
            port->setPos(-port->radius(), y);
        }
        y += h;
    }
    height = y;

    QPainterPath p;
    p.addRoundedRect(0, 0, width, height, 5, 5);
    setPath(p);

}

QNEPort* QNEBlock::addInputPort(const QString &name)
{
    return addPort(name, false);
}

QNEPort* QNEBlock::addOutputPort(const QString &name)
{
    return addPort(name, true);
}

#include <QStyleOptionGraphicsItem>

void QNEBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

    if (isSelected())
    {
        auto p = pen();
        p.setColor(p.color().lighter());
        painter->setPen(p);
        auto b = brush();
        b.setColor(b.color().lighter());
        painter->setBrush(b);
    }
    else
    {
        painter->setPen(pen());
        painter->setBrush(brush());
	}

	painter->drawPath(path());
}

QVector<QNEPort*> QNEBlock::ports()
{
	QVector<QNEPort*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
        {
            res.append((QNEPort*) port_);
        }
    }
	return res;
}

QVariant QNEBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{

    Q_UNUSED(change);

	return value;
}

