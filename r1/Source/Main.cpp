#include "Game/LeikaShooting.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//_CrtSetBreakAlloc(218366);

	/**
	 *	@fn		CoInitializeEx
	 *	@brief	COMライブラリの初期化
	 *	@note	https://msdn.microsoft.com/ja-jp/library/windows/desktop/ff485844(v=vs.85).aspx
	 */
	if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return 1;

	LeikaShooting game;
	const int Msg = static_cast<int>(game.Run().wParam);
	/**
	 *	@fn		CoUninitialize
	 *	@brief	COMライブラリの終了処理
	 *	@note	https://msdn.microsoft.com/ja-jp/library/windows/desktop/ff485844(v=vs.85).aspx
	 */
	CoUninitialize();

	return Msg;
}