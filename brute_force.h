#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>    // 스레드
#include <vector>    // 벡터
#include "timer.h" // 타이머
#define LISTLEN 94

// 연구 기반으로 빈도를 설정해도 괜찮을 듯?
// https://www.researchgate.net/figure/Character-count-and-frequency-within-50-547-passwords-of-eight-characters-or-less_tbl4_357876614

const char chlist[94] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                             'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                             'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                             'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                             '!', '"', '#', '$', '%', '&',
                             '\'', '(', ')', '*', '+', ',',
                             '-', '.', '/', ':', ';', '<',
                             '=', '>', '?', '@', '[', '\\',
                             ']', '^', '_', '`', '{', '|',
                             '}', '~'};
// 전체 문자 94개 리스트

/**
* 입력받은 문자열에 대해 brute-force을 진행하는 함수
 * @param pc 프로세서가 지원하는 개수
 * @param size 문자 경우의 수
 * @param str 비교 대상이 되는 문자열
 * @return 발견한 비밀번호의 인덱스(전체 경우의 수 기준)
*/
long long brute_force(const unsigned& pc, const long long& size, const std::string& str)
{
    std::cout << str << " "; // 현재 비교 대상인 문자열
    const int length = str.length(); // 문자열의 길이
    //long long gap = size / pc; // 이 숫자만큼 연산 수행할 예정
    std::vector<std::thread> thread_list; // 스레드 리스트
    bool find = false; // 결과를 발견했는지 -> 스레드 for문 탈출 조건
    long long find_idx = -1; // 발견한 인덱스 값.

    for (unsigned i = 0; i < pc; i++) // 환경이 가진 스레드 개수만큼 돌린다.
    {
        // 스레드 추가 ( 람다로 작업 시키기 )
        std::thread t([&find, &length, &pc, &str, &find_idx, &size](int sidx)
            {
                for (long long idx = sidx; idx < size; idx+= pc) {
                    if (find) {
                        break; // 발견하면 나간다.
                    }
                    std::vector<char> fvec(length);
                    long long cur = idx; // 현재 인덱스 길이...
                    bool cmp_check = true;
                    // 생성될 문자열 각 자리의 인덱스 처리
                    for (int j = 0; j < length; j++) {
                        fvec[length - j - 1] = chlist[cur % LISTLEN];
                        cur /= LISTLEN;
                        //if (chlist[cur % LISTLEN] != str[length - j - 1]) {
                        //    cmp_check = false;
                        //    break;
                        //}

                    }
                    //if (cmp_check) {
                    //    std::cout << "find[ " << str << " : " << idx << " ] "; // 찾았으면 위치 출력하고
                    //    find_idx = idx; // 찾은 인덱스 얻기
                    //    find = true; // 발견했으므로 모든 스레드 탈출
                    //}
                    std::string result(fvec.begin(), fvec.end()); // 문자열 생성
                    if (result.compare(str) == 0) {
                        find_idx = idx; // 찾은 인덱스 얻기
                        find = true; // 발견했으므로 모든 스레드 탈출
                    }
                } }, i);
        thread_list.push_back(std::move(t));
    }

    for (auto& th : thread_list)
    {
        th.join(); // 모든 스레드 끝날때까지 대기하기
    }

    return find_idx;
}
/*
* 문자열 길이와 비밀번호 리스트가 담긴 파일 이름을 입력받고,
* 해당 비밀번호들에 대해 각각 브루트포스를 수행.
* 파일 읽기, 시간 측정 등의 작업을 수행한다.
* @param length 문자열 길이
* @param path 비밀번호 파일 이름
* @param min_len 비밀번호의 최소 길이
* @param max_len 비밀번호의 최대 길이
*/
void brute_force_all(const std::string& path, const int& min_len = 4, const int& max_len = 8)
{
    const auto processor_count = std::thread::hardware_concurrency();
    // 코어 개수 가져오기. 이거 기반으로 스레드 돌릴 예정.

    if (min_len > max_len)
    {
        return; // 조건에 맞을때만 처리한다.
    }

    std::ifstream stream;
    stream.open(path, std::ios_base::in); // 읽기 모드로 오픈
    
    long long default_size = 1; // 길이 지정하는 코드
    for (unsigned i = 0; i < min_len; i++) // 최소 길이만큼 곱해둔다.
    {
        default_size *= LISTLEN; // 나중에 하드코딩에서 바꿀 수 있음.
    }
    long long inner_size = default_size;

    MyTimer timer;

    int count = 0; // 현재 어디까지 진행했는지?
    long long sum = 0; // 평균 시간 측정 용도
    int word_len = min_len;
    std::string curstr;
    while (stream >> curstr)
    {
        count++;
        if (count % 10 == 1)
        {
            cout <<"case: "<< count <<"##########"<< '\n';
            sum = 0;
        }
        timer.start(); // 작업 시작
        long long idx = -1;
        while (idx < 0 && word_len <= max_len) {
            idx = brute_force(processor_count, inner_size, curstr);
            inner_size *= LISTLEN;
            word_len++;
        }
        
        timer.end(); // 작업 끝
        /*다음 단계를 위해 값 초기화*/
        inner_size = default_size;
        word_len = min_len;
        /**/
        if (idx >= 0) {
            std::cout << "find[ " << curstr << " : "
                << idx << " ] "
                << timer.getTimeLapse() << std::endl; // 찾았으면 위치 출력하고
        }
        else {
            std::cout << "cannot find " << curstr << std::endl;
        }
        
        sum += timer.getTimeLapse();
        if (count % 10 == 0)
        {
            std::cout << "average time: " << sum / 10 << '\n';
            cout << "##########################################" << '\n';
        }
    }
}