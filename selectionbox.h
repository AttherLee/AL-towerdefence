#ifndef SELECTIONBOX_H
#define SELECTIONBOX_H

#include <QString>
struct SubbutStr
{
    int SubX;           //子按钮相对选择框的横坐标
    int SubY;           //纵坐标
    int SubWidth = 56;  //子按钮宽
    int SubHeight = 56; //高
    QString SubImgPath; //子按钮的图片路径
};

//选择框对象
class SelectionBox
{
private:
    bool display = false;   //是否显示
    int mx = 0, my = 0;     //最左上角坐标
    const int mwidth, mheight; //整体宽高
    QString SelecBoxImgPath;//选择框图片路径

    SubbutStr SubBut[4];    //四个子按钮

public:
    SelectionBox(QString Path, int width = 200, int height = 200);     //构造

    int GetX() const;     //获取横坐标
    int GetY() const;     //获取横坐标
    int GetWidth() const; //获取宽
    int GetHeight() const;//获取高
    QString GetImgPath() const; //获取选择框图片路径
    bool GetDisplay() const;  //获取显示状态
    void SetDisplay(const bool SetPlay); //设置显示状态
    void CheckTower(int x, int y); //选中防御塔
    SubbutStr* GetSelSubBut(); //获取子按钮结构数组
};

#endif // SELECTIONBOX_H
