#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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
    } //생성자
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

void generateNoise(AudioBuffer& buf, int generateCount) {
    cout << " [생성기] 소음 " << generateCount << "개 생성 ➡️ 버퍼에 저장\n";
    for(int i = 0; i < generateCount; i++){
        float noise = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        buf.write(noise);
    }
}

// 2. [오디오 콜백] - 스마트폰 스피커가 "데이터 내놔!" 할 때마다 실행되는 함수
void audioCallback(AudioBuffer& buf, int requestCount) {
    cout << " [스피커] 오디오 " << requestCount << "개 요구 ⬅️ 버퍼에서 추출: ";
    for(int i = 0; i < requestCount; i++){
        float sound = buf.read();
        printf("%.2f  ", sound); // 소수점 2자리까지만 깔끔하게 출력
    }
    cout << "\n------------------------------------------------\n";
}

int main() {
    // 앱이 켜질 때 딱 한 번! 현재 시간을 기준으로 랜덤 씨앗(Seed)을 심습니다.
    // 이제 실행할 때마다 매번 다른 백색소음 패턴이 생성됩니다.
    srand(time(NULL)); 
    
    // 크기가 10인 넉넉한 오디오 버퍼 생성
    AudioBuffer myBuffer(10); 

    cout << "🎧 실시간 오디오 콜백 시뮬레이션 시작 (총 5 틱)\n\n";

    // 실제 앱에서는 이 부분이 눈에 보이지 않는 백그라운드에서 무한히 돌아갑니다.
    for(int tick = 1; tick <= 5; tick++) {
        cout << "=== [Tick " << tick << "] ===" << endl;

        // 1. 엔진이 미리 버퍼에 소음을 4개 만들어 둡니다. (Producer)
        generateNoise(myBuffer, 4); 

        // 2. 찰나의 순간, 스마트폰 스피커가 소리를 4개 뺏어갑니다. (Consumer)
        audioCallback(myBuffer, 4);
    }
    
    cout << "\n✅ 시뮬레이션 정상 종료!" << endl;
    return 0;
}
