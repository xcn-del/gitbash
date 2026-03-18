# Simple Notepad Application

간단한 GUI 메모장 애플리케이션입니다.

## 기능

- 파일 열기/저장
- 텍스트 편집 (실행 취소, 다시 실행)
- 잘라내기, 복사, 붙여넣기
- 한글 지원

## 설치 및 빌드

### 사전 요구사항

- Python 3.6 이상
- pip (Python 패키지 관리자)

### Python 스크립트로 실행

```bash
python notepad.py
```

### Windows EXE 파일로 빌드

#### 1. 자동 빌드 (권장)

```bash
build_exe.bat
```

더블클릭하면 자동으로 빌드됩니다.

#### 2. 수동 빌드

```bash
# PyInstaller 설치
pip install -r requirements.txt

# exe 빌드
pyinstaller --onefile --windowed --name=Notepad notepad.py
```

### 빌드 결과

생성된 exe 파일은 `dist/Notepad.exe` 위치에 있습니다.

## 메뉴

### 파일
- 새 파일: 새로운 문서 작성
- 열기: 기존 파일 열기
- 저장: 현재 파일 저장
- 다른 이름으로 저장: 새로운 이름으로 저장
- 종료: 프로그램 종료

### 편집
- 실행 취소: 마지막 작업 취소
- 다시 실행: 취소한 작업 다시 실행
- 잘라내기: 텍스트 잘라내기
- 복사: 텍스트 복사
- 붙여넣기: 텍스트 붙여넣기

### 도움말
- 정보: 프로그램 정보

## 라이선스

MIT License
