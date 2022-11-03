#include <iostream>
using namespace std;

int main()
{
    // 員工數, 幾天, 幾種班表, 至少休息幾天, 未滿足休假需求權重, 超額夜班權重, 幾個放假需求, 缺幾個人
    int ni, nj, nk, l, w1, w2, r, lack_of_worker=0;
    int shift[50][25], workerdemand[50][25], workernum[3200], dayoff[3200], ans[50][25];
    cin >> ni >> nj >> nk >> l >> w1 >> w2 >> r;
// input
    // 輸入班表 19~24是夜班
    for (int i = 1; i <= nk; i++)
    {
        for (int j = 1; j <= 24; j++)
        {
            cin >> shift[i][j];
        }
    }
    for (int i = 1; i <= 24; i++)
    {
        cin >> shift[0][i];
    }
    // 輸入人力 1~nj天
    for (int i = 1; i <= nj; i++)
    {
        for (int j = 1; j <= 24; j++)
        {
            cin >> workerdemand[i][j];
            lack_of_worker += workerdemand[i][j];
        }
    }
    // 輸入放假需求
    for (int i = 0; i < r; i++)
    {
        cin >> workernum[i];
    }
    for (int i = 0; i < r; i++)
    {
        cin >> dayoff[i];
    }



}