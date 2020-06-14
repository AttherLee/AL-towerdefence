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
    //根据图片大小设置窗口大小
    this->setWindowTitle("塔防小游戏");
      this->resize(900,490);
     m_waves=0;
    m_playerHp=7;
    m_playrGold=1500;
    m_gameEnded=false;
    m_gameWin =false;
    QHBoxLayout *blayout =new QHBoxLayout();
    QVBoxLayout *vlayout = new QVBoxLayout();
    QLabel *labelround =new QLabel("关卡:     1");
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
   // initWaves();
      initConfig();//初始化怪物信息

    addWayPoints();
    //初始化炮塔位置
     initPosition();
    combox =new QComboBox(this);
    combox->addItem(QIcon("../AL_towerdefence/image/激光1.png"),"一级激光塔");
    combox->addItem(QIcon("../AL_towerdefence/image/激光2.png"),"二级激光塔");
    combox->addItem(QIcon("../AL_towerdefence/image/箭塔1.png"),"一级箭塔");
    combox->addItem(QIcon("../AL_towerdefence/image/箭塔2.png"),"二级箭塔");
    combox->setCurrentIndex(-1);
    combox->hide();
    connect(combox,SIGNAL(currentIndexChanged(int)),this,SLOT(getInform(int)));
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
  //  QPoint p22(575,425);

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
        QString text = m_gameEnded ? "YOU LOST!!!" : "YOU WIN!!!";
        QPainter painter(this);
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(), Qt::AlignCenter, text);
        return;
    }

    QPixmap cachePix("../AL_towerdefence/image/Background.png");
    QPainter cachePainter(&cachePix);

    foreach (const TowerPosition &towerPos, m_towerPositionsList)
        towerPos.draw(&cachePainter);

    foreach (const Tower *tower, m_towersList)
        tower->draw(&cachePainter);

//    foreach (const attackPath *wayPoint, m_wayPointsList)
//        wayPoint->draw(&cachePainter);

    foreach (const Monster *enemy, m_enemyList)
        enemy->draw(&cachePainter);

    foreach (const Weapon *bullet, m_bulletList)
        bullet->draw(&cachePainter);

//	drawWave(&cachePainter);
//	drawHP(&cachePainter);
//	drawPlayerGold(&cachePainter);
    //金币更新
    m_waves=+1;
    goldcount->setText(QString::number(m_playrGold));
    gameHp->setText(QString::number(m_playerHp));
    QPainter painter(this);
    painter.drawPixmap(0, 0, cachePix);
//    QPixmap result;
//    if(m_playerHp==5)
//    {
//       result =QPixmap("../AL_towerdefence/image/45kg.png");
//    }
//    else if(m_playerHp==4)
//    {
//        result =QPixmap("../AL_towerdefence/image/50kg.png");
//    }
//    else if(m_playerHp==3)
//    {
//         result =QPixmap("../AL_towerdefence/image/55kg.png");
//    }
//    else
//    {
//        result =QPixmap("../AL_towerdefence/image/60kg.png");
//    }
//    painter.drawPixmap(730,420,result);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<event->pos();
    combox->hide();
    QPoint pressPos = event->pos();
            //选中防御塔，选择框显示

    auto it = m_towerPositionsList.begin();
    while (it != m_towerPositionsList.end())
    {
        if (canBuyTower() && it->containPoint(pressPos) && !it->hasTower())
        {

    //		m_audioPlayer->playSound(TowerPlaceSound);
            combox->move(event->pos());
            combox->showPopup();
          //  combox->show();


            m_pos=it->centerPos();
             it->setHasTower();
//            Tower *tower = new Tower(it->centerPos(), this);
//            m_towersList.push_back(tower);
//            update();
            break;
        }

        ++it;
    }

}

bool MainWindow::canBuyTower() const
{
    if (m_playrGold >= initGold)
        return true;
    return false;
}


//void MainWindow::drawWave(QPainter *painter)
//{
//	painter->setPen(QPen(Qt::red));
//	painter->drawText(QRect(400, 5, 100, 25), QString("WAVE : %1").arg(m_waves + 1));
//}

//void MainWindow::drawHP(QPainter *painter)
//{
//	painter->setPen(QPen(Qt::red));
//	painter->drawText(QRect(30, 5, 100, 25), QString("HP : %1").arg(m_playerHp));
//}

//void MainWindow::drawPlayerGold(QPainter *painter)
//{
//	painter->setPen(QPen(Qt::red));
//	painter->drawText(QRect(200, 5, 200, 25), QString("GOLD : %1").arg(m_playrGold));
//}

void MainWindow::doGameOver()
{
    if (!m_gameEnded)
    {
        m_gameEnded = true;
        // 此处应该切换场景到结束场景
        // 暂时以打印替代,见paintEvent处理
    }
}

void MainWindow::initConfig()
{
      QPair<QString,int> temp;
    QVector<QPair<QString,int>> round1;
     QVector<QPair<QString,int>> round2;
       QVector<QPair<QString,int>> round3;
    for (int i=0;i<5;i++) {
        temp.first="怪物1";
        temp.second=(qrand()%30)*100;
        round1.append(temp);
        temp.first="怪物2";
        temp.second=(10+qrand()%40)*100;
        round2.append(temp);
        temp.first="怪物3";
         temp.second=(3+qrand()%3)*1000;
         round3.append(temp);
    }
    for (int i=0;i<3;i++) {
        temp.first="怪物2";
        temp.second=(3+qrand()%3)*1000;
        round1.append(temp);
        round3.append(temp);
        temp.first="怪物3";
        temp.second=(2+qrand()%3)*1000;
        round2.append(temp);
    }
    paraconfig.insert(1,round1);
     paraconfig.insert(2,round2);
      paraconfig.insert(3,round3);
}

void MainWindow::awardGold(int gold)
{
    m_playrGold += gold;
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

void MainWindow::getHpDamage(int damage/* = 1*/)
{
//	m_audioPlayer->playSound(LifeLoseSound);
    m_playerHp -= damage;
    if (m_playerHp <= 0)
        doGameOver();
}

void MainWindow::removedEnemy(Monster *enemy)
{
    Q_ASSERT(enemy);

    m_enemyList.removeOne(enemy);
    delete enemy;

    if (m_enemyList.empty())
    {
        ++m_waves;
        if (!loadWave())
        {
            m_gameWin = true;
            // 游戏胜利转到游戏胜利场景
            // 这里暂时以打印处理
        }
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
//初始化波数
//void MainWindow::initWaves()
//{
//    QFile file("../lwTowerDemo/config/Waves.plist");
//    if (!file.open(QFile::ReadOnly | QFile::Text))
//    {
//        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
//        return;
//    }

//    PListReader reader;
//    reader.read(&file);
//    m_wavesInfo = reader.data();
//    file.close();
//}

bool MainWindow::loadWave()
{
   if(m_waves>paraconfig.size())
   {
       return false;
   }
     attackPath *startWayPoint = m_wayPointsList.back();
     for (int i=0;i<paraconfig.size();i++) {
         for (int j=0;j<paraconfig[i].size();j++) {
               QPixmap temp;
             if(paraconfig[i][j].first=="怪物1")
             {
                  temp=QPixmap("../AL_towerdefence/image/怪物一.png");
             }
              else if(paraconfig[i][j].first=="怪物2")
              {
                   temp=QPixmap("../AL_towerdefence/image/怪物二.png");
              }
//              else if(paraconfig[i][j].first=="怪物3")
//              {
//                    temp=QPixmap("../AL_towerdefence/image/怪兽3.png");
//              }
             int stime=paraconfig[i][j].second;
             Monster *enemy = new Monster(startWayPoint, this,temp);
             m_enemyList.push_back(enemy);
              QTimer::singleShot(stime, enemy, SLOT(doActivate()));
         }
     }
     return true;
//    if (m_waves >= m_wavesInfo.size())
//        return false;

//    attackPath *startWayPoint = m_wayPointsList.back();
//  //  attackPath *startWayPoint = m_wayPointsList.at(0);
//    QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();

//    for (int i = 0; i < curWavesInfo.size(); ++i)
//    {
//        QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
//        int spawnTime = dict.value("spawnTime").toInt();
//        QString mositername=dict.value("data").toString();
//        QPixmap temp;
//        if(mositername=="怪物1")
//        {
//              temp=QPixmap("../lwTowerDemo/image/怪兽1.png");
//        }
//        else if(mositername=="怪物2")
//        {
//              temp=QPixmap("../lwTowerDemo/image/怪兽2.png");
//        }
//        else if(mositername=="怪物3")
//        {
//              temp=QPixmap("../lwTowerDemo/image/怪兽3.png");
//        }

//        Monster *enemy = new Monster(startWayPoint, this,temp);
//        m_enemyList.push_back(enemy);
//        QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
//    }

//    return true;
}

QList<Monster *> MainWindow::enemyList() const
{
    return m_enemyList;
}

void MainWindow::gameStart()
{
    startPushBUtton->setDisabled(true);
    loadWave();
}

void MainWindow::getInform(int temp)
{

    if(temp==0)
    {
        Tower *tower = new Tower(m_pos, this,QPixmap("../AL_towerdefence/image/激光1.png"));
        m_towersList.push_back(tower);
           m_playrGold =m_playrGold-100;
        update();
    }
    else if(temp==1)
        {
        Tower *tower = new Tower(m_pos, this,QPixmap("../AL_towerdefence/image/激光2.png"));
        m_towersList.push_back(tower);
          m_playrGold =m_playrGold-200;
        update();
      }
    else if(temp==2)
    {
        Tower *tower = new Tower(m_pos, this,QPixmap("../AL_towerdefence/image/箭塔1.png"));
        m_towersList.push_back(tower);
          m_playrGold =m_playrGold-300;
        update();
    }
    else if(temp==3)
    {
        Tower *tower = new Tower(m_pos, this,QPixmap("../AL_towerdefence/image/箭塔2.png"));
        m_towersList.push_back(tower);
          m_playrGold =m_playrGold-400;
        update();
    }
    combox->hide();
    combox->setCurrentIndex(-1);
}
