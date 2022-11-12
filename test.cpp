#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

struct shift_combination
{
    int how_many_people_dealt = 0;
    int day_off_better = 0;
    int decreased_weight = 0;
    int decreased_weight_that_round = 0;
    int night_shift_cnt = 0;
    // 假設root為7, 那就只能加入7以後的shifts
    int root;
    deque<int> the_last_couple_shifts;
    vector<int> shift_cnt;
    vector<int> curr_fulfill_worker_demand;
};

bool compare_by_decreased_weight_that_round(const shift_combination &a, const shift_combination &b)
{
    return a.decreased_weight_that_round > b.decreased_weight_that_round;
}

void find_one_day_shift_combination_candidates(int day, int ni, int nj, int nk, int night_shifts[], int worker_demand[50][25], int shift[50][25])
{
    int day_off_threshold = ni / 7;
    deque<shift_combination> return_val;
    deque<shift_combination> temp_save_added_shift_comb;
    vector<int> for_init_use;
    for (int i = 0; i <= nk; i++)
    {
        for_init_use.push_back(0);
    }

    // 先找班表組成一個tetris
    int tetris_arr[25] = {0}, use_which_shift[32] = {0}, each_shift_fill_zero_cnt[32] = {0};
    int tetris_yet = 1, shift_fill_zero_cnt = 0;
    while(tetris_yet)
    {
        int max = -1, max_pos = -1;
        // 
        for(int i = 1; i <= nk ; i++)
        {
            shift_fill_zero_cnt = 0;
            for(int time = 1; time <= 24; time++)
            {
                if(tetris_arr[time] || shift[i][time])
                {
                    if(tetris_arr[time] && shift[i][time])
                    {
                        continue;
                    }
                    shift_fill_zero_cnt++;
                }
            }
            each_shift_fill_zero_cnt[i] = shift_fill_zero_cnt;
            if(shift_fill_zero_cnt > max)
            {
                max = shift_fill_zero_cnt;
                max_pos = i;
            }
        }
        use_which_shift[max_pos] = 1;
        for(int time = 1; time <= 24; time ++)
        {
            tetris_arr[time] += shift[max_pos][time];
        }
        for(int time = 1; time <= 24; time ++)
        {
            if(!tetris_arr[time])
            {
                tetris_yet = 1;
                break;
            }
            else
            {
                if(time == 24)
                {
                    tetris_yet = 0;
                }
            }
        }
    }

    for(int i = 1; i <= 24; i++)
    {
        cout << tetris_arr[i];
    }
    // 把workerdemand一路扣tetris扣到上限
    int stop = 1, count = 0;
    int tetris_fill[24] = {0};
    shift_combination the_first_batch;
    while(stop)
    {
        count ++;
        for(int time = 1; time <= 24; time ++)
        {
            tetris_fill[time] += tetris_arr[time];
            if(tetris_fill[time] > worker_demand[day][time])
            {
                stop = 0;
                count --;
                for(int i = 1; i <= time; i++)
                {
                    tetris_fill[time] -= tetris_arr[time];
                }
                break;
            }
        }
    }
    // 東西丟到first batch裏面
    the_first_batch.how_many_people_dealt = count;
    for(int i = 1; i <= 24; i ++)
    {
        the_first_batch.curr_fulfill_worker_demand.push_back(tetris_fill[i]);
    }
    the_first_batch.shift_cnt.push_back(0);
    for(int i = 1; i <= nk; i ++)
    {
        the_first_batch.shift_cnt.push_back(use_which_shift[i]*count);
    }
}

int main()
{
    int ni = 40, nj = 31, nk =11, l, w1, w2, r, lack_of_worker = 0;
    int shift[50][25], workerdemand[50][25], workernum[2000], dayoff[2000], ans[50][25], night_shifts[50] = {0};

    // input
    cin >> ni >> nj >> nk >> l >> w1 >> w2 >> r;
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

    deque<shift_combination> one_day_all_shift_combination_candidates;
    int day = 1;
    find_one_day_shift_combination_candidates(day, ni, nj, nk, night_shifts, workerdemand, shift);
    
}