#include "IMGUI_GUI.h"
#include "IMGUI/imgui.h"
#include "IMGUI/backends/imgui_impl_win32.h"
#include "IMGUI/backends/imgui_impl_dx12.h"

#include "DX12_DescriptorHeapManager.h"
#include "DX12_CommandList.h"
#include "DX12_CommandListPool.h"

void CIMGUI_GUI::Initialize(HWND InHWnd, Delight::Comptr<ID3D12Device> InDevice, Delight::Comptr<ID3D12CommandQueue> InCommandQueue, int32 NumFrame)
{
	if (!InDevice.IsValid() || !InCommandQueue.IsValid())
	{
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // ui가 독립된 창으로 처리

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	myHWnd = InHWnd;
	ImGui_ImplWin32_Init(myHWnd);

	ImGui_ImplDX12_InitInfo init_info = {};
	init_info.Device = InDevice.GetData();
	init_info.CommandQueue = InCommandQueue.GetData();
	init_info.NumFramesInFlight = NumFrame;
	init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM; // backbuffer format?

	extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
	FDescriptorHeap* Heap = GDescriptorHeapManager.GetHeap(DHT_ShaderResource);
	
	if (Heap == nullptr)
	{
		return;
	}

	init_info.SrvDescriptorHeap = Heap->Heap.GetData();
	init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) {
		extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
		FDescriptorHandleSet HandleSet = GDescriptorHeapManager.GetHandle(DHT_ShaderResource);
		*out_cpu_handle = HandleSet.CPUHandle;
		*out_gpu_handle = HandleSet.GPUHandle; };
	init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) {
		extern CDX12_DescriptorHeapManager GDescriptorHeapManager;
		FDescriptorHandleSet HandleSet(cpu_handle, gpu_handle);
		GDescriptorHeapManager.ReturnToQueue(DHT_ShaderResource, HandleSet); };

	ImGui_ImplDX12_Init(&init_info);
	
	Device = InDevice;
	CommandQueue = InCommandQueue;
	bInitialized = true;
}

void CIMGUI_GUI::Release()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	bInitialized = false;
}

void CIMGUI_GUI::Render(CDX12_CommandList& CommandList)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 랜더링 하고자 하는 윈도우들 세팅
	BuildFramerateWindow();

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CommandList.Get().GetData());

	// ImGuiConfigFlags_ViewportsEnable 일때만
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

LRESULT CIMGUI_GUI::WinProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (bInitialized)
	{
		extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

		auto& IO = ImGui::GetIO();

		return IO.WantCaptureKeyboard | IO.WantCaptureMouse;
	}

	return false;
}

void CIMGUI_GUI::BuildFramerateWindow()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
