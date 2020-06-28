#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPoint>
#include <QPainter>
class QPainter;


class attackPath
{
public:
    attackPath(QPoint pos);

    void setNextWayPoint(attackPath *nextPoint);
    attackPath* nextWayPoint() const;
	const QPoint pos() const;

	void draw(QPainter *painter) const;

private:
	const QPoint		m_pos;
    attackPath *			m_nextWayPoint;
};

#endif // WAYPOINT_H
