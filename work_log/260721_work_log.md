# 개발 일지 — 2026-07-21

> 프로젝트명: `CNN 가속기 설계 프로젝트`  
> 작성자: `김동현`  
> 태그: `#CNN` `#C`

---

## 1. 오늘의 목표
<!-- 작업 시작 전, 오늘 하려던 것을 적습니다 -->
- [x] Max pooling과 관련한 오류 디버깅
- [x] Convolution 연산 유닛 하드웨어 친화적으로 리팩토링 작업 수행

---

## 2. 수행 내용
<!-- 실제로 한 작업과 '왜 그렇게 했는지'를 함께 적습니다 -->

### 2.1 작업 내용

*Convolution layer (Single) C Code 제작 완료*
- Convolution 연산, Line buffer, Max pooling을 포함하는 Convolution layer 코드 작성 완료
- 최대한 하드웨어 친화적으로 작성
- Sequential Logic이 포함되는 경우, 구조체를 선언하여 다른 값이 저장될 수 있도록 함
- <주의> 현재 Convolution연산 코드의 경우, valid가 한 clk 지연되는 것을 표현하기 위해 이후 verilog로 옮기기 위한 방향과 다르게 구현되어 있으므로 주의하며 옮길 것


### 2.2 자료
<!-- 이미지는 같은 폴더의 images/ 안에 두고 아래처럼 링크합니다 -->


---


## 3. 문제 및 디버깅
<!-- 포트폴리오에서 가장 중요한 부분. 사고의 흐름을 남깁니다 -->

### 문제 #1
- **증상**: max pooling 연산이 정상적으로 수행되지 않는 것 확인
- **확인 과정**: 
  1. Max pooling 코드의 경우 매우 간단하여 연산 과정에서 문제가 생기기 어려움
  2. Max pooling의 입력을 확인한 결과 실제 convolution 연산 결과로 인해 기대하는 값이 아닌 다른 값이 입력되는 것 확인
  3. line buffer가 문제라 생각
- **원인**: line buffer를 convolution 연산에 사용한 3x3 window를 출력하는 것을 그대로 사용하였더니 2x2 window에 맞지 않아 문제가 발생
- **해결**: Convolution연산과 Max pooling에서 각기 다른 line buffer를 사용하도록 코드 수정 후 정상 동작 확인

---

## 4. 결과 및 진척도
- 완료한 목표: 
  1. Max pooling 결합시 발생한 문제 해결
  2. Convolution layer (Single) 코드 작성 완료
- 진행 중: 
- 보류: 

---

## 5. 다음 할 일
- [ ] n channel Convolution layer 코드 작성
- [ ] Fully Connected layer 설계

---

## 6. 메모 / 떠오른 생각
<!-- 의문점, 아이디어, 다음에 시도해볼 것 -->