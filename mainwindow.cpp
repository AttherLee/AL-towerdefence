#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "attackpath.h"
#include "monster.h"
#include "weapon.h"
#include "plistreader.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>
#define MouClickRegion(X, Width, Y, Height)     \
(event->x() >= (X) && event->x() <= (X) + (Width) &&  \
event->y() >= (Y) && event->y() <= (Y) + (Height))

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("AL_towerdefence");
    setWindowIcon(QIcon("../AL_towerdefence/image/logo2.ico"));
      this->resize(900,490);
     m_waves=0;
    m_playerHp=3;
    m_playrGold=3000;
    m_gameEnded=false;
    m_gameWin =false;
    m_round =1;
    m_flag=true;
    QHBoxLayout *blayout =new QHBoxLayout();
    QVBoxLayout *vlayout = new QVBoxLayout();
     labelround =new QLabel(tr("关卡:     ")+QString::number(m_round));
    round=new QLabel();
      QLabel *labelgold =new QLabel("金币:");
      goldcount=new QLabel();
       QLabel *labelhp =new QLabel("血量:");
       gameHp = new QLabel();
       startPushBUtton=new QPushButton(tr("开始"));
     vlayout->addSpacing(100);
    vlayout->addWidget(labelround,Qt::AlignCenter);
 //   vlayout->addWidget(round,Qt::AlignRight);
    vlayout->addSpacing(10);
    QHBoxLayout *goldlayhout= new QHBoxLayout();
    goldlayhout->addWidget(labelgold);
    goldlayhout->addWidget(goldcount);
    vlayout->addLayout(goldlayhout,Qt::AlignCenter);
    QHBoxLayout *hplayhout= new QHBoxLayout();
    hplayhout->addWidget(labelhp);
     hplayhout->addWidget(gameHp);
     vlayout->addSpacing(10);
     vlayout->addLayout(hplayhout,Qt::AlignCenter);
      vlayout->addSpacing(100);
       vlayout->addWidget(startPushBUtton,Qt::AlignCenter);
       vlayout->addSpacing(50);
      blayout->addWidget(ui->widget);
      blayout->addLayout(vlayout);
      ui->widget->setFixedSize(780,490);
        this->centralWidget()->setLayout(blayout);

    initPosition();
    m_musicPlayer =new QMediaPlayer();
    m_musicPlayer->setVolume(50);
    m_musicPlayer2 =new QMediaPlayer();
    m_musicPlayer2->setVolume(120);
    combox =new QComboBox(this);
    combox->addItem(QIcon("../AL_towerdefence/image/激光1.png"),"一级激光塔");
    combox->addItem(QIcon("../AL_towerdefence/image/箭塔1.png"),"一级箭塔");
    combox->addItem(QIcon("../AL_towerdefence/image/环状炮塔1.png"),"一级环状");
    combox->setCurrentIndex(-1);
    combox->hide();

    combox_c=new QComboBox(this);
    combox_c->addItem(QIcon("../AL_towerdefence/image/升级标识.png"),"升级");
    combox_c->addItem(QIcon("../AL_towerdefence/image/拆除标识.png"),"卖出");
    combox_c->setCurrentIndex(-1);
    combox_c->hide();
    connect(combox,SIGNAL(currentIndexChanged(int)),this,SLOT(getInform(int)));
    connect(combox_c,SIGNAL(currentIndexChanged(int)),this,SLOT(changeState(int)));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(10);
    connect(startPushBUtton,&QPushButton::clicked,this,&MainWindow::gameStart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initPosition()
{
   //第一排
    QPoint p1(85,10);
     QPoint p2(85,160);
    QPoint p5(85,315);
    QPoint p7(85,390);
    //第二
    QPoint p9(238,86);
    QPoint p11(238,235);
    QPoint p12(238,390);
     //第三
    QPoint p13(392,10);
    QPoint p14(392,163);
    QPoint p15(392,318);
      //第四排
    QPoint p16(546,86);
    QPoint p17(546,238);
    QPoint p18(546,391);

    //第五排
    QPoint p19(621,163);
    QPoint p20(621,314);
    QPoint p21(621,469);

    m_towerPositionsList.push_back(p1);
   m_towerPositionsList.push_back(p2);
   m_towerPositionsList.push_back(p5);
    m_towerPositionsList.push_back(p7);
   m_towerPositionsList.push_back(p9);
   m_towerPositionsList.push_back(p11);
   m_towerPositionsList.push_back(p12);
    m_towerPositionsList.push_back(p13);
    m_towerPositionsList.push_back(p14);
    m_towerPositionsList.push_back(p15);
     m_towerPositionsList.push_back(p16);
     m_towerPositionsList.push_back(p17);
     m_towerPositionsList.push_back(p18);
     m_towerPositionsList.push_back(p19);
     m_towerPositionsList.push_back(p20);
     m_towerPositionsList.push_back(p21);

}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (m_gameEnded || m_gameWin)
    {
        QPainter painter(this);
        QPixmap endPic;
        if(m_gameEnded)
        {
             m_flag =false;
            endPic=QPixmap("../AL_towerdefence/image/lost.png");
             painter.drawPixmap(QRect(0,0,760,534),endPic);
        }
        else if(m_gameWin)
        {   m_flag =false;
            m_towerPositionsList.clear();
            m_towersList.clear();
            m_enemyList.clear();
            m_bulletList.clear();
              endPic=QPixmap("../AL_towerdefence/image/success1.png");
              if(m_round==3)
              {
                    endPic=QPixmap("../AL_towerdefence/image/success2.png");
              }
               painter.drawPixmap(QRect(0,0,760,534),endPic);
                  startPushBUtton->setEnabled(true);


        }

    }
    QPixmap cachePix;
    if(m_round==1&&m_flag)
    {
        cachePix=QPixmap("../AL_towerdefence/image/Background.png");
    }
    else if(m_round==2&&m_flag)
    {
         cachePix=QPixmap("../AL_towerdefence/image/Background2.png");
    }

    QPainter cachePainter(&cachePix);
    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const Tower *tower, m_towersList)
        tower->draw(&cachePainter);


    foreach (const Monster *enemy, m_enemyList)
        enemy->draw(&cachePainter);

    foreach (const Weapon *bullet, m_bulletList)
    {
        bullet->draw(&cachePainter);
     }


    m_waves=+1;
    goldcount->setText(QString::number(m_playrGold));
    gameHp->setText(QString::number(m_playerHp));
    labelround->setText(tr("关卡:     ")+QString::number(m_round));
    QPainter painter(this);
      painter.drawPixmap(0, 0, cachePix);

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<event->pos();
    combox->hide();
    combox_c->hide();
    QPoint pressPos = event->pos();

    auto it = m_towerPositionsList.begin();
    while (it != m_towerPositionsList.end())
    {
        if(it->containPoint(pressPos)&&it->hasTower())
        {
            combox_c->move(event->pos());
            combox_c->showPopup();

             m_position=&it.i->t();
             m_pos=it->centerPos();
        }
        if (canBuyTower(400) && it->containPoint(pressPos) && !it->hasTower())
        {

    //		m_audioPlayer->playSound(TowerPlaceSound);//音乐接口
            m_position=&it.i->t();
            combox->move(event->pos());
            combox->showPopup();
            m_pos=it->centerPos();
            break;
        }

        ++it;
    }

}

bool MainWindow::canBuyTower(int n) const
{
    if (m_playrGold >= n)
        return true;
    return false;
}

void MainWindow::doGameOver()
{
    if (!m_gameEnded)
    {
        m_gameEnded = true;
    }
}

void MainWindow::initConfig()
{
      QPair<QString,int> temp;
    QVector<QPair<QString,int>> round1;
     QVector<QPair<QString,int>> round2;
       QVector<QPair<QString,int>> round3;
         QVector<QPair<QString,int>> round4;
    for (int i=0;i<3;i++) {
        temp.first="怪物1";
        temp.second=(qrand()%3)*1000;
        round1.append(temp);
        temp.second=(45+qrand()%3)*1000;
        round4.append(temp);
        temp.first="怪物2";
        temp.second=(20+qrand()%20)*100;
        round2.append(temp);
      //  temp.first="怪物3";
         temp.second=(32+qrand()%2)*1000;
         round3.append(temp);
         temp.second=(30+qrand()%2)*1000;
           round3.append(temp);
    }
    for (int i=0;i<3;i++) {
        temp.first="怪物2";
        temp.second=(2+qrand()%1)*1000;
        round1.append(temp);
 //       round3.append(temp);
//        temp.first="怪物3";
        temp.second=(19+qrand()%3)*1000;
        round2.append(temp);
          temp.second=(29+qrand()%2)*1000;
         round3.append(temp);
          temp.second=(48+qrand()%2)*1000;
           round3.append(temp);
    }
    paraconfig.insert(0,round1);
     paraconfig.insert(1,round2);
        paraconfig.insert(2,round3);
            paraconfig.insert(3,round4);
}

void MainWindow::musiscplay(QString str)
{
    m_musicPlayer->setMedia(QUrl::fromLocalFile(str));
    m_musicPlayer->play();
}

void MainWindow::musiscplay2(QString str)
{
    m_musicPlayer2->setMedia(QUrl::fromLocalFile(str));
    m_musicPlayer2->play();
}

void MainWindow::awardGold(int gold)
{
    m_playrGold += gold;
     musiscplay2("../AL_towerdefence/image/music/addgold.mp3");
    update();
}


void MainWindow::addWayPoints()
{
    attackPath *wayPoint1 = new attackPath(QPoint(727,526));
    m_wayPointsList.push_back(wayPoint1);

    attackPath *wayPoint2 = new attackPath(QPoint(721,39));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    attackPath *wayPoint3 = new attackPath(QPoint(500,45));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    attackPath *wayPoint4 = new attackPath(QPoint(500,500));
    m_wayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    attackPath *wayPoint5 = new attackPath(QPoint(340,500));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    attackPath *wayPoint6 = new attackPath(QPoint(345,45));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);

    attackPath *wayPoint7 = new attackPath(QPoint(190,42));
    m_wayPointsList.push_back(wayPoint7);
    wayPoint7->setNextWayPoint(wayPoint6);

    attackPath *wayPoint8 = new attackPath(QPoint(190,500));
    m_wayPointsList.push_back(wayPoint8);
    wayPoint8->setNextWayPoint(wayPoint7);

    attackPath *wayPoint9 = new attackPath(QPoint(30,500));
    m_wayPointsList.push_back(wayPoint9);
    wayPoint9->setNextWayPoint(wayPoint8);


    attackPath *wayPoint10 = new attackPath(QPoint(35,0));
    m_wayPointsList.push_back(wayPoint10);
    wayPoint10->setNextWayPoint(wayPoint9);

}

void MainWindow::addWayPoints2()
{
    attackPath *wayPoint1 = new attackPath(QPoint(727,500));
    m_wayPointsList.push_back(wayPoint1);

    attackPath *wayPoint2 = new attackPath(QPoint(492,505));
    m_wayPointsList.push_back(wayPoint2);
    wayPoint2->setNextWayPoint(wayPoint1);

    attackPath *wayPoint3 = new attackPath(QPoint(494,274));
    m_wayPointsList.push_back(wayPoint3);
    wayPoint3->setNextWayPoint(wayPoint2);

    attackPath *wayPoint4 = new attackPath(QPoint(26,276));
    m_wayPointsList.push_back(wayPoint4);
    wayPoint4->setNextWayPoint(wayPoint3);

    attackPath *wayPoint5 = new attackPath(QPoint(29,117));
    m_wayPointsList.push_back(wayPoint5);
    wayPoint5->setNextWayPoint(wayPoint4);

    attackPath *wayPoint6 = new attackPath(QPoint(722 ,122));
    m_wayPointsList.push_back(wayPoint6);
    wayPoint6->setNextWayPoint(wayPoint5);

    attackPath *wayPoint7 = new attackPath(QPoint(722,7));
    m_wayPointsList.push_back(wayPoint7);
    wayPoint7->setNextWayPoint(wayPoint6);

}

void MainWindow::getHpDamage(int damage/* = 1*/)
{
//	m_audioPlayer->playSound(LifeLoseSound);//音乐接口
    m_playerHp -= damage;
    if (m_playerHp <= 0)
        {
        m_playerHp=0;
        doGameOver();
          musiscplay("../AL_towerdefence/image/music/gameover.mp3");
        }
}

void MainWindow::removedEnemy(Monster *enemy)
{
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty()&&m_playerHp>0)
    {
        ++m_waves;
            m_gameWin = true;
            m_round++;
             musiscplay("../AL_towerdefence/image/music/success.mp3");
    }
}

void MainWindow::removedBullet(Weapon *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.removeOne(bullet);
    delete bullet;
}

void MainWindow::addBullet(Weapon *bullet)
{
    Q_ASSERT(bullet);

    m_bulletList.push_back(bullet);
}

void MainWindow::updateMap()
{
    foreach (Monster *enemy, m_enemyList)
        enemy->move();
    foreach (Tower *tower, m_towersList)
        tower->checkEnemyInRange();
    update();
}


bool MainWindow::loadWave()
{
     attackPath *startWayPoint = m_wayPointsList.back();
     for (int i=0;i<paraconfig.size();i++) {
         for (int j=0;j<paraconfig[i].size();j++) {
               int  temp;
             if(paraconfig[i][j].first=="怪物1")
             {
                  temp=1;
             }
              else if(paraconfig[i][j].first=="怪物2")
              {
                   temp=2;
              }

             int stime=paraconfig[i][j].second;
             Monster *enemy = new Monster(startWayPoint, this,temp);
             m_enemyList.push_back(enemy);
              QTimer::singleShot(stime, enemy, SLOT(doActivate()));
         }
     }

}

bool MainWindow::loadWave2()
{
    m_flag=true;
    m_waves=0;
   m_playerHp=10;
   m_playrGold=3000;
   m_gameEnded=false;
   m_gameWin =false;

}



QList<Monster *> MainWindow::enemyList() const
{
    return m_enemyList;
}

void MainWindow::gameStart()
{

    startPushBUtton->setDisabled(true);
    if(m_round==1)
    {
         musiscplay("../AL_towerdefence/image/music/background1.mp3");
        initConfig();

      addWayPoints();

        loadWave();
    }
    else if(m_round==2)
    {
          musiscplay("../AL_towerdefence/image/music/background2.mp3");
        loadWave2();
        addWayPoints2();
        loadWave();
        loadPosition();
    }


}

void MainWindow::loadPosition()
{
     QPoint p1(541,11);
      QPoint p2(390,11);
     QPoint p5(235,11);
     QPoint p7(82,11);
     //第二
     QPoint p9(83,164);
     QPoint p11(238,164);
     QPoint p12(388,164);
       QPoint p13(543,164);
      //第三
       QPoint p16(82,316);
       QPoint p17(236,316);
       QPoint p18(390,316);
     QPoint p14(540,311);

       //第四排

       QPoint p15(540,394);
        QPoint p19(390,470);
     m_towerPositionsList.push_back(p1);
    m_towerPositionsList.push_back(p2);
    m_towerPositionsList.push_back(p5);
     m_towerPositionsList.push_back(p7);
    m_towerPositionsList.push_back(p9);
    m_towerPositionsList.push_back(p11);
    m_towerPositionsList.push_back(p12);
     m_towerPositionsList.push_back(p13);
     m_towerPositionsList.push_back(p14);
     m_towerPositionsList.push_back(p15);
      m_towerPositionsList.push_back(p16);
      m_towerPositionsList.push_back(p17);
      m_towerPositionsList.push_back(p18);
      m_towerPositionsList.push_back(p19);
}

void MainWindow::getInform(int temp)
{

    if(temp==0)
    {
        Tower *tower = new Tower(m_pos,this,1,1);
        m_towersList.push_back(tower);
           m_playrGold =m_playrGold-400;
        update();
    }
    else if(temp==1)
        {
        Tower *tower = new Tower(m_pos, this,2,1);
        m_towersList.push_back(tower);
          m_playrGold =m_playrGold-300;
        update();
      }
    else if(temp==2)
    {
        Tower *tower = new Tower(m_pos, this,3,1);
        m_towersList.push_back(tower);
          m_playrGold =m_playrGold-350;
        update();
    }

     musiscplay2("../AL_towerdefence/image/music/reducegold.mp3");
    combox->hide();
    combox->setCurrentIndex(-1);
    m_position->setHasTower();
}
void MainWindow::changeState(int temp)
{
    if(temp==1)
    {
         musiscplay2("../AL_towerdefence/image/music/addgold.mp3");
       m_position->setNoToewer();
       for (int i=0;i<m_towersList.count();i++) {
           if(m_towersList.at(i)->m_pos==m_pos)
           {
               m_towersList.removeAt(i);
                   m_playrGold =m_playrGold+150;
                   break;
           }
       }
    }
    else if(temp==0&&canBuyTower(300))
    {
         musiscplay2("../AL_towerdefence/image/music/reducegold.mp3");
        for (int i=0;i<m_towersList.count();i++) {
            if(m_towersList.at(i)->m_pos==m_pos&&m_towersList.at(i)->m_degree<3)
            {
                int degree=m_towersList.at(i)->m_degree;
                int type=m_towersList.at(i)->m_type;
                Tower *tw=new Tower(m_pos,this,type,++degree);
                m_towersList.replace(i,tw);
                m_playrGold =m_playrGold-300;
                break;
            }
        }
    }
    combox_c->setCurrentIndex(-1);
}
