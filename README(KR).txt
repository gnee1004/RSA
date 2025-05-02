```
# RSA 암호화/복호화 프로젝트

## 개요
이 프로젝트는 OpenSSL 라이브러리를 사용하여 C 언어로 구현된 간단한 RSA 암호화 및 복호화 도구입니다. 최대 5000자까지의 메시지를 245바이트 단위로 나누어 암호화하고 복호화할 수 있습니다.

- `encrypt.c`: 사용자가 입력한 메시지를 RSA로 암호화하여 사용자 지정 디렉토리에 암호화된 데이터, 공개 키, 개인 키를 저장합니다. 암호화된 데이터는 16진수 형식으로도 표시됩니다.
- `decrypt.c`: 개인 키를 사용하여 암호화된 데이터를 복호화하고 원래 메시지를 출력합니다.

## 기능
- 최대 5000자까지의 메시지를 245바이트 단위로 나누어 처리.
- 파일(`public.pem`, `private.pem`, `encrypted.bin`)을 사용자가 실행 시 지정한 디렉토리에 동적으로 저장.
- 비-ASCII 문자(예: 한글)를 UTF-8 인코딩으로 지원.
- 디버깅을 위해 암호화된 데이터를 16진수 형식으로 출력.

## 요구 사항
- **OpenSSL 3.0 이상**: RSA 암호화 및 복호화에 필요.
- **Windows 환경**: 코드가 Windows 전용 함수를 사용하지 않도록 수정되었으나, 콘솔 설정은 Windows 기반.
- **C 컴파일러**: Visual Studio 또는 OpenSSL을 지원하는 C 컴파일러(예: MinGW).

## 설정

### 1. OpenSSL 설치
- Windows용 OpenSSL을 다운로드하여 설치하세요(예: [Win64 OpenSSL](https://slproweb.com/products/Win32OpenSSL.html)).
- `C:\OpenSSL-Win64`에 설치(경로 변경 시 설정 수정 필요).
- Visual Studio 사용 시:
  - **포함 디렉토리**: `C:\OpenSSL-Win64\include` 추가 (프로젝트 → 속성 → C/C++ → 일반 → 추가 포함 디렉토리).
  - **라이브러리 디렉토리**: `C:\OpenSSL-Win64\lib` 추가 (프로젝트 → 속성 → 링커 → 일반 → 추가 라이브러리 디렉토리).
  - **추가 종속성**: `libcrypto.lib` 추가 (프로젝트 → 속성 → 링커 → 입력 → 추가 종속성).
  - `libcrypto-*.dll`(예: `libcrypto-3-x64.dll`)을 실행 파일 디렉토리(예: `C:\Path\To\Your\Project\x64\Debug`)에 복사.
- 플랫폼을 `x64`로 설정.

### 2. 코드 컴파일
- 이 저장소를 클론하거나 다운로드.
- `encrypt.c`와 `decrypt.c`를 각각 컴파일:
  - Visual Studio 사용 시: 두 파일을 프로젝트에 추가하고 빌드.
  - 명령줄 컴파일러(예: MinGW) 사용 시:
    ```
    gcc -o encrypt.exe encrypt.c -I"C:\OpenSSL-Win64\include" -L"C:\OpenSSL-Win64\lib" -lcrypto
    gcc -o decrypt.exe decrypt.c -I"C:\OpenSSL-Win64\include" -L"C:\OpenSSL-Win64\lib" -lcrypto
    ```
- 프로젝트 디렉토리에 `encrypt.exe`와 `decrypt.exe`가 생성되었는지 확인.

## 사용법

### 1. `encrypt.exe` 실행
- 명령 프롬프트(CMD)에서 `encrypt.exe` 실행:
  ```
  encrypt.exe
  ```
- 파일(`public.pem`, `private.pem`, `encrypted.bin`)을 저장할 디렉토리를 입력하라는 메시지가 표시됨:
  ```
  Enter the directory to save files (e.g., C:\Path\To\Directory):
  ```
  예시 입력: `C:\Users\YourUsername\Documents\RSAFiles`
- 암호화할 메시지(최대 5000자)를 입력:
  ```
  Enter the message to encrypt (max 4999 characters):
  ```
  예시 입력: `안녕하세요`
- 프로그램이 다음 작업을 수행:
  - 공개 키와 개인 키(`public.pem`, `private.pem`)를 생성 및 저장.
  - 메시지를 암호화하여 `encrypted.bin`에 저장.
  - 암호화된 데이터를 16진수 형식으로 출력.

**예시 출력**:
```
Enter the directory to save files (e.g., C:\Path\To\Directory):
C:\Users\YourUsername\Documents\RSAFiles
Enter the message to encrypt (max 4999 characters):
안녕하세요
Saving public key to: C:\Users\YourUsername\Documents\RSAFiles\public.pem
Saving private key to: C:\Users\YourUsername\Documents\RSAFiles\private.pem
Saving encrypted data to: C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
Total number of chunks: 1
Chunk 1 (length: 256):
3A 5F 8C 2D ... (256바이트 16진수 데이터)
Encrypted data with header saved to C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
```

### 2. `decrypt.exe` 실행
- 명령 프롬프트(CMD)에서 `decrypt.exe` 실행:
  ```
  decrypt.exe
  ```
- 암호화 시 지정한 디렉토리를 입력:
  ```
  Enter the directory where files are located (e.g., C:\Path\To\Directory):
  ```
  예시 입력: `C:\Users\YourUsername\Documents\RSAFiles`
- 프로그램이 다음 작업을 수행:
  - 개인 키(`private.pem`)와 암호화된 데이터(`encrypted.bin`)를 읽음.
  - 데이터를 복호화하여 원래 메시지를 출력.

**예시 출력**:
```
Enter the directory where files are located (e.g., C:\Path\To\Directory):
C:\Users\YourUsername\Documents\RSAFiles
Attempting to read private key from: C:\Users\YourUsername\Documents\RSAFiles\private.pem
Attempting to read encrypted data from: C:\Users\YourUsername\Documents\RSAFiles\encrypted.bin
Total number of chunks: 1
Decrypted message: 안녕하세요
```

## 주의사항

### 디렉토리 지정
- `encrypt.exe`와 `decrypt.exe`는 파일을 저장하거나 읽을 디렉토리를 사용자에게 입력받습니다. 다음 사항을 확인하세요:
  - 두 프로그램에서 동일한 디렉토리를 사용.
  - 지정한 디렉토리에 읽기/쓰기 권한이 있는지 확인.
  - 경로는 백슬래시를 사용(예: `C:\Path\To\Directory`), Windows 환경에서 실행.

### 한글 지원
- 이 프로그램은 한글과 같은 비-ASCII 문자를 UTF-8 인코딩으로 지원합니다.
- CMD에서 복호화된 메시지가 한글일 경우 깨질 수 있음(예: `????`):
  - CMD에서 프로그램 실행 전에 다음 명령 실행:
    ```
    chcp 65001
    ```
  - 또는 CMD 설정 변경:
    - CMD 창을 마우스 오른쪽 버튼으로 클릭 → "기본값" → "옵션" 탭 → "현재 코드 페이지"를 "UTF-8(65001)"로 설정.
  - 문제가 지속되면 출력을 파일로 리디렉션:
    ```
    decrypt.exe > output.txt
    ```

### 문제 해결
- **"Failed to open private.pem" 또는 "Failed to open encrypted.bin" 오류**:
  - `decrypt.exe`에서 입력한 디렉토리가 `encrypt.exe`에서 사용한 디렉토리와 동일한지 확인.
  - 지정한 디렉토리에 `private.pem`과 `encrypted.bin` 파일이 있는지 확인.
  - 디렉토리에 읽기/쓰기 권한이 있는지 확인.
- **OpenSSL 오류**:
  - OpenSSL이 올바르게 설치되고 링크되었는지 확인.
  - `libcrypto-*.dll`이 실행 파일 디렉토리 또는 시스템 PATH에 있는지 확인.

## 라이선스
이 프로젝트는 MIT 라이선스에 따라 배포됩니다. 자세한 내용은 `LICENSE` 파일을 참조하세요.

## 기여
기여는 언제나 환영입니다! 제안, 버그 리포트, 개선 사항이 있다면 풀 리퀘스트를 제출하거나 이슈를 열어 주세요.
```

---

## 📌 변경 사항 요약
- **한국어 번역**:
  - 영어 `README.md`를 한국어로 번역하여 한국어 사용자도 쉽게 이해할 수 있도록 작성.
- **사용법 명확화**:
  - `encrypt.exe`와 `decrypt.exe` 실행 과정을 단계별로 설명하고 한글 예시 출력 추가.
- **환경 설정**:
  - OpenSSL 설치 및 Visual Studio 설정 방법을 한국어로 자세히 기술.
- **한글 깨짐 문제**:
  - CMD에서 한글 깨짐 문제 해결 방법(`chcp 65001`, CMD 설정 변경) 추가.
- **문제 해결**:
  - 흔한 오류와 해결 방법 포함.
- **라이선스 및 기여**:
  - MIT 라이선스와 기여 방법 안내 추가.
