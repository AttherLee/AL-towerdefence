#ifndef BULLET_H
#define BULLET_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <QPainter>
#include "monster.h"

class MainWindow;

class Weapon : QObject
{
	Q_OBJECT
	Q_PROPERTY(QPoint m_currentPos READ currentPos WRITE setCurrentPos)

public:
    Weapon(QPoint startPos, QPoint targetPoint, int damage, Monster *target,
           MainWindow *game, const QPixmap &sprite = QPixmap("../lwTowerDemo/image/Shells0.png"));

	void draw(QPainter *painter) const;
	void move();
	void setCurrentPos(QPoint pos);
	QPoint currentPos() const;

private slots:
	void hitTarget();

private:
	const QPoint	m_startPos;
	const QPoint	m_targetPos;
	const QPixmap	m_sprite;
	QPoint			m_currentPos;
    Monster *			m_target;
	MainWindow *	m_game;
	int				m_damage;

	static const QSize ms_fixedSize;
};

#endif // BULLET_H
