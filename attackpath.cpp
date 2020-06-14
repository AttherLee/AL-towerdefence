#include "attackpath.h"
#include <QPainter>
#include <QColor>

attackPath::attackPath(QPoint pos)
	: m_pos(pos)
	, m_nextWayPoint(NULL)
{
}

void attackPath::setNextWayPoint(attackPath *nextPoint)
{
	m_nextWayPoint = nextPoint;
}

attackPath* attackPath::nextWayPoint() const
{
	return m_nextWayPoint;
}

const QPoint attackPath::pos() const
{
	return m_pos;
}

void attackPath::draw(QPainter *painter) const
{
	painter->save();
	painter->setPen(Qt::green);
	painter->drawEllipse(m_pos, 6, 6);
	painter->drawEllipse(m_pos, 2, 2);

	if (m_nextWayPoint)
		painter->drawLine(m_pos, m_nextWayPoint->m_pos);
	painter->restore();
}
