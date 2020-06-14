#include "selectionbox.h"

//构造
SelectionBox::SelectionBox(QString Path, int width, int height) :
    mwidth(width), mheight(height), SelecBoxImgPath(Path) {}

int SelectionBox::GetX() const     //获取横坐标
{
    return mx;
}
int SelectionBox::GetY() const     //获取横坐标
{
    return my;
}
int SelectionBox::GetWidth() const //获取宽
{
    return mwidth;
}
int SelectionBox::GetHeight() const //获取高
{
    return mheight;
}

QString SelectionBox::GetImgPath() const    //返回选择框图片路径
{
    return SelecBoxImgPath;
}

//获取显示状态
bool SelectionBox::GetDisplay() const
{
    return display;
}

//设置显示状态
void SelectionBox::SetDisplay(const bool SetPlay)
{
    display = SetPlay;
}

//选中防御塔
void SelectionBox::CheckTower(int x, int y)
{
    //确定选择框的位置
    mx = x - 65, my = y - 65;

    //确定子按钮的位置
    SubBut[0].SubX = mx + 60, SubBut[0].SubY = my + 8;
    SubBut[0].SubImgPath = QString("../lwTowerDemo/image/tower.png");

    SubBut[1].SubX = mx + 8, SubBut[1].SubY = my + 60;
    SubBut[1].SubImgPath = QString("../lwTowerDemo/image/tower2.png");

    SubBut[2].SubX = mx + 160, SubBut[2].SubY = my + 60;
    SubBut[2].SubImgPath = QString("../lwTowerDemo/image/tower3.png");

    SubBut[3].SubX = mx + 60, SubBut[3].SubY = my + 160;
    SubBut[3].SubImgPath = QString("../lwTowerDemo/image/tower4.png");

    display = true; //显示状态设为真
}

//获取子按钮结构数组
SubbutStr* SelectionBox::GetSelSubBut()
{
    return SubBut;
}
