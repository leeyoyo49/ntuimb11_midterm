#include <iostream>
#include <vector>
#include <deque>
using namespace std;

struct shift_combination
{
    int how_many_people_dealt = 0;
    int decreased_weight = 0;
    int decreased_weight_that_round = 0;
    int night_shift_cnt = 0;
    vector <int> shift_cnt;
    vector <int> curr_fulfill_worker_demand;
};

deque<shift_combination> find_one_day_shift_combination_candidates(int day, int ni, int nj, int nk, int night_shifts[], int worker_demand[50][25], int shift[50][25])
{
    // 班表用多少, 填了多少洞
    int tetrisshift[100], tetriseachtime[100]
    // 先找班表組成 tetris
    while(1)
    {
        int arr[100]儲存每一個班表能填滿多少洞
        int fillmaxhole, fillmaxholeloc
        for(對所有nk迭代)
        {
            for(對所有時間迭代)
            {
                找能夠補多少洞
            }
            if(填比較多洞)
            {
                fillmaxhole就是他
            }
        }
        更新 tetrisshift, tetriseachtime
        滿了就break
    }        

    //看有沒有夜班
    int anynightshift=0;
    for(所有時間)
    {
        假設有夜班 anynightshift =1
        // 就只能跑 int ni/7次
    }
    int count = 0, filled_tetris;

    // 將tetris一直填滿 填到某一個的上限
    if(anynightshift)
    {
        for(count小於ni)
        {
            填filled_tetris
        }
    }
    else
    {
        填filled_tetris
    }
    
    
    // 開始爆破
    for(員工數)
    {
        for(所有班表)
        {
            看 currworkerfill 和 workerdemand
            找填滿最多的進下一輪
        }
    }
}