#ifndef TOWER_H
#define TOWER_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>

class QPainter;
class Monster;
class MainWindow;
class QTimer;

class Tower : QObject
{
	Q_OBJECT
public:
    Tower(QPoint pos, MainWindow *game,int type,int degree);
	~Tower();

	void draw(QPainter *painter) const;
	void checkEnemyInRange();
	void targetKilled();
	void attackEnemy();
    void chooseEnemyForAttack(Monster *enemy);
	void removeBullet();
	void damageEnemy();
	void lostSightOfEnemy();
    const QPoint	m_pos;
    int m_type;
    int m_degree;
private slots:
	void shootWeapon();

private:
	bool			m_attacking;
    int				m_attackRange;
    int				m_damage;
    int				m_fireRate;
	qreal			m_rotationSprite;

    Monster *			m_chooseEnemy;
	MainWindow *	m_game;
	QTimer *		m_fireRateTimer;


    QPixmap	m_sprite;

	static const QSize ms_fixedSize;
};

#endif // TOWER_H
