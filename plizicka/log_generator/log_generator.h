#ifndef _LOG_GENERATOR
#define _LOG_GENERATOR

#include <string>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <list>

#define CURVE_STEP 2
#define CURVE_LENGTH 40
#define CURVE_MIN_Y 0
#define CURVE_MAX_Y 5

#define MIN_YEAR 2022
#define MAX_YEAR 2022
#define MIN_MONTH 2
#define MAX_MONTH 12
#define MIN_DAY 1
#define MAX_DAY_SHORT 30
#define MAX_DAY_LONG 31
#define MAX_DAY_NON_LAP 28
#define MAX_DAY_LAP 29

#define DOC_ID_LENGTH 10
#define MIN_LOGS 50
#define MAX_LOGS 70

#define LOCATION_MIN_LETTER 'A'
#define LOCATION_MAX_LETTER 'Z'
#define LOCATION_MIN_NUM 1
#define LOCATION_MAX_NUM 26
#define LOCATION_MIN_COUNT 4
#define LOCATION_MAX_COUNT 6

#define MIN_COMMON_LOCS_CNT 1
#define MAX_COMMON_LOCS_CNT 2

#define TEAM_COUNT 8
#define STATION_COUNT 8
#define CAMP_COUNT 5

int get_rand(int min, int max);

//month types by day count
enum Month_type { LONG, SHORT, LAP_DEPENDENT };
//month lenghts
const Month_type month_length[12] = { LONG, LAP_DEPENDENT, LONG, SHORT, LONG, SHORT, LONG, LONG, SHORT, LONG, SHORT, LONG };

/**
 * @brief Date type { year, month, day }
 * 
 * @details
 * Methods:
 * to_string()
 * value()
 * init_random()
 */
struct date_t
{
    int year;
    int month;
    int day;

    //to string in format DD/MM/YYYY
    public: std::string to_string()
    {
        return  std::to_string(year) + "/" + (month < 10 ? "0" : "") + std::to_string(month) + "/" + (day < 10 ? "0" : "") + std::to_string(day);
    }

    public: int value()
    {
        int value = (year - MIN_YEAR) * 365 + month_days() + day;

        for (int i = MIN_YEAR; i < year; i++)
        {
            value += (is_lap(i) ? 1 : 0);
        }

        return value;
    }

    private: int month_days()
    {
        int value = 0;
        for (uint8_t i = 0; i < month; i++)
        {
            switch (month_length[i])
            {
            case SHORT:
                value += MAX_DAY_SHORT;
                break;
            case LONG:
                value += MAX_DAY_LONG;
                break;
            case LAP_DEPENDENT:
                if (is_lap())
                    value += MAX_DAY_LAP;
                else
                    value += MAX_DAY_NON_LAP;
                break;
            default:
                //month_length is type Month_type {SHORT, LONG, LAP_DEPENDENT}
                std::cerr << "\nERR: month type is of unspecified type\n";
                throw std::exception();
                break;
            }
        }
        return value;
    }

    public: void init_random()
    {
        year = get_rand(MIN_YEAR, MAX_YEAR);
        month = get_rand(MIN_MONTH, MAX_MONTH);
        switch (month_length[month - 1])
        {
        case SHORT:
            day = get_rand(MIN_DAY, MAX_DAY_SHORT);
            break;

        case LONG:
            day = get_rand(MIN_DAY, MAX_DAY_LONG);
            break;

        case LAP_DEPENDENT:
            if (is_lap())
                day = get_rand(MIN_DAY, MAX_DAY_LAP);
            else
                day = get_rand(MIN_DAY, MAX_DAY_NON_LAP);
            break;

        default:
            //month_length is type Month_type {SHORT, LONG, LAP_DEPENDENT}
            std::cerr << "\nERR: month type is of unspecified type\n";
            throw std::exception();
            break;
        }
    }

    private: bool is_lap()
    {
        return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    }

    private: bool is_lap(int y)
    {
        return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
    }
};

/**
 * @brief Time type { h, m, s }
 * 
 * @details
 * Methods:
 * to_string()
 * value()
 * init_random()
 */
struct log_time_t
{
    uint8_t h;
    uint8_t m;
    uint8_t s;

    public: std::string to_string()
    {
        return (h < 10 ? "0" : "") + std::to_string(h) + ":" + (m < 10 ? "0" : "") + std::to_string(m) + ":" + (s < 10 ? "0" : "") + std::to_string(s);
    }

    public: int value()
    {
        return h * 3600 + m * 60 + s;
    }

    public: void init_random()
    {
        h = get_rand(0, 23);
        m = get_rand(0, 59);
        s = get_rand(0, 59);
    }
    
};

/**
 * @brief Curve type { points[CURVE_LENGTH] }
 * 
 * @details
 * stores y coords of curve (if generated by init_random(), then in range from CURVE_MIN_Y to CURVE_MAX_Y)
 * 
 * Methods:
 * to_latex()
 * init_random()
 */
struct curve_t
{
    uint8_t points[CURVE_LENGTH];

    /**
     * @brief initializes random values (from CURVE_MIN_Y to CURVE_MAX_Y) for all points
     * 
     */
    public: void init_random()
    {
        for (uint8_t i = 0; i < CURVE_LENGTH; i++)
        {
            points[i] = get_rand(CURVE_MIN_Y, CURVE_MAX_Y);
        }
    }

    /**
     * @brief returns latex code to draw the curve using tikz package
     * 
     * @return std::string curve code in tikz
     */
    public: std::string to_latex()
    {

        std::string out = "\\resizebox{0.33\\columnwidth}{!}{\n"
                          "\t\\begin{tikzpicture}\n"
                          "\t\t\\draw [draw=none] (0,0) -- (0,";
                          out += std::to_string(CURVE_MAX_Y);
                          out += ");\t%%set height\n"
                          "\t\t\\draw [line width=2mm, black] plot [smooth, tension=0.7] coordinates { ";

        //write points
        for (uint8_t i = 0; i < CURVE_LENGTH; i++)
        {
            out += "(" + std::to_string(i * CURVE_STEP) + "," + std::to_string(points[i]) + ") ";
        }

        out += "};\n\t\\end{tikzpicture}}";

        return out;
    }
    /*output of this fnc

    \resizebox{!}{\fontheight}{
        \begin{tikzpicture}
            \draw [draw=none] (0,0) -- (0,4);   %set height
            \draw [line width=2mm, black] plot [smooth, tension=0.7] coordinates { "points (x,y)" };
        \end{tikzpicture}}
    */
};

struct coords_t
{
    char letter;
    uint8_t number;

    public: void add_coords(char c, uint8_t n)
    {
        letter = c;
        number = n;
    }

    public: void init_random()
    {
        letter = get_rand(LOCATION_MIN_LETTER, LOCATION_MAX_LETTER);
        number = get_rand(LOCATION_MIN_NUM, LOCATION_MAX_NUM);
    }
    
    public: std::string to_string()
    {
        return std::to_string(letter) + std::to_string(number);
    }

    bool operator ==(const coords_t coords) const
    {
        return letter == coords.letter && number == coords.number;
    }
};

struct locations_t
{
    uint8_t size;
    coords_t locations[LOCATION_MAX_COUNT];

    public: void init_random()
    {
        size = get_rand(LOCATION_MIN_COUNT, LOCATION_MAX_COUNT);

        do
        {
            for (uint8_t i = 0; i < size; i++)
            {
                locations[i].init_random();
            }
        }while (!check_locations());
    }

    public: void init_random(uint8_t count)
    {
        size = count;

        do
        {
            for (uint8_t i = 0; i < size; i++)
            {
                locations[i].init_random();
            }
        }while (!check_locations());
    }

    /**
     * @brief Generate new locations on top of existing
     * 
     * @param count count of NEWLY generated locations
     * @param dont_touch already generated or given locations, init won't touch
     */
    public: void init_random(uint8_t count, uint8_t dont_touch)
    {
        size += count;

        do
        {
            for (uint8_t i = dont_touch; i < size; i++)
            {
                locations[i].init_random();
            }
        }while (!check_locations());
    }

    public: void shuffle()
    {
        for (uint8_t i = 0; i < size; i++)
        {
            std::swap(locations[i], locations[get_rand(0, size - 1)]);
        }
    }

    private: bool check_locations()
    {
        for (uint8_t i = 0; i < size - 1; i++)
        {
            for (uint8_t j = i + 1; j < size; j++)
            {
                if (locations[i] == locations[j])
                    return false;
            }
        }

        return true;
    }

    public: std::string to_string()
    {
        std::string out;
        out += locations[0].letter;
        out += std::to_string(locations[0].number);
        for (uint8_t i = 1; i < size; i++)
        {
            out += ", ";
            out += locations[i].letter;
            out += std::to_string(locations[i].number);
        }

        return out;
    }
};

/**
 * @brief Log type { id, date, time, curve, locations }
 * 
 */
struct log_t
{
    date_t date;
    log_time_t time;
    curve_t curve;
    locations_t locations;
    unsigned int value;

    public: void init_random()
    {
        date.init_random();
        time.init_random();
        curve.init_random();
        locations.init_random();
        value = date.value() * 24 * 60 * 60 + time.value();
    }

    public: void get_value()
    {
        value = date.value() * 24 * 60 * 60 + time.value();
    }

    bool operator<(const log_t &log) const
    {
        return value < log.value;
    }
};

struct doc_t
{
    char id[DOC_ID_LENGTH + 1];
    uint16_t log_count;
    std::list<log_t> log_list;

    public: void init_random()
    {
        for (uint8_t i = 0; i < DOC_ID_LENGTH; i++)
        {
            id[i] = '0' + get_rand(0, 9);
        }
        id[DOC_ID_LENGTH] = '\0';
        log_count = get_rand(MIN_LOGS, MAX_LOGS);
        log_t tmp;
        for (int i = 0; i < log_count; i++)
        {
            tmp.init_random();
            log_list.push_back(tmp);
        }
    }
};

struct camp_locations_t
{
    coords_t locations[CAMP_COUNT];
};

struct team_logs_t
{
    log_t logs[CAMP_COUNT];
    locations_t false_locations[CAMP_COUNT][3];

    public: void add_locations (camp_locations_t &camps)
    {
        for (uint8_t i = 0; i < CAMP_COUNT; i++)
        {
            logs[i].date.init_random();
            logs[i].time.init_random();
            logs[i].curve.init_random();
            logs[i].locations.size = 1;
            logs[i].locations.locations[0] = camps.locations[i];
            logs[i].get_value();
        }
    }

    //pushes team logs to random station log list
    //for each team log creates 3 random false logs (only their locations, the rest is same)
    public: void distrubute_logs(doc_t stations[])
    {
        init_random_false_locations();

        log_t tmp_log;

        //indicates which stations have been filled with data in current layer
        //true if they are free, false if they've been used
        //after all are used (current layer is filled) the values are reset (in get_random_station())
        bool station_min[STATION_COUNT];

        //set all mins to true
        for (uint8_t i = 0; i < STATION_COUNT; i++)
            station_min[i] = true;

        uint8_t min_station_cnt = STATION_COUNT;
        uint8_t used_idxs[3];

        for (uint8_t camp = 0; camp < CAMP_COUNT; camp++)
        {
            used_idxs[0] = -1;
            used_idxs[1] = -1;
            
            tmp_log.date.year = logs[camp].date.year;
            tmp_log.date.month = logs[camp].date.month;
            tmp_log.date.day = logs[camp].date.day;
            tmp_log.time = logs[camp].time;
            tmp_log.curve = logs[camp].curve;
            tmp_log.locations = logs[camp].locations;
            tmp_log.get_value();

            for (uint8_t false_location_num = 0; false_location_num < 3; false_location_num++)
            {
                tmp_log.locations.size = false_locations[camp][false_location_num].size + 1;
                for (uint8_t j = 1; j < tmp_log.locations.size; j++)
                {
                    tmp_log.locations.locations[j] = false_locations[camp][false_location_num].locations[j-1];
                }
                tmp_log.locations.shuffle();

                used_idxs[false_location_num] = get_random_station(station_min, min_station_cnt, used_idxs);

                station_min[used_idxs[false_location_num]] = false;
                min_station_cnt--;

                stations[used_idxs[false_location_num]].log_list.push_back(tmp_log);
            }
        }
    }

    private: uint8_t get_random_station(bool min[], uint8_t &station_cnt, uint8_t used[])
    {
        uint8_t idx = -1;

        if (station_cnt == 1)
        {
            //reset
            station_cnt = STATION_COUNT;
            for (uint8_t i = 0; i < STATION_COUNT; i++)
            {
                if (min[i])
                    idx = i;
                else
                    min[i] = true;
            }

            return idx;
        }
        
        do
        {
            idx = get_rand(1, station_cnt);

            for (uint8_t i = 0; i < STATION_COUNT; i++)
            {
                if (min[i])
                    idx--;
                if (idx == 0)
                {
                    idx = i;
                    break;
                }
            }
        } while (idx == used[0] || idx == used[1]);

        return idx;
    }

    /**
     * @brief Initializes random false locations
     * Correct location is given in logs[camp].locations.locations[0]
     * 
     * Creates random number of common and unique locations (only correct location is implicitly common for all)
     * Puts these locations to false locations (not the correcr one)
     */
    private: void init_random_false_locations()
    {
        uint8_t cnt01;
        uint8_t cnt12;
        uint8_t cnt20;
        
        uint8_t cnt0;
        uint8_t cnt1;
        uint8_t cnt2;

        uint8_t false0_idx;
        uint8_t false1_idx;
        uint8_t false2_idx;

        //for each camp generate false locations
        for (uint8_t camp = 0; camp < CAMP_COUNT; camp++)
        {
            false0_idx = 0;
            false1_idx = 0;
            false2_idx = 0;

            //random number of common locations
            cnt01 = get_rand(MIN_COMMON_LOCS_CNT, MAX_COMMON_LOCS_CNT);
            cnt12 = get_rand(MIN_COMMON_LOCS_CNT, MAX_COMMON_LOCS_CNT);
            cnt20 = get_rand(MIN_COMMON_LOCS_CNT, MAX_COMMON_LOCS_CNT);

            //ramdom number of unique locations
            cnt0 = get_rand(std::abs(LOCATION_MIN_COUNT - 1 - cnt01 - cnt20),LOCATION_MAX_COUNT - 1 - cnt01 - cnt20);
            cnt1 = get_rand(std::abs(LOCATION_MIN_COUNT - 1 - cnt12 - cnt01),LOCATION_MAX_COUNT - 1 - cnt12 - cnt01);
            cnt2 = get_rand(std::abs(LOCATION_MIN_COUNT - 1 - cnt20 - cnt12),LOCATION_MAX_COUNT - 1 - cnt20 - cnt12);

            //set sizes acording to generated counts
            false_locations[camp][0].size = cnt0 + cnt01 + cnt20;
            false_locations[camp][1].size = cnt1 + cnt12 + cnt01;
            false_locations[camp][2].size = cnt2 + cnt20 + cnt12;

            uint8_t size_of_all = 1 + cnt0 + cnt1 + cnt2 + cnt01 + cnt12 + cnt20;

            //init all locations
            coords_t *common = (coords_t*) malloc((size_of_all) * sizeof(coords_t));
            //set correct location
            common[0] = logs[camp].locations.locations[0];
            
            //init all locations ||1 correct|common 01|common 12|common 20|unique 0|unique 1|unique 2||
            do
            {
                //don"t touch correct location(1), init rest
                for (uint8_t i = 1; i < size_of_all; i++)
                {
                    common[i].init_random();
                }
            } while (!check_locations(common, size_of_all));
            
            //skip correct location
            uint8_t i = 1;

            //fill common 01
            while (i < cnt01 + 1)
            {
                false_locations[camp][0].locations[false0_idx] = common[i];
                false_locations[camp][1].locations[false1_idx] = common[i];

                false0_idx++;
                false1_idx++;
                i++;
            }

            //fill common 12
            while (i < cnt01 + cnt12 + 1)
            {
                false_locations[camp][1].locations[false1_idx] = common[i];
                false_locations[camp][2].locations[false2_idx] = common[i];

                false1_idx++;
                false2_idx++;
                i++;
            }

            //fill common 20
            while (i < cnt01 + cnt12 + cnt20 + 1)
            {
                false_locations[camp][2].locations[false2_idx] = common[i];
                false_locations[camp][0].locations[false0_idx] = common[i];

                false2_idx++;
                false0_idx++;
                i++;
            }

            //fill unique 0
            while (i < cnt01 + cnt12 + cnt20 + cnt0 + 1)
            {
                false_locations[camp][0].locations[false0_idx] = common[i];

                false0_idx++;
                i++;
            }

            //fill unique 1
            while (i < cnt01 + cnt12 + cnt20 + cnt0 + cnt1 + 1)
            {
                false_locations[camp][1].locations[false1_idx] = common[i];

                false1_idx++;
                i++;
            }

            //fill unique 2
            while (i < cnt01 + cnt12 + cnt20 + cnt0 + cnt1 + cnt2 + 1)
            {
                false_locations[camp][2].locations[false2_idx] = common[i];

                false2_idx++;
                i++;
            }

            free(common);
        }
    }

    private: bool check_locations(coords_t locations[], uint8_t size)
    {
        for (uint8_t i = 0; i < size - 1; i++)
        {
            for (uint8_t j = i + 1; j < size; j++)
            {
                if (locations[i] == locations[j])
                    return false;
            }
        }

        return true;
    }
};



/**
 * @brief Get random value between ints including the bounds
 * 
 * @param min min value
 * @param max max value
 * @return min <= random_value <= max
 */
int get_rand(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

#endif //_LOG_GENERATOR