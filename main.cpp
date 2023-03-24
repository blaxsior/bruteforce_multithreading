// 숫자 알파벳 특수문자
// 알파벳은 대소문자 52개
// 숫자는 0부터 9까지 10개
// 특수문자 ascii 상의 모든 특문 space 제외 -> 32개

// 숫자
// 알파벳
// 특문
// 숫자 알파벳
// 숫자 특문
// 알파벳 특문
// 숫자 알파벳 특문

// 비밀번호 길이 -> 4 ~ 8 패스워드: 4, 5, 6, 7, 8로 5개
// 각 10개씩 크래킹? 실습?
// 시간복잡도 측면에서 설명

// 유형은 모름
// 비밀번호가 4 ~ 8 자리라는 사실 자체는 알고 있다.
#pragma once
//#include "timer.h"
#include "brute_force.h"
#include <iostream>
int main() {
    brute_force_all("password4.txt"); // 파일, 최소 길이, 최대 길이 변경 가능
    return 0;
}