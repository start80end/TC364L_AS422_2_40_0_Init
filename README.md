# 🚀 AURIX TC364L MCAL Starter Project (AS422 V2.40.0)

이 프로젝트는 Infineon **AURIX TC364L** 마이크로컨트롤러 환경에서 **AUTOSAR MCAL(AS422 V2.40.0)**의 기초적인 사용법과 초기화 과정을 학습하기 위한 베이스 예제입니다. 
주로 **GTM(Generic Timer Module)**과 **STM(System Timer)**을 활용한 주기적 작업 처리 및 인터럽트 구동 방식에 초점을 맞추고 있습니다.

---

## � 프로젝트 기본 명세 (Specifications)
- **MCU**: Infineon AURIX **TC364L** (SAK-TC364DP-64F300W)
- **Board**: AURIX TC3X4 TriBoard V1.2 (또는 호환 개발 보드)
- **MCAL**: MC-ISAR AS422 V2.40.0
- **Configuration Tool**: **EB Tresos 26.2.0**
- **Compiler**: TASKING TriCore v6.3r1
- **Debugger**: TRACE32 / Lauterbach

---

## 🛠️ 개발 환경 및 설정 (Development Setup)

### 1. 주요 MCAL 모듈 (Configured Modules)
EB Tresos를 통해 다음과 같은 필수 모듈이 설정되어 있습니다.
- **MCU**: 시스템 클럭(fSYS=100MHz), PLL 설정 및 클럭 배분
- **PORT & DIO**: LED1(P33.4), LED2(P33.5) 설정 및 제어
- **GPT & GTM**: 1ms 단위의 시스템 틱 생성을 위한 GTM TOM 설정
- **IRQ / MCL**: 타이머 인터럽트(STM, GTM) 서비스 요청 및 엔진 관리

### 2. 클럭 구성 (Clock Configuration)
- **External Crystal**: 20 MHz
- **Target System Frequency (fSYS)**: 100 MHz
- **Timer Source (fGTM / fSTM)**: 100 MHz (기본 분주율 적용)

---

## 🚀 구현된 주요 기능 (Main Functionality)

이 프로젝트는 두 가지 타이머 소스를 선택하여 테스트할 수 있도록 설계되었습니다. (`App_Cfg.h`의 `CURRENT_TEST_MODE` 설정 참조)

### **Mode A: GTM 기반 협동형 스케줄러 (Task Scheduler)**
- **구동 원리**: GTM TOM0_CH0의 1ms 주기 Notification을 기반으로 `TaskScheduler.c` 프레임워크가 구동됩니다.
- **Task 구성**:
  - `100ms Task`: LED1 (P33.4) Toggle
  - `1000ms Task`: LED2 (P33.5) Toggle (기본값)
- **특징**: 단순한 루프 내에서의 우선순위 기반 주기적 호출 구조를 가집니다.

### **Mode B: STM 기반 주기적 인터럽트 (Standalone ISR)**
- **구동 원리**: 시스템 타이머 STM0의 하드웨어 Comparator(CMP0, CMP1)를 직접 사용하여 인터럽트를 발생시킵니다.
- **Interrupts**:
  - **SR0 (Priority 10)**: 100ms 마다 LED1 (P33.4) Toggle
  - **SR1 (Priority 11)**: 500ms 마다 LED2 (P33.5) Toggle
- **특징**: 운영체제나 소프트웨어 스케줄러 없이 하드웨어 타이머에 의해 정확한 주기가 보장됩니다.

---

## 📂 폴더 구조 (Project Structure)

| 폴더/파일 | 설명 |
| :--- | :--- |
| `App/Main/` | `Cpu0_Main.c` (진입점), `TaskScheduler.c` (GTM 기반 스케줄러) |
| `App/Tasks/` | `StmTask.c` (STM 타이머 로직), `LedToggleTask.c` (LED 제어 함수) |
| `Config/` | EB Tresos에서 자동 생성된 MCAL 코드 및 설정 파일 |
| `Drivers/` | MCAL 라이브러리 및 하위 시스템 드라이버 |
| `Scripts/` | `debug_auto.cmm`, `test_stm.cmm` 등 TRACE32용 자동화 스크립트 |

---

## 🔍 시작하기 및 테스트 (Usage)

1. **프로젝트 임포트**: Tasking IDE에서 해당 프로젝트 폴더를 활성화합니다.
2. **모드 설정**: `App/Main/App_Cfg.h` 파일 내의 `#define CURRENT_TEST_MODE` 값을 원하는 테스트 모드로 변경합니다.
3. **빌드**: Tasking Compiler를 이용하여 컴파일을 완료합니다.
4. **다운로드**: TRACE32를 실행하고 생성된 `.elf` 파일을 플래싱합니다.
5. **실행**: 보드의 LED1, LED2가 설정된 주기(GTM/STM에 따라 다름)에 맞춰 정상적으로 깜빡이는지 확인합니다.
