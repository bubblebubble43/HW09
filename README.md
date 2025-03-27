# HW09
# ⚾ ChatBaseBall

> 숫자 야구 게임을 언리얼 엔진에서 멀티플레이로 구현한 프로젝트

## 📌 소개

**ChatBaseBall**은 플레이어들이 채팅창에 숫자를 입력하여 진행하는 **숫자 야구 게임(Number Baseball)**입니다.  
각 플레이어는 랜덤으로 생성된 비밀 숫자를 추측하며, 서버는 입력된 숫자에 대해 **스트라이크(S)** 와 **볼(B)** 로 판정합니다.  
언리얼 엔진의 **멀티플레이** 시스템을 기반으로, 실시간 채팅과 게임 진행이 가능합니다.

## 🕹️ 주요 기능

- 🎲 **플레이 소개**  
  - 1~9 사이의 중복 없는 3자리 숫자 자동 생성
  - 위치와 숫자가 맞으면 S(Strike), 위치는 틀리지만 숫자가 포함되면 B(Ball), 모두 해당하지 않으면 OUT
  - /123 처럼 "/ + 숫자3자리"를 입력하면 판정을 진행합니다.
  - 기회는 플레이어당 3번
  - /를 입력하지 않으면 일반채팅 가능
 
- 🏆 **게임 결과 처리**  
  - 먼저 3스트라이크 달성 시 게임 승리 및 리셋
  - 모든 플레이어가 기회를 소진하면 자동 무승부 처리

- 💬 **채팅 기반 게임 진행**  
  - 채팅창을 통한 /숫자 입력
  - 판정 결과를 모든 플레이어에게 실시간 브로드캐스트

- 🧑‍🤝‍🧑 **멀티플레이 지원**  
  - 서버-클라이언트 통신을 통해 모든 게임 흐름 처리
