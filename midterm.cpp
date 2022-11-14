#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

struct worker_and_its_shifts
{
    int number;
    int last_night_shift_n_days_ago = 100;
};

struct shift_combination
{
    int how_many_people_dealt = 0;
    int decreased_weight = 0;
    int decreased_weight_that_round = 0;
    int night_shift_cnt = 0;
    int last_added_shift = -1;
    vector<int> shift_cnt;
    vector<int> curr_fulfill_worker_demand;
};

bool compare_by_decreased_weight_that_round(const shift_combination &a, const shift_combination &b)
{
    if (a.decreased_weight_that_round > b.decreased_weight_that_round)
        return true;
    if (a.decreased_weight_that_round == b.decreased_weight_that_round)
        return a.decreased_weight > b.decreased_weight;
    return false;
}

bool compare_by_decreased_weight(const shift_combination &a, const shift_combination &b)
{
    return a.decreased_weight > b.decreased_weight;
}

shift_combination find_one_day_shift_combination_candidates(int day, int ni, int nj, int nk, int night_shifts[], int each_shift_howmany_one[], int worker_demand[50][25], int shift[50][25])
{
    // find tetris
    int tetrisshift[50] = {0}, tetrisfillhole[25] = {0}, fillmaxhole, fillmaxholeloc, found_tetris = 1, anynightshift = 0, count_threshold = ni / 6, thisshiftfillhowmany, overlimit = 0;
    vector<int> worker_demand_fill_by_tetris;
    for (int i = 0; i <= 24; i++)
    {
        worker_demand_fill_by_tetris.push_back(0);
    }
    while (found_tetris)
    {
        fillmaxhole = 0;
        fillmaxholeloc = -1;

        for (int shiftcnt = 1; shiftcnt <= nk; shiftcnt++)
        {
            thisshiftfillhowmany = 0;
            for (int time = 1; time <= 24; time++)
            {
                if ((tetrisfillhole[time] == 0) && shift[shiftcnt][time])
                {
                    thisshiftfillhowmany++;
                }
            }
            if (thisshiftfillhowmany > fillmaxhole)
            {
                fillmaxhole = thisshiftfillhowmany;
                fillmaxholeloc = shiftcnt;
            }
            else if (thisshiftfillhowmany == fillmaxhole)
            {
                if ((night_shifts[shiftcnt] == 0) && (night_shifts[fillmaxholeloc] == 1))
                {
                    fillmaxhole = thisshiftfillhowmany;
                    fillmaxholeloc = shiftcnt;
                }
                else if ((night_shifts[shiftcnt] == 1) && (night_shifts[fillmaxholeloc] == 0))
                {
                    continue;
                }
                else
                {
                    if (each_shift_howmany_one[shiftcnt] > each_shift_howmany_one[fillmaxholeloc])
                    {
                        fillmaxhole = thisshiftfillhowmany;
                        fillmaxholeloc = shiftcnt;
                    }
                }
            }
        }
        tetrisshift[fillmaxholeloc] = 1;
        for (int time = 1; time <= 24; time++)
        {
            tetrisfillhole[time] += shift[fillmaxholeloc][time];
        }
        for (int time = 1; time <= 24; time++)
        {
            if (tetrisfillhole[time] == 0)
            {
                continue;
            }
            if (time == 24)
            {
                found_tetris = 0;
            }
        }
    }
    for (int shiftcnt = 1; shiftcnt <= nk; shiftcnt++)
    {
        if (night_shifts[shiftcnt] && tetrisshift[shiftcnt])
        {
            anynightshift = 1;
            break;
        }
    }
    if (anynightshift == 1)
    {
        for (int count = 1; count <= count_threshold; count++)
        {
            for (int time = 1; time <= 24; time++)
            {
                int temp = tetrisfillhole[time];
                if (((temp * count) > worker_demand[day][time]) || (count == count_threshold))
                {
                    overlimit = count;
                    break;
                }
            }
            if (overlimit != 0)
            {
                for (int time = 1; time <= 25; time++)
                {
                    worker_demand_fill_by_tetris[time] += tetrisfillhole[time] * overlimit;
                }
                break;
            }
        }
    }
    else
    {
        for (int count = 1; count <= count_threshold; count++)
        {
            for (int time = 1; time <= 24; time++)
            {
                if (((tetrisfillhole[time] * count) > worker_demand[day][time]))
                {
                    overlimit = count;
                    break;
                }
            }
            if (overlimit != 0)
            {
                for (int time = 1; time <= 24; time++)
                {
                    worker_demand_fill_by_tetris[time] += tetrisfillhole[time] * overlimit;
                }
                break;
            }
        }
    }
    // find tetris end

    shift_combination initshift;
    for (int i = 0; i <= nk; i++)
    {
        initshift.shift_cnt.push_back(tetrisshift[i] * overlimit);
        if (tetrisshift[i] >= 1)
        {
            initshift.how_many_people_dealt += tetrisshift[i];
        }
        if (night_shifts[i] && tetrisshift[i])
        {
            initshift.night_shift_cnt++;
        }
    }
    for (int i = 0; i <= 24; i++)
    {
        initshift.curr_fulfill_worker_demand.push_back(tetrisfillhole[i] * overlimit);
        initshift.decreased_weight += tetrisfillhole[i] * overlimit;
    }
    initshift.how_many_people_dealt *= overlimit;
    initshift.night_shift_cnt *= overlimit;

    int minus_tetris_time = overlimit / 2;
    deque<shift_combination> all_shift_candidates;
    deque<shift_combination> save_add_everyshifts;
    all_shift_candidates.push_back(initshift);
    // find best shift
    int last_check = -1;
    for (int workercnt = initshift.how_many_people_dealt; workercnt <= ni; workercnt++)
    {
        while (!all_shift_candidates.empty())
        {
            initshift = all_shift_candidates.front();
            all_shift_candidates.pop_front();
            for (int shiftcnt = 1; shiftcnt <= nk; shiftcnt++)
            {
                int demand_decreased = 0;
                shift_combination for_shift_adding = initshift;
                for (int time = 1; time <= 24; time++)
                {
                    if (((worker_demand[day][time] - for_shift_adding.curr_fulfill_worker_demand[time]) > 0) && shift[shiftcnt][time])
                    {
                        demand_decreased++;
                    }
                    for_shift_adding.curr_fulfill_worker_demand[time] += shift[shiftcnt][time];
                }
                for_shift_adding.how_many_people_dealt++;
                for_shift_adding.decreased_weight_that_round = demand_decreased;
                for_shift_adding.shift_cnt[shiftcnt]++;
                for_shift_adding.decreased_weight += demand_decreased;
                for_shift_adding.last_added_shift = shiftcnt;
                // check if night shift
                if (night_shifts[shiftcnt] == 1)
                {
                    for_shift_adding.night_shift_cnt++;
                    if (for_shift_adding.night_shift_cnt >= count_threshold)
                    {
                        continue;
                    }
                }
                save_add_everyshifts.push_back(for_shift_adding);
            }
        }
        sort(save_add_everyshifts.begin(), save_add_everyshifts.end(), compare_by_decreased_weight_that_round);
        // 取前幾個啦qqqqq
        for (int i = 0; i < 10; i++)
        {
            all_shift_candidates.push_back(save_add_everyshifts.front());
            save_add_everyshifts.pop_front();
            if (save_add_everyshifts.empty())
            {
                break;
            }
        }
        save_add_everyshifts.clear();
        // cout << all_shift_candidates.size() << endl;
        // 每個都拔掉看看
        if (workercnt == ni)
        {
            for (int shiftcnt = 1; shiftcnt <= nk; shiftcnt++)
            {
                if (all_shift_candidates.front().shift_cnt[shiftcnt] > 0)
                {
                    // 該扣得扣一扣
                    int shift_gone_weight_gone = 0;
                    all_shift_candidates.front().shift_cnt[shiftcnt]--;
                    // cout << shiftcnt << ' ' << all_shift_candidates.front().shift_cnt[shiftcnt]  << 'a';

                    for (int time = 1; time <= 24; time++)
                    {
                        if (shift[shiftcnt][time])
                        {
                            all_shift_candidates.front().curr_fulfill_worker_demand[time]--;
                            if (all_shift_candidates.front().curr_fulfill_worker_demand[time] < worker_demand[day][time])
                            {
                                all_shift_candidates.front().decreased_weight--;
                                shift_gone_weight_gone++;
                            }
                        }
                    }
                    if (night_shifts[shiftcnt])
                    {
                        all_shift_candidates.front().night_shift_cnt--;
                    }
                    int finish_whole_thing = 0;
                    // 所有shift塞一遍
                    for (int inshiftcnt = 1; inshiftcnt <= nk; inshiftcnt++)
                    {
                        finish_whole_thing++;
                        if (night_shifts[inshiftcnt] && (all_shift_candidates.front().night_shift_cnt >= count_threshold))
                        {
                            continue;
                        }
                        if (inshiftcnt == shiftcnt)
                        {
                            continue;
                        }

                        int inshiftweight = 0;

                        for (int time = 1; time <= 24; time++)
                        {
                            if ((all_shift_candidates.front().curr_fulfill_worker_demand[time] < worker_demand[day][time]) && shift[inshiftcnt][time])
                            {
                                inshiftweight++;
                            }
                        }
                        if (inshiftweight > shift_gone_weight_gone)
                        {
                            all_shift_candidates.front().shift_cnt[inshiftcnt]++;
                            all_shift_candidates.front().decreased_weight += inshiftweight;
                            for (int time = 1; time <= 24; time++)
                            {
                                all_shift_candidates.front().curr_fulfill_worker_demand[time] += shift[inshiftcnt][time];
                            }
                            if (night_shifts[inshiftcnt])
                            {
                                all_shift_candidates.front().night_shift_cnt++;
                            }
                            shiftcnt = 1;
                            break;
                        }
                    }
                    // 通過就全部塞回去
                    if (finish_whole_thing == nk)
                    {
                        // cout << "aaaaaa";
                        all_shift_candidates.front().shift_cnt[shiftcnt]++;
                        for (int time = 1; time <= 24; time++)
                        {
                            if (shift[shiftcnt][time])
                            {
                                all_shift_candidates.front().curr_fulfill_worker_demand[time]++;
                                if (all_shift_candidates.front().curr_fulfill_worker_demand[time] <= worker_demand[day][time])
                                {
                                    all_shift_candidates.front().decreased_weight++;
                                }
                            }
                        }
                        if (night_shifts[shiftcnt])
                        {
                            all_shift_candidates.front().night_shift_cnt++;
                        }
                    }
                }
                // for(int i = 1; i <= nk; i++)
                // {
                //     cout << all_shift_candidates.front().shift_cnt[i] << ' ';
                // }
                // cout << endl;
            }
        }
    }
    // cout << endl;

    return all_shift_candidates.front();
}

int main()
{
    // 員工數, 幾天, 幾種班表, 至少休息幾天, 未滿足休假需求權重, 超額夜班權重, 幾個放假需求, 缺幾個人
    int ni, nj, nk, l, w1, w2, r, lack_of_worker = 0;
    int shift[50][25], workerdemand[50][25], workernum[2000], dayoff[2000], night_shifts[50] = {0}, ans[102][102], each_shift_howmany_one[50] = {0};
    // input
    cin >> ni >> nj >> nk >> l >> w1 >> w2 >> r;
    for (int i = 0; i <= nj; i++)
    {
        for (int j = 0; j <= ni; j++)
        {
            ans[j][i] = -1;
        }
    }
    // 輸入班表 19~24是夜班
    for (int i = 1; i <= nk; i++)
    {
        for (int j = 1; j <= 24; j++)
        {
            cin >> shift[i][j];
            if (shift[i][j] == 1)
            {
                each_shift_howmany_one[i]++;
            }
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

    shift_combination todayshift;
    deque<shift_combination> whole_month_shift_candidates;

    // get candidates
    for (int day = 1; day <= nj; day++)
    {
        todayshift = find_one_day_shift_combination_candidates(day, ni, nj, nk, night_shifts, each_shift_howmany_one, workerdemand, shift);
        todayshift.how_many_people_dealt = ni;
        whole_month_shift_candidates.push_back(todayshift);
    }

    // for (auto i : whole_month_shift_candidates)
    // {
    //     for (auto j : i.shift_cnt)
    //     {
    //         cout << j << ' ';
    //     }
    //     cout << i.decreased_weight << endl;
    // }

    deque <worker_and_its_shifts> all_workers;
    for(int i = 0; i <= ni; i++)
    {
        worker_and_its_shifts temp;
        temp.number = i;
        all_workers.push_back(temp);
    }
    // 讓他們都放假

    for (int dayoffworker = 0; dayoffworker < r; dayoffworker++)
    {
        int whichday = dayoff[dayoffworker], whichguy = workernum[dayoffworker];
        // cout << whichday << ' ' << whichguy << endl ;
        int smallest_weight = 100000, smallest_weight_shift = -1;
        for (int shiftcnt = 1; shiftcnt <= nk; shiftcnt++)
        {
            int temp_save_weight = 0;
            for (int time = 1; time <= 24; time++)
            {
                if (shift[shiftcnt][time])
                {
                    if (whole_month_shift_candidates[whichday].curr_fulfill_worker_demand[time] <= workerdemand[whichday][time])
                    {
                        temp_save_weight++;
                    }
                }
            }
            if (temp_save_weight < smallest_weight)
            {
                smallest_weight = temp_save_weight;
                smallest_weight_shift = shiftcnt;
            }
            if (temp_save_weight == smallest_weight)
            {
                if ((night_shifts[shiftcnt] == 1) && (night_shifts[smallest_weight_shift] == 0))
                {
                    smallest_weight_shift = shiftcnt;
                }
            }
        }
        if(smallest_weight > w1)
        {
            continue;
        }

        ans[whichguy][whichday] = 0;
        whole_month_shift_candidates[whichday].shift_cnt[smallest_weight_shift]--;
        whole_month_shift_candidates[whichday].decreased_weight -= smallest_weight;
        whole_month_shift_candidates[whichday].how_many_people_dealt--;
        if (night_shifts[smallest_weight_shift])
        {
            whole_month_shift_candidates[whichday].night_shift_cnt--;
        }
        for (int time = 1; time <= 24; time++)
        {
            whole_month_shift_candidates[whichday].curr_fulfill_worker_demand[time] -= shift[smallest_weight_shift][time];
        }
    }

    // 排排夜班
    for (int day = 1; day <= nj; day++)
    {
        for (int worker = 1; worker <= ni; worker++)
        {
            all_workers[worker].last_night_shift_n_days_ago ++;
            if (ans[worker][day] != -1)
            {
                continue;
            }
            if (whole_month_shift_candidates[day].night_shift_cnt == 0)
            {
                break;
            }
            if( all_workers[worker].last_night_shift_n_days_ago >= 7)
            {
                for(int find_night_shift_avai = 1; find_night_shift_avai <= nk; find_night_shift_avai++)
                {
                    if(night_shifts[find_night_shift_avai] == 1)
                    {
                        ans[worker][day] = find_night_shift_avai;
                        whole_month_shift_candidates[day].night_shift_cnt --;
                        whole_month_shift_candidates[day].shift_cnt[find_night_shift_avai] --;
                        whole_month_shift_candidates[day].how_many_people_dealt--;
                        all_workers[worker].last_night_shift_n_days_ago = 0;
                        break;
                    }
                }
            }
        }
    }
    
    // 剩下的班塞一塞
    for(int day = 1; day <= nj; day ++)
    {
        for(int worker = 1; worker <= ni; worker++)
        {
            if(ans[worker][day] == -1)
            {
                for(int shift_avai = 1; shift_avai <= nk; shift_avai++)
                {
                    if(whole_month_shift_candidates[day].shift_cnt[shift_avai])
                    {
                        ans[worker][day] = shift_avai;
                        whole_month_shift_candidates[day].shift_cnt[shift_avai] --;
                        whole_month_shift_candidates[day].how_many_people_dealt--;
                        break;
                    }
                }
            }
        }
    }
    
    // 假設夜班沒排完
    for(int day = 1; day <= nj; day ++)
    {    
        // 把無法使用的夜班從總共全中扣除
        for(int i = 0; i < whole_month_shift_candidates[day].how_many_people_dealt; i++)
        {
            for(int currshift = 1; currshift <= nk; currshift ++)
            {
                if(night_shifts[currshift] && whole_month_shift_candidates[day].shift_cnt[currshift])
                {
                    for(int time = 1; time <= 24; time ++)
                    {
                        whole_month_shift_candidates[day].curr_fulfill_worker_demand[time] -= shift[currshift][time];
                        whole_month_shift_candidates[day].shift_cnt[currshift] --;
                    }
                }
            }
        }    
        // 塞新的
        while(whole_month_shift_candidates[day].how_many_people_dealt--)
        {
            for(int worker = 1; worker <= ni; worker ++)
            {
                if(ans[worker][day] == -1)
                {
                    int max_weight = -1, max_weight_loc = -1, temp_weight_save = 0;
                    for(int currshift = 1; currshift <= nk; currshift ++)
                    {
                        temp_weight_save = 0;
                        if(night_shifts[currshift])
                        {
                            continue;
                        }
                        for(int time = 1; time <= 24; time ++)
                        {
                            if(shift[currshift][time])
                            {
                                if(whole_month_shift_candidates[day].curr_fulfill_worker_demand[time] < workerdemand[day][time])
                                {
                                    temp_weight_save ++;
                                }
                            }
                        }   
                        if(temp_weight_save > max_weight)
                        {
                            temp_weight_save = max_weight;
                            max_weight_loc = currshift;
                        }
                    }
                    ans[worker][day] = max_weight_loc;
                }
            }
        }
    }
    

    // 最後輸出
    for (int worker = 1; worker <= ni; worker++)
    {
        for (int day = 1; day < nj; day++)
        {
            // if((ans[worker][day] == -1) || (ans[worker][day] > nk))
            // {
            //     cout << 1;
            // }
            // else
            // {
            cout << ans[worker][day] << ',';
            // }
        }
        cout << ans[worker][nj] << '\n';
    }
}    