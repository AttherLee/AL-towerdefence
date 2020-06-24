#include "tower.h"
#include "monster.h"
#include "weapon.h"
#include "mainwindow.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>

const QSize Tower::ms_fixedSize(42, 42);

Tower::Tower(QPoint pos, MainWindow *game,int type,int degree)
    : m_pos(pos)
{
     m_attacking=false;
     m_attackRange=140;
     m_damage=10;
     m_fireRate=1000;
     m_rotationSprite=0.0;
     m_chooseEnemy=NULL;
     m_game=game;
    m_type =type;
    m_degree =degree;
    if(m_type==1)
    {
        if(m_degree==1)
        {
        m_sprite=QPixmap("../AL_towerdefence/image/激光1.png");
         m_damage=5;
        }
        else
        {
              m_sprite=QPixmap("../AL_towerdefence/image/激光2.png");
              m_damage=11;
               m_attackRange=150;
        }

    }
    else if(m_type==2)
    {
        if(m_degree==1)
        {
        m_sprite=QPixmap("../AL_towerdefence/image/箭塔1.png");
        m_damage=9;
         m_attackRange=140;
        }
        else
        {
              m_sprite=QPixmap("../AL_towerdefence/image/箭塔2.png");
              m_damage=18;
               m_attackRange=160;
        }
    }
    else if(m_type==3)
    {
        if(m_degree==1)
        {
        m_sprite=QPixmap("../AL_towerdefence/image/环状炮塔1.png");
        m_damage=6;
         m_attackRange=150;

        }
        else
        {
              m_sprite=QPixmap("../AL_towerdefence/image/环状炮塔2.png");
         m_attackRange=200;
        }
    }
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}

Tower::~Tower()
{
    delete m_fireRateTimer;
    m_fireRateTimer = NULL;
}

void Tower::checkEnemyInRange()
{
    if (m_chooseEnemy)
    {
        // 这种情况下,需要旋转炮台对准敌人
        // 向量标准化
        QVector2D normalized(m_chooseEnemy->pos() - m_pos);
        normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;
        if(m_type==3)
        {
            m_rotationSprite=0;
        }

        // 如果敌人脱离攻击范围
        if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        // 遍历敌人,看是否有敌人在攻击范围内
        QList<Monster *> enemyList = m_game->enemyList();
        foreach (Monster *enemy, enemyList)
        {
            if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}

void Tower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    // 绘制攻击范围
//	painter->drawEllipse(m_pos, m_attackRange, m_attackRange);

    // 绘制偏转坐标,由中心+偏移=左上
    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    // 绘制炮塔并选择炮塔
    painter->translate(m_pos);
    painter->rotate(m_rotationSprite);
    m_sprite.scaled(30,30);
    painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void Tower::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);
}

void Tower::chooseEnemyForAttack(Monster *enemy)
{
    m_chooseEnemy = enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);
}

void Tower::shootWeapon()
{
    Weapon *wp = new Weapon(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game,m_type);
    if(m_type!=3)
    {
    wp->move();
    }
    else
    {
        wp->round();
    }
    m_game->addBullet(wp);
}

void Tower::targetKilled()
{
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();
    m_rotationSprite = 0.0;
}

void Tower::lostSightOfEnemy()
{
    m_chooseEnemy->gotLostSight(this);
    if (m_chooseEnemy)
        m_chooseEnemy = NULL;

    m_fireRateTimer->stop();
    m_rotationSprite = 0.0;
}
