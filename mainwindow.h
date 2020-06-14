#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
//#include "selectionbox.h"
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

    //AudioPlayer* audioPlayer() const;
    QList<Monster *> enemyList() const;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
public slots:
    void updateMap();
    void gameStart();
    void getInform(int);
private:
    QMap<int,QVector<QPair<QString,int>>> paraconfig;
    const int initGold =300;
    QPushButton *startPushBUtton;
    QComboBox *combox;

  //  SelectionBox* SelBox;
   QLabel *round;
   QLabel *goldcount;
   QLabel*gameHp;
   void initPosition();
   void addWayPoints();
   bool loadWave();
   bool canBuyTower() const;
    void DrawSelectionBox(QPainter&);
//	void drawWave(QPainter *painter);
//	void drawHP(QPainter *painter);
//	void drawPlayerGold(QPainter *painter);
   void doGameOver();
   void initConfig();

   QPoint m_pos;
    int						m_waves;
    int						m_playerHp;
    int						m_playrGold;
    bool					m_gameEnded;
    bool					m_gameWin;
//	AudioPlayer *			m_audioPlayer;
    QList<QVariant>			m_wavesInfo;
    QList<TowerPosition>	m_towerPositionsList;
    QList<Tower *>			m_towersList;
    QList<attackPath *>		m_wayPointsList;
    QList<Monster *>			m_enemyList;
    QList<Weapon *>			m_bulletList;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
