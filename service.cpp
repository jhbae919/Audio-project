#include <iostream>
#include <vector>

using namespace std;

// 오디오 데이터를 빙글빙글 돌며 담아둘 원형 큐 클래스
class AudioBuffer {
private:
    vector<float> buffer;
    int head; // 데이터를 읽을 위치
    int tail; // 데이터를 쓸 위치
    int maxSize;

public:
    // 버퍼 초기화 (원하는 크기만큼 공간 생성)
    AudioBuffer(int size) {
        maxSize = size;
        buffer.resize(maxSize, 0.0f);
        head = 0;
        tail = 0;
    }
    // 데이터 넣기
    void write(float data) {
        // [방어막] 만약 꽉 찼다면? (다음 꼬리 위치가 머리 위치와 같다면)
        if ((tail + 1) % maxSize == head) {
            cout << "경고: 버퍼가 꽉 찼습니다! 데이터를 버립니다." << endl;
            return; // 함수를 즉시 종료해서 덮어쓰기 방지
        }

        buffer[tail] = data; 
        tail = (tail + 1) % maxSize; 
    }

    // 데이터 빼기
    float read() {
        // [방어막] 만약 텅 비었다면? (머리와 꼬리가 같은 위치라면)
        if (head == tail) {
            // cout << "경고: 버퍼가 비어있습니다! 침묵(0.0)을 반환합니다." << endl;
            return 0.0f; // 오디오가 비었을 땐 보통 소리가 안 나는 0을 반환
        }

        float data = buffer[head]; 
        head = (head + 1) % maxSize; 
        
        return data; 
    }
};

int main() {
    cout << "오디오 엔진 알고리즘(원형 큐) 테스트 준비 완료!" << endl;
    
    // 크기가 10인 테스트용 오디오 버퍼 생성
    AudioBuffer myBuffer(10);
    
    return 0;
}
