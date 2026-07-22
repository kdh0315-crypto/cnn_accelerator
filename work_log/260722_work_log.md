# 개발 일지 — 2026-07-22

> 프로젝트명: `CNN 가속기 설계 프로젝트`  
> 작성자: `김동현`  
> 태그: `#CNN` `#C`

---

## 1. 오늘의 목표
<!-- 작업 시작 전, 오늘 하려던 것을 적습니다 -->
- [ ] n channel Convolution layer 코드 작성
- [ ] Fully Connected layer 설계

---

## 2. 수행 내용
<!-- 실제로 한 작업과 '왜 그렇게 했는지'를 함께 적습니다 -->

### 2.1 작업 내용

*기존 conv3x3 -> convFilter로 이름 변경*
- 이름으로 인해 추후 확장 시에 이름이 명확하지 않아 생기는 문제 방지하고자 수행
- 이와 관련한 다른 함수 및 변수(ex. lineBuf_Conv_t -> lineBuf_Filter_t)도 이름 유사하게 변경

*출력 Valid 신호가 입력 Valid 신호와 함께 출력되도록 변경*
- Max pooling의 코드가 Stride 2로 변경됨에 따라 Valid 신호가 한번 출력되면 전 layer의 출력이 항상 유효한 것이 아니게 됨
- 이에 따라 입력 valid 신호를 관찰하여 이에 맞추어 출력되도록 변경
```c
ctx->lb_valid = ctx->lb_start && (ctx->row_cnt >= MAX_POOL_KERNEL_SIZE-1) && (ctx->col_cnt >= MAX_POOL_KERNEL_SIZE-1);
```

### 2.2 자료
<!-- 이미지는 같은 폴더의 images/ 안에 두고 아래처럼 링크합니다 -->


---


## 3. 문제 및 디버깅
<!-- 포트폴리오에서 가장 중요한 부분. 사고의 흐름을 남깁니다 -->


---

## 4. 결과 및 진척도
- 완료한 목표: 
- 진행 중: 
- 보류: 

---

## 5. 다음 할 일
- [ ] n channel Convolution layer 코드 작성
- [ ] Fully Connected layer 설계

---

## 6. 메모 / 떠오른 생각
<!-- 의문점, 아이디어, 다음에 시도해볼 것 -->