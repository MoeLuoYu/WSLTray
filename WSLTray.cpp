#include <windows.h>
#include <tchar.h>

// 启动WSL并使其后台常驻
bool LaunchWSLBackground() {
	// 启动WSL的bash shell（保持WSL实例运行）
	// WSL默认启动后会保持运行，直到手动执行wsl --shutdown
	TCHAR cmdLine[] = _T("wsl.exe --exec bash");
	
	// 配置进程启动信息（隐藏窗口）
	STARTUPINFO si = {0};
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;  // 隐藏WSL窗口
	
	// 进程信息
	PROCESS_INFORMATION pi = {0};
	
	// 启动WSL进程（不创建窗口）
	bool success = CreateProcess(
		NULL,               // 自动查找wsl.exe（系统PATH中）
		cmdLine,            // 启动bash（保持WSL运行）
		NULL,               // 默认进程安全属性
		NULL,               // 默认线程安全属性
		FALSE,              // 不继承句柄
		CREATE_NO_WINDOW,   // 禁止显示控制台窗口
		NULL,               // 继承当前环境变量
		NULL,               // 使用当前工作目录
		&si,
		&pi
		);
	
	if (success) {
		// 启动完成，关闭句柄
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return true;
	} else {
		return false;
	}
}

// 无控制台窗口的程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// 启动WSL后立即退出程序，WSL在后台常驻
	LaunchWSLBackground();
	return 0;
}
