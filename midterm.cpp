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

deque<shift_combination> find_one_day_shift_combination_candidates(int day, int ni, int nj, int nk, int night_shifts[], int worker_demand[50][25], int shift[50][25])
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
    int tetris_arr[26] = {0}, use_which_shift[32] = {0}, each_shift_fill_zero_cnt[32] = {0};
    int tetris_yet = 1, shift_fill_zero_cnt = 0;
    while (tetris_yet)
    {
        int max = -1, max_pos = -1;
        //
        for (int i = 1; i <= nk; i++)
        {
            shift_fill_zero_cnt = 0;
            for (int time = 1; time <= 24; time++)
            {
                if (tetris_arr[time] || shift[i][time])
                {
                    if (tetris_arr[time] && shift[i][time])
                    {
                        continue;
                    }
                    shift_fill_zero_cnt++;
                }
            }
            each_shift_fill_zero_cnt[i] = shift_fill_zero_cnt;
            if (shift_fill_zero_cnt > max)
            {
                max = shift_fill_zero_cnt;
                max_pos = i;
            }
        }
        use_which_shift[max_pos] = 1;
        for (int time = 1; time <= 24; time++)
        {
            tetris_arr[time] += shift[max_pos][time];
        }
        for (int time = 1; time <= 24; time++)
        {
            if (!tetris_arr[time])
            {
                tetris_yet = 1;
                break;
            }
            else
            {
                if (time == 24)
                {
                    tetris_yet = 0;
                }
            }
        }
    }
    // 把workerdemand一路扣tetris扣到上限
    int stop = 1, count = 0;
    int tetris_fill[26] = {0};
    shift_combination the_first_batch;
    while (stop)
    {
        count++;
        for (int time = 1; time <= 24; time++)
        {
            tetris_fill[time] += tetris_arr[time];
            if (tetris_fill[time] > worker_demand[day][time])
            {
                stop = 0;
                count--;
                for (int i = 1; i <= time; i++)
                {
                    tetris_fill[i] -= tetris_arr[i];
                }
                break;
            }
        }
    }

    // 東西丟到first batch裏面
    the_first_batch.how_many_people_dealt = count;
    for (int i = 1; i <= 24; i++)
    {
        the_first_batch.curr_fulfill_worker_demand.push_back(tetris_fill[i]);
    }

    the_first_batch.shift_cnt.push_back(0);
    for (int i = 1; i <= nk; i++)
    {
        the_first_batch.shift_cnt.push_back(use_which_shift[i] * count);
    }

    return_val.push_back(the_first_batch);

    // main computation
    for (int curr_worker_dealt = count; curr_worker_dealt <= ni; curr_worker_dealt++)
    {
        // pop every shift passed last round and add shift 1~nk into curr_dealing_shift_comb
        while (!return_val.empty())
        {
            shift_combination curr_dealing_shift_comb = return_val.front();
            return_val.pop_front();

            for (int add_shift_num = curr_dealing_shift_comb.root; add_shift_num <= nk; add_shift_num++)
            {
                shift_combination for_adding_shift_use = curr_dealing_shift_comb;
                for_adding_shift_use.how_many_people_dealt++;
                for_adding_shift_use.shift_cnt[add_shift_num]++;
                int reduced_demand_amount = 0;
                for (int time = 1; time <= 24; time++)
                {
                    for_adding_shift_use.curr_fulfill_worker_demand[time] += shift[add_shift_num][time];
                    if ((worker_demand[day][time] >= for_adding_shift_use.curr_fulfill_worker_demand[time]) && shift[add_shift_num][time])
                    {
                        reduced_demand_amount++;
                    }
                }
                if (night_shifts[add_shift_num])
                {
                    for_adding_shift_use.night_shift_cnt++;
                    int halfni = ni / 2;
                    if (for_adding_shift_use.night_shift_cnt > halfni)
                    {
                        continue;
                    }
                }

                if (ni - for_adding_shift_use.how_many_people_dealt < 10)
                {
                    for_adding_shift_use.the_last_couple_shifts.push_back(add_shift_num);
                }

                for_adding_shift_use.decreased_weight += reduced_demand_amount;
                for_adding_shift_use.decreased_weight_that_round = reduced_demand_amount;
                temp_save_added_shift_comb.push_back(for_adding_shift_use);
            }
        }

        // find top 10% decreased most worker demand, if same as 10%, go to nxt round also
        sort(temp_save_added_shift_comb.begin(), temp_save_added_shift_comb.end(), compare_by_decreased_weight_that_round);
        int threshold_loc = temp_save_added_shift_comb.size() / 10;
        if (threshold_loc > 50)
        {
            int push_who = threshold_loc / 10;
            for (int i = 0; i < 11; i++)
            {
                return_val.push_back(temp_save_added_shift_comb[push_who * i]);
            }
        }
        else
        {
            int threshold = temp_save_added_shift_comb[threshold_loc].decreased_weight_that_round;
            for (auto candidate_passed : temp_save_added_shift_comb)
            {
                if (candidate_passed.decreased_weight_that_round >= threshold)
                {
                    return_val.push_back(candidate_passed);
                }
            }
        }

        temp_save_added_shift_comb.clear();
        cout << return_val.size() << ' ' << curr_worker_dealt << endl;
    }
    return return_val;
}

int main()
{
    // 員工數, 幾天, 幾種班表, 至少休息幾天, 未滿足休假需求權重, 超額夜班權重, 幾個放假需求, 缺幾個人
    int ni, nj, nk, l, w1, w2, r, lack_of_worker = 0;
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
    one_day_all_shift_combination_candidates = find_one_day_shift_combination_candidates(day, ni, nj, nk, night_shifts, workerdemand, shift);

    // 看一下輸出
    for (auto i : one_day_all_shift_combination_candidates)
    {
        for (auto j : i.shift_cnt)
        {
            cout << j << ' ';
        }
        cout << endl;
        for (auto j : i.curr_fulfill_worker_demand)
        {
            cout << j << ' ';
        }
        cout << endl;
        for (auto j : i.the_last_couple_shifts)
        {
            cout << j << ' ';
        }
        cout << 'n' << i.night_shift_cnt;
        cout << endl << endl;
    }
}