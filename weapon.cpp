#include "weapon.h"
#include "monster.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPropertyAnimation>

const QSize Weapon::ms_fixedSize(8, 8);

Weapon::Weapon(QPoint startPos, QPoint targetPoint, int damage, Monster *target,
               MainWindow *game,int type)
    : m_startPos(startPos)
    , m_targetPos(targetPoint)
    , m_currentPos(startPos)
    , m_target(target)
    , m_game(game)
    , m_damage(damage)
{
    m_type=type;
    if(type==1)
    {
        m_sprite=QPixmap("../AL_towerdefence/image/shells3.jpg");
    }else if(type==2)
    {
           m_sprite=QPixmap("../AL_towerdefence/image/Shells0.png");
    }
    else
    {
        m_sprite=QPixmap("../AL_towerdefence/image/Shells2.png");
    }
}

void Weapon::draw(QPainter *painter) const
{
    if(m_type!=3)
    {
    painter->drawPixmap(m_currentPos, m_sprite);
    }
    else
    {
        QPoint temppos(m_currentPos.x()-55,m_currentPos.y()-55);
        painter->drawPixmap(temppos, m_sprite);
    }

}

void Weapon::move()
{
    // 100毫秒内击中敌人
    static const int duration = 100;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentPos");
    animation->setDuration(duration);
    animation->setStartValue(m_startPos);
    animation->setEndValue(m_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));

    animation->start();
}
void Weapon::round()
{
    static const int dur_round = 100;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "m_currentPos");
    animation->setDuration(dur_round);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));
    animation->start();
}
void Weapon::hitTarget()
{
    // 这样处理的原因是:
    // 可能多个炮弹击中敌人,而其中一个将其消灭,导致敌人delete
    // 后续炮弹再攻击到的敌人就是无效内存区域
    // 因此先判断下敌人是否还有效
    if (m_game->enemyList().indexOf(m_target) != -1)
        m_target->getDamage(m_damage);
    m_game->removedBullet(this);
}

void Weapon::setCurrentPos(QPoint pos)
{
    m_currentPos = pos;
}

QPoint Weapon::currentPos() const
{
    return m_currentPos;
}
