//
// Created by john on 27/10/2019.
//

#ifndef WSS_SCORES_H
#define WSS_SCORES_H

#include <array>
#include <limits>

using letter_values = std::array<int, std::numeric_limits<char>::max()+1>;

constexpr auto full_rack_score_index = '\0';

constexpr letter_values scrabble_scores()
{
    constexpr auto full_rack_bonus = 0;
    constexpr auto letter_score_a = 1;
    constexpr auto letter_score_b = 3;
    constexpr auto letter_score_c = 3;
    constexpr auto letter_score_d = 2;
    constexpr auto letter_score_e = 1;
    constexpr auto letter_score_f = 4;
    constexpr auto letter_score_g = 2;
    constexpr auto letter_score_h = 4;
    constexpr auto letter_score_i = 1;
    constexpr auto letter_score_j = 8;
    constexpr auto letter_score_k = 5;
    constexpr auto letter_score_l = 1;
    constexpr auto letter_score_m = 3;
    constexpr auto letter_score_n = 1;
    constexpr auto letter_score_o = 1;
    constexpr auto letter_score_p = 3;
    constexpr auto letter_score_q = 10;
    constexpr auto letter_score_r = 1;
    constexpr auto letter_score_s = 1;
    constexpr auto letter_score_t = 1;
    constexpr auto letter_score_u = 1;
    constexpr auto letter_score_v = 4;
    constexpr auto letter_score_w = 4;
    constexpr auto letter_score_x = 8;
    constexpr auto letter_score_y = 4;
    constexpr auto letter_score_z = 10;

    letter_values scores{};
    scores[full_rack_score_index] = full_rack_bonus;
    scores['A'] = letter_score_a;
    scores['B'] = letter_score_b;
    scores['C'] = letter_score_c;
    scores['D'] = letter_score_d;
    scores['E'] = letter_score_e;
    scores['F'] = letter_score_f;
    scores['G'] = letter_score_g;
    scores['H'] = letter_score_h;
    scores['I'] = letter_score_i;
    scores['J'] = letter_score_j;
    scores['K'] = letter_score_k;
    scores['L'] = letter_score_l;
    scores['M'] = letter_score_m;
    scores['N'] = letter_score_n;
    scores['O'] = letter_score_o;
    scores['P'] = letter_score_p;
    scores['Q'] = letter_score_q;
    scores['R'] = letter_score_r;
    scores['S'] = letter_score_s;
    scores['T'] = letter_score_t;
    scores['U'] = letter_score_u;
    scores['V'] = letter_score_v;
    scores['W'] = letter_score_w;
    scores['X'] = letter_score_x;
    scores['Y'] = letter_score_y;
    scores['Z'] = letter_score_z;
    return scores;
}

constexpr letter_values wwf_scores()
{
    constexpr auto full_rack_bonus = 35;
    constexpr auto letter_score_a = 1;
    constexpr auto letter_score_b = 4;
    constexpr auto letter_score_c = 4;
    constexpr auto letter_score_d = 2;
    constexpr auto letter_score_e = 1;
    constexpr auto letter_score_f = 4;
    constexpr auto letter_score_g = 3;
    constexpr auto letter_score_h = 3;
    constexpr auto letter_score_i = 1;
    constexpr auto letter_score_j = 10;
    constexpr auto letter_score_k = 5;
    constexpr auto letter_score_l = 2;
    constexpr auto letter_score_m = 4;
    constexpr auto letter_score_n = 2;
    constexpr auto letter_score_o = 1;
    constexpr auto letter_score_p = 4;
    constexpr auto letter_score_q = 10;
    constexpr auto letter_score_r = 1;
    constexpr auto letter_score_s = 1;
    constexpr auto letter_score_t = 1;
    constexpr auto letter_score_u = 2;
    constexpr auto letter_score_v = 5;
    constexpr auto letter_score_w = 4;
    constexpr auto letter_score_x = 8;
    constexpr auto letter_score_y = 3;
    constexpr auto letter_score_z = 10;

    letter_values scores{};
    scores[full_rack_score_index] = full_rack_bonus;
    scores['A'] = letter_score_a;
    scores['B'] = letter_score_b;
    scores['C'] = letter_score_c;
    scores['D'] = letter_score_d;
    scores['E'] = letter_score_e;
    scores['F'] = letter_score_f;
    scores['G'] = letter_score_g;
    scores['H'] = letter_score_h;
    scores['I'] = letter_score_i;
    scores['J'] = letter_score_j;
    scores['K'] = letter_score_k;
    scores['L'] = letter_score_l;
    scores['M'] = letter_score_m;
    scores['N'] = letter_score_n;
    scores['O'] = letter_score_o;
    scores['P'] = letter_score_p;
    scores['Q'] = letter_score_q;
    scores['R'] = letter_score_r;
    scores['S'] = letter_score_s;
    scores['T'] = letter_score_t;
    scores['U'] = letter_score_u;
    scores['V'] = letter_score_v;
    scores['W'] = letter_score_w;
    scores['X'] = letter_score_x;
    scores['Y'] = letter_score_y;
    scores['Z'] = letter_score_z;

    return scores;
}

#endif //WSS_SCORES_H
