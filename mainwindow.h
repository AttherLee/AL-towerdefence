#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QComboBox>
#include "towerposition.h"
#include "tower.h"
#include "weapon.h"
#include "attackpath.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    void getHpDamage(int damage = 1);
    void removedEnemy(Monster *enemy);
    void removedBullet(Weapon *bullet);
    void addBullet(Weapon *bullet);
    void awardGold(int gold);
    QList<Monster *> enemyList() const;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
public slots:
    void updateMap();
    void gameStart();
    void getInform(int);
    void changeState(int);
private:
    QMap<int,QVector<QPair<QString,int>>> paraconfig;
    QPushButton *startPushBUtton;
    QComboBox *combox;
    QComboBox*combox_c;
    TowerPosition* m_position;
   QLabel *round;
   QLabel *goldcount;
   QLabel*gameHp;
   void initPosition();
   void addWayPoints();
   void addWayPoints2();
   bool loadWave();
   bool loadWave2();
   void loadPosition();
   bool m_flag;
   bool canBuyTower(int) const;
    void DrawSelectionBox(QPainter&);
   void doGameOver();
   void initConfig();
   void musiscplay(QString str);
    void musiscplay2(QString str);
   QPoint m_pos;
   int                      m_round;
    int						m_waves;
    int						m_playerHp;
    int						m_playrGold;
    bool					m_gameEnded;
    bool					m_gameWin;
    QLabel                 *labelround;
    QMediaPlayer *			m_musicPlayer;
     QMediaPlayer *			m_musicPlayer2;
    QList<QVariant>			m_wavesInfo;
    QList<TowerPosition>	m_towerPositionsList;
    QList<Tower *>			m_towersList;
    QList<attackPath *>		m_wayPointsList;
    QList<Monster *>			m_enemyList;
    QList<Weapon *>			m_bulletList;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
