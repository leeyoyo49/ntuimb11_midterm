#include <iostream>
#include <vector>
using namespace std;
// 目前第幾天, 這是組合班表n, 從班表root開始的（不能看root以前的), 弄了幾個員工了
void find_best_shift(int day, int curr_candidate_num, int root, int how_many_worker_dealt, int nk, int ni, int one_month_arranged_shift_candidates[32][50][101], int shift[50][25], int workerdemand[50][25], int night_shifts[50])
{
    // 人滿了就停掉
    if(how_many_worker_dealt == ni)
    {
        return;
    }

    int temp_arranged_shift[50];
    for(int i = 0; i <= nk; i++)
    {
        temp_arranged_shift[i] = one_month_arranged_shift_candidates[day][curr_candidate_num][i];
    }
    // 目前每個時段有多少員工
    int curr_satisfied_worker[25] = {0};
    for (int shiftnum = 1; shiftnum <= nk; shiftnum++)
    {
        int shift_used = one_month_arranged_shift_candidates[day][curr_candidate_num][shiftnum];
        for (int time = 1; time <= 24; time++)
        {
            if (shift[shiftnum][time])
            {
                curr_satisfied_worker[time] += shift_used;
            }
        }
    }
    // 把缺工去減掉每個時段有多少員工, 小魚0設為0
    for (int time = 1; time <= 24; time++)
    {
        curr_satisfied_worker[time] = workerdemand[day][time] - curr_satisfied_worker[time];
        if (curr_satisfied_worker[time] < 0)
        {
            curr_satisfied_worker[time] = 0;
        }
    }

    // 減少了多少班數
    int each_shift_dealt_worker[32] = {0};
    // 每個班表都塞一次
    for (int shiftnum = 1; shiftnum <= nk; shiftnum++)
    {
        for (int time = 1; time < 24; time++)
        {
            if (curr_satisfied_worker[time] && shift[shiftnum][time])
            {
                each_shift_dealt_worker[shiftnum]++;
            }
        }
    }

    // 前三高存在這
    int top_ranks[50];
    int top_rank_cnt = 0;
    int max1 = max(each_shift_dealt_worker[1], max(each_shift_dealt_worker[2], each_shift_dealt_worker[3])), max2 = -1, max3 = -1;
    // 找前三高
    for (int i = 1; i <= nk; i++)
    {
        if (each_shift_dealt_worker[i] > max3)
        {
            if (each_shift_dealt_worker[i] > max2)
            {
                if (each_shift_dealt_worker[i] > max1)
                {
                    max1 = each_shift_dealt_worker[i];
                    continue;
                }
                max2 = each_shift_dealt_worker[i];
                continue;
            }
            max3 = each_shift_dealt_worker[i];
            continue;
        }
    }
    how_many_worker_dealt ++;
    int night_shift_threshold = ni/7, curr_candidate_num_add = 0;
    // 如果同時是前三高, 夜班又沒太多, 就遞迴
    for (int i = 1; i <= nk; i++)
    {
        if ((each_shift_dealt_worker[i] == max1) || (each_shift_dealt_worker[i] == max2) || (each_shift_dealt_worker[i] == max3))
        {
            int night_shift_cnt = 0;
            for (int j = 1; j <= nk; j++)
            {
                if((j == i) && night_shifts[j])
                {
                    night_shift_cnt ++;
                }
                else if(night_shifts[j] && (one_month_arranged_shift_candidates[day][curr_candidate_num][j]))
                {
                    night_shift_cnt += (one_month_arranged_shift_candidates[day][curr_candidate_num][j]);
                }

                if(night_shift_cnt > night_shift_threshold)
                {
                    break;
                }
                if(j == nk)
                {
                    for(int k = 1; k < nk; k++)
                    {
                        one_month_arranged_shift_candidates[day][curr_candidate_num+curr_candidate_num_add][k] = temp_arranged_shift[k];
                    }
                    one_month_arranged_shift_candidates[day][curr_candidate_num+curr_candidate_num_add][nk] ++;
                    find_best_shift(day, curr_candidate_num+curr_candidate_num_add, root, how_many_worker_dealt, nk, ni, one_month_arranged_shift_candidates, shift, workerdemand, night_shifts);
                    curr_candidate_num_add++;

                }
            }
        }
    }
    return;
}


int main()
{
    // 員工數, 幾天, 幾種班表, 至少休息幾天, 未滿足休假需求權重, 超額夜班權重, 幾個放假需求, 缺幾個人
    int ni, nj, nk, l, w1, w2, r, lack_of_worker = 0;
    int shift[50][25], workerdemand[50][25], workernum[2000], dayoff[2000], ans[50][25], night_shifts[50] = {0};
    int one_day_total_worker_demand[50] = {0};
    // 存一整個月的候選班表 v[day][候選班表組合n號][班表n] = 那天 班表組合n 的班表n 用了幾次
    int one_month_arranged_shift_candidates[32][50][101];

    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            for (int k = 0; k < 101; j++)    
            {
                one_month_arranged_shift_candidates[i][j][k] = 0;
            }
        }
    }
    cin >> ni >> nj >> nk >> l >> w1 >> w2 >> r;
    // input
    // 輸入班表 19~24是夜班
    for (int i = 1; i <= nk; i++)
    {
        for (int j = 1; j <= 24; j++)
        {
            cin >> shift[i][j];
            if ((19 < j) && (shift[i][j]))
            {
                night_shifts[i] = 1;
            }
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
            one_day_total_worker_demand[i] += workerdemand[i][j];
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

    for(int day = 1; day <= nj; day++)
    {
        find_best_shift(day, 0, 1, 0, nk, ni, one_month_arranged_shift_candidates, shift, workerdemand, night_shifts);
    }

    for(int i = 0; i < 31; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            for(int k = 0; k <= nk; k++)
            {
                cout << one_month_arranged_shift_candidates[i][j][k];
            }
        }
    }
}