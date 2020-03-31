# DelightEngine
공부용. RHI 스왑가능 DX12기반, 

RHI, InputDevice등 스왑가능하게..
그래픽스 넣어보고싶은것들 넣어보게.

일할떄 프로토용으로 사용할 클라이언트제작

툴은 ...?생각중

Trello : https://trello.com/b/zPtM1BwQ/delightengine

# External Librarys
1. eastl(https://github.com/electronicarts/EASTL)
STL 대체용.. std STL을 사용하면 DLL내부에서 사용이 불가능하기 때문에..

2. DirectXLibs(https://github.com/microsoft/DirectX-Graphics-Samples/tree/master/Libraries)
외부는 아니지만 External처럼 쓰므로..

3. imgui(https://github.com/ocornut/imgui)
플로팅 ui용 라이브러리.

4. Thrading Building Blocks(https://github.com/oneapi-src/oneTBB)
병렬 처리(parallel_for..), 메모리 할당(scalable_malloc, realloc, etc) 라이브러리