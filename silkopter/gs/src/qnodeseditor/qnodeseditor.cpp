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

#include "qnodeseditor.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include "qneport.h"
#include "qneconnection.h"
#include "qneblock.h"

QNodesEditor::QNodesEditor(QObject *parent) :
    QObject(parent)
{
    m_connection = 0;
}

void QNodesEditor::install(QGraphicsScene *s)
{
	s->installEventFilter(this);
    m_scene = s;
}

QGraphicsItem* QNodesEditor::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = m_scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
    {
        if (item->type() > QGraphicsItem::UserType)
        {
            return item;
        }
    }

	return 0;
}

bool QNodesEditor::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent* me = (QGraphicsSceneMouseEvent*)e;

    const QPen unconnectedPen(QColor(0x3498db), 3);
    const QPen errorPen(QColor(0xe74c3c), 3);
    const QPen connectedPen(QColor(0x2c3e50), 3);

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{

		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
                m_connection = new QNEConnection(0);
                m_scene->addItem(m_connection);
                m_connection->setPort1((QNEPort*) item);
                m_connection->setPos2(me->scenePos());
                m_connection->setPen(unconnectedPen);

				return true;
            }
            else if (item && item->type() == QNEBlock::Type)
			{
				/* if (selBlock)
					selBlock->setSelected(); */
				// selBlock = (QNEBlock*) item;
			}
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && (item->type() == QNEConnection::Type || item->type() == QNEBlock::Type))
            {
                delete item;
                return true;
            }
            break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
        if (m_connection)
		{
            m_connection->setPos2(me->scenePos());

            QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == QNEPort::Type)
            {
                QNEPort *port1 = m_connection->port1();
                QNEPort *port2 = (QNEPort*) item;
                if (port1->block() != port2->block() &&
                    port1->isOutput() != port2->isOutput() &&
                    port1->portType() == port2->portType() &&
                    !port1->isConnected(port2))
                {
                    m_connection->setPen(connectedPen);
                }
                else
                {
                    m_connection->setPen(errorPen);
                }
            }
            else
            {
                m_connection->setPen(unconnectedPen);
            }

			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
        if (m_connection && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
                QNEPort *port1 = m_connection->port1();
				QNEPort *port2 = (QNEPort*) item;

                if (port1->block() != port2->block() &&
                        port1->isOutput() != port2->isOutput() &&
                        port1->portType() == port2->portType() &&
                        !port1->isConnected(port2))
				{
                    port1->connectedSignal.execute(port2);
                    port2->connectedSignal.execute(port1);

                    m_connection->setPort2(port2);
                    m_connection->setPen(connectedPen);
                    m_connection = 0;
					return true;
				}
			}

            delete m_connection;
            m_connection = 0;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}

