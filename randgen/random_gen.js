import { appendFile } from 'fs/promises';
const num_idxlist = [[48, 57]];
const alpha_idxlist = [[65, 90], [97, 122]];
const special_idxlist = [[33, 47], [58, 64], [91, 96], [123, 126]];
// 33 ~ 47, 58 ~ 64, 91 ~ 96, 123 ~ 126
// 숫자 48 ~ 57
// 알파벳 65 ~ 90, 97 ~ 122
function charCodeGenerator(chlist) {
    const retarr = [];
    for (const [start, end] of chlist) {
        for (let num = start; num <= end; num++) {
            retarr.push(String.fromCharCode(num));
        }
    }
    return retarr;
}
/**
 * @param count 비밀번호의 길이. 4 ~ 8개로 지정하자.
 * @param chlist 입력하는 문자 배열
 * @param total_count 만드는 비밀번호의 개수. 기본 값은 10
 */
function randomPasswordGenerator(count, chlist, total_count = 10) {
    const retarr = [];
    const remained = count - chlist.length;
    if (remained < 0) { // 해당하는 비밀번호가 성립할 수 없음.
        return "";
    }
    for (let c = 0; c < total_count; c++) {
        const charr = [];
        for (const arr of chlist) { // 각 타입을 우선 하나씩 넣음.
            charr.push(arr[Math.trunc(Math.random() * arr.length)]);
        }
        for (let i = 0; i < remained; i++) { // 남은 비밀번호 문자 채워넣기.
            const lidx = Math.trunc(Math.random() * chlist.length); // 리스트의 인덱스
            charr.push(chlist[lidx][Math.trunc(Math.random() * chlist[lidx].length)]);
        }
        const password = charr.sort(() => Math.random() - 0.5).join(''); // 배열 한번 더 섞기.
        retarr.push(password);
    }
    return retarr.join('\n') + '\n'; // 섞은거 문자열로 바꿔서 엔터키 붙여서 출력
}
async function generate() {
    const nl = charCodeGenerator(num_idxlist);
    const al = charCodeGenerator(alpha_idxlist);
    const sl = charCodeGenerator(special_idxlist);
    //0 1 2 01 02 12 012
    console.log(nl, nl.length);
    console.log(al, al.length);
    console.log(sl, sl.length);
    for (let count = 4; count <= 8; count++) {
        let pwstr = "";
        pwstr += randomPasswordGenerator(count, [nl]);
        pwstr += randomPasswordGenerator(count, [al]);
        pwstr += randomPasswordGenerator(count, [sl]);
        pwstr += randomPasswordGenerator(count, [nl, al]);
        pwstr += randomPasswordGenerator(count, [nl, sl]);
        pwstr += randomPasswordGenerator(count, [al, sl]);
        pwstr += randomPasswordGenerator(count, [nl, al, sl]);
        await appendFile(`password${count}.txt`, pwstr, {
            encoding: 'utf-8'
        });
    }
}
generate();
