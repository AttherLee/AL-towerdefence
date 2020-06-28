#include "monster.h"
#include "attackpath.h"
#include "tower.h"
#include "utility.h"
#include "mainwindow.h"
//#include "audioplayer.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>

static const int Health_Bar_Width = 20;

const QSize Monster::ms_fixedSize(46, 46);

Monster::Monster(attackPath *startWayPoint, MainWindow *game,int type)
	: QObject(0)

{
    m_active=false;

     m_currentHp=40;

     m_rotationSprite=0.0;
    m_pos=startWayPoint->pos();
    m_destinationWayPoint=startWayPoint->nextWayPoint();
    m_game=game;

    if(type==1)
    {
     m_sprite=QPixmap("../AL_towerdefence/image/怪物一.png");
     m_walkingSpeed=2.0;
      m_maxHp=30;
       m_currentHp=30;
    }
    else if(type==2)
    {
         m_sprite= QPixmap("../AL_towerdefence/image/怪物二.png");
         m_walkingSpeed=1.0;
          m_maxHp=40;
           m_currentHp=40;
    }

}

Monster::~Monster()
{
	m_attackedTowersList.clear();
	m_destinationWayPoint = NULL;
	m_game = NULL;
}

void Monster::doActivate()
{
	m_active = true;
}

void Monster::move()
{
	if (!m_active)
		return;

	if (collisionWithCircle(m_pos, 1, m_destinationWayPoint->pos(), 1))
	{

		if (m_destinationWayPoint->nextWayPoint())
		{

			m_pos = m_destinationWayPoint->pos();
			m_destinationWayPoint = m_destinationWayPoint->nextWayPoint();
		}
		else
		{

			m_game->getHpDamage();
			m_game->removedEnemy(this);
			return;
		}
	}


	QPoint targetPoint = m_destinationWayPoint->pos();

	qreal movementSpeed = m_walkingSpeed;
	QVector2D normalized(targetPoint - m_pos);
	normalized.normalize();
	m_pos = m_pos + normalized.toPoint() * movementSpeed;

}

void Monster::draw(QPainter *painter) const
{
	if (!m_active)
		return;

	painter->save();

	QPoint healthBarPoint = m_pos + QPoint(-Health_Bar_Width / 2 - 5, -ms_fixedSize.height() / 3);
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::red);
	QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
	painter->drawRect(healthBarBackRect);

	painter->setBrush(Qt::green);
	QRect healthBarRect(healthBarPoint, QSize((double)m_currentHp / m_maxHp * Health_Bar_Width, 2));
	painter->drawRect(healthBarRect);

	static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
	painter->translate(m_pos);
	painter->rotate(m_rotationSprite);
	painter->drawPixmap(offsetPoint, m_sprite);

	painter->restore();
}

void Monster::getRemoved()
{
	if (m_attackedTowersList.empty())
		return;

	foreach (Tower *attacker, m_attackedTowersList)
		attacker->targetKilled();
	m_game->removedEnemy(this);
}

void Monster::getDamage(int damage)
{
//	m_game->audioPlayer()->playSound(LaserShootSound);
	m_currentHp -= damage;

	if (m_currentHp <= 0)
	{
    //	m_game->audioPlayer()->playSound(EnemyDestorySound);//音乐接口
        m_game->awardGold(300);
		getRemoved();
	}
}

void Monster::getAttacked(Tower *attacker)
{
	m_attackedTowersList.push_back(attacker);
}

void Monster::gotLostSight(Tower *attacker)
{
	m_attackedTowersList.removeOne(attacker);
}

QPoint Monster::pos() const
{
	return m_pos;
}
