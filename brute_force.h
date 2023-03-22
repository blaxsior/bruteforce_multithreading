#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>    // 스레드
#include <vector>    // 벡터
#include "timer.h" // 타이머
#define LISTLEN 94

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
 * @param pc 프로세서가 지원하는 개수
 * @param size 문자 경우의 수
 * @param str 비교 대상이 되는 문자열
*/
long long brute_force(const unsigned& pc, const long long& size, const std::string& str)
{
    std::cout << str << std::endl;
    const int length = str.length(); // 문자열의 길이
    long long gap = size / pc; // 이 숫자만큼 연산 수행할 예정
    std::vector<std::thread> thread_list;
    bool find = false;
    long long find_idx = -1;

    for (unsigned i = 0; i < pc; i++) // 환경이 가진 스레드 개수만큼 돌린다.
    {
        // 스레드 추가
        std::thread t([&find, &length, &pc, &str, &find_idx, &size](int sidx)
            {
                for (long long idx = sidx; idx < size; idx+= pc) {
                    if (find) {
                        break; // 발견하면 나간다.
                    }
                    std::vector<char> fvec(length);
                    long long cur = idx; // 현재 인덱스 길이...

                    // 생성될 문자열 각 자리의 인덱스 처리
                    for (int j = 0; j < length; j++) {
                        fvec[length - j - 1] = chlist[cur % LISTLEN];
                        cur /= LISTLEN;
                    }

                    std::string result(fvec.begin(), fvec.end()); // 문자열 생성
                    if (result.compare(str) == 0) {
                        std::cout << "find[ " << result << " : " << idx << " ]\n"; // 찾았으면 위치 출력하고
                        find_idx = idx; // 찾은 인덱스 얻기
                        find = true;
                    }
                } }, i);
        thread_list.push_back(std::move(t));
    }

    for (auto& th : thread_list)
    {
        th.join();
    }

    return find_idx;
}
/*
* @param length 문자열 길이
*/
void brute_force_all(const int &length, const std::string& path)
{
    const auto processor_count = std::thread::hardware_concurrency();
    // 코어 개수 가져오기. 이거 기반으로 스레드 돌릴 예정.

    if (length < 4 || length > 8)
    {
        return; // 조건에 맞을때만 처리한다.
    }

    std::ifstream stream;
    stream.open(path, std::ios_base::in); // 읽기 모드로 오픈

    long long full_size = 1; // 길이 지정하는 코드
    for (unsigned i = 0; i < length; i++)
    {
        full_size *= LISTLEN; // 나중에 하드코딩에서 바꿀 수 있음.
    }

    MyTimer timer;

    int count = 0; // 현재 어디까지 진행했는지?
    long long sum = 0; // 시간 초기화 용도
    std::string curstr;
    while (stream >> curstr)
    {
        count++;
        if (count % 10 == 1)
        {
            cout <<"case: "<< count <<"##########"<< '\n';
            sum = 0;
        }
        timer.start();

        long long idx = brute_force(processor_count, full_size, curstr);
        cout << "idx: "<<idx << endl;
        timer.end();
        std::cout << timer.getTimeLapse() << '\n';
        sum += timer.getTimeLapse();
        if (count % 10 == 0)
        {
            std::cout << "average time: " << sum / 10 << '\n';
            cout << "##########################################" << '\n';
        }
    }
}