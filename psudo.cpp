#include <iostream>
#include <vector>
#include <deque>
using namespace std;

class one_day_shift_combination{
public:
    int how_many_people_dealt = 0;
    int day_off_better = 0;
    int decreased_weight = 0;
    int night_shift_cnt = 0;
    deque <int> the_last_couple_shifts;
    vector <int> shift_cnt;
};

// bfs + greedy
// void find_best_shift(int 員工數, 幾天, 幾種班表, 弄幾個人了, 所有班表候選, 暫存目前哪些班表用多少)
// {
//     int 複製暫存班表[];
//     int 每個時段目前多少人[]; 
//     int 假設每個班表都塞一輪會補多少缺工額[]
//     //  end condition
//     if(弄幾個人==員工數)
//     {
//         放入所有班表後選
//         return;
//     }

//     for(24h)
//     {
//         計算每個時段目前多少人;
//     }
//     for(i=1,幾種班表)
//     {
//         塞班表i
//         看補多少缺工額
//         丟到假設每個班表都塞一輪會補多少缺工額[]
//     }
//     for(幾種班表)
//     {
//         找補最多的前三名;
//     }
//     for(幾種班表)
//     {
//         假設是前三名 且 放假數沒太多

//         把那個班表塞進去
//         再跑一次函式
//     }
// }
int main()
{
    deque <one_day_shift_combination> que;
    one_day_shift_combination a;
    que.push_back(a);
    one_day_shift_combination b = que.front();
    // // 員工數, 幾天, 幾種班表, 至少休息幾天, 未滿足休假需求權重, 超額夜班權重, 幾個放假需求, 缺幾個人
    // int ni, nj, nk, l, w1, w2, r, lack_of_worker = 0;
    // int shift[50][25], workerdemand[50][25], workernum[2000], dayoff[2000], ans[50][25], night_shifts[50] = {0};
    // int one_day_total_worker_demand[50] = {0};
    // // 存一整個月的候選班表 v[day][候選班表組合n號][班表n] = 那天 班表組合n 的班表n 用了幾次
    // vector < vector < vector <int> > > one_month_arranged_shift_candidates;
    // vector < vector < int > > init_use;
    // for(int i = 0; i < nj; i ++)
    // {
    //     one_month_arranged_shift_candidates.push_back(init_use);
    // }
    // int remember_each_shift_count[50] = {0};

    // cin >> ni >> nj >> nk >> l >> w1 >> w2 >> r;
    // // input
    // // 輸入班表 19~24是夜班
    // for (int i = 1; i <= nk; i++)
    // {
    //     for (int j = 1; j <= 24; j++)
    //     {
    //         cin >> shift[i][j];
    //         if ((19 < j) && (shift[i][j]))
    //         {
    //             night_shifts[i] = 1;
    //         }
    //     }
    // }
    // for (int i = 1; i <= 24; i++)
    // {
    //     cin >> shift[0][i];
    // }
    // // 輸入人力 1~nj天
    // for (int i = 1; i <= nj; i++)
    // {
    //     for (int j = 1; j <= 24; j++)
    //     {
    //         cin >> workerdemand[i][j];
    //         one_day_total_worker_demand[i] += workerdemand[i][j];
    //     }
    // }
    // // 輸入放假需求
    // for (int i = 0; i < r; i++)
    // {
    //     cin >> workernum[i];
    // }
    // for (int i = 0; i < r; i++)
    // {
    //     cin >> dayoff[i];
    // }
    // for(天數)
    // {
    //     find_best_shift();
    // }
}