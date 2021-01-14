#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

#include <string>
#include <vector>
#include <iostream>
using namespace std;
#include <io.h>

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Version.lib")

#define _FPS_ 30

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define ACTION_INIT 0
#define ACTION_QUIT 1
#define ACTION_JUMP 2

#define SCENE_EDITOR	-1
#define SCENE_MAINMENU 0
#define SCENE_NEWMAP	1
#define SCENE_OPENMAP	2
#define SCENE_ABOUT		3

#define MENUITEMLENGTH 4

int CurrentScene = SCENE_MAINMENU;

bool bIsRunning = true, bIsMainMenuBgImgMoveDown = true, bIsMainMenuBgImgMoveRight = true;

SDL_Window* pWindow = nullptr;
SDL_Renderer* pWRenderer = nullptr;

TTF_Font* pFont_XINWEI_125 = nullptr;
TTF_Font* pFont_XINWEI_70 = nullptr;
TTF_Font* pFont_XINWEI_40 = nullptr;
TTF_Font* pFont_XINWEI_30 = nullptr;
TTF_Font* pFont_XINWEI_25 = nullptr;

SDL_Surface* pSurfaceBackground = nullptr;
SDL_Surface* pSurfaceMainMenuTitle_Another = nullptr;
SDL_Surface* pSurfaceMainMenuTitle_World = nullptr;
SDL_Surface* pSurfaceMainMenuTitle_MapEditor = nullptr;
SDL_Surface* pSurfaceMainMenuTip = nullptr;
SDL_Surface* pSurfaceMainMenuAuthor = nullptr;
SDL_Surface* aryPSurfaceMainMenuItems[MENUITEMLENGTH] = { nullptr, nullptr, nullptr, nullptr };
SDL_Surface* pSurfaceOpenMapScene_Title = nullptr;
SDL_Surface* aryPSurfaceOpenMapTips[2] = { nullptr, nullptr };

SDL_Texture* pTextureBackground = nullptr;
SDL_Texture* pTextureMainMenuTitle_Another = nullptr;
SDL_Texture* pTextureMainMenuTitle_World = nullptr;
SDL_Texture* pTextureMainMenuTitle_MapEditor = nullptr;
SDL_Texture* pTextureMainMenuTip = nullptr;
SDL_Texture* pTextureMainMenuAuthor = nullptr;
SDL_Texture* aryPTextureMainMenuItems[MENUITEMLENGTH] = { nullptr, nullptr, nullptr, nullptr };
SDL_Texture* pTextureOpenMapScene_Title = nullptr;
SDL_Texture* aryPTextureOpenMapTips[2] = { nullptr, nullptr };

int iMainMenuItemIndex = 0, iPreMenuItemIndex = MENUITEMLENGTH - 1, iMapListIndex = 0, iMapShowingListFirstIndex = 0;

string aryStrMainMenuItem[MENUITEMLENGTH] = { "新 建 地 图", "打 开 地 图", "关 于", "退 出" };

SDL_Color clrText_MenuItemIdle = { 211, 203, 198, 255 }, clrText_MenuItemHover = { 254, 242, 99, 255 };

SDL_Rect rcMainMenuBgImgClip = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

vector<string> vMapList, vShowingMapList;

void ManageScene(int action, int scene)
{
	if (action == ACTION_JUMP || action == ACTION_QUIT)
	{
		/* 处理当前场景卸载 */
		if (CurrentScene == SCENE_EDITOR)
		{

		}
		else if (CurrentScene == SCENE_MAINMENU)
		{
			SDL_DestroyTexture(pTextureBackground);
			SDL_DestroyTexture(pTextureMainMenuTitle_Another);
			SDL_DestroyTexture(pTextureMainMenuTitle_World);
			SDL_DestroyTexture(pTextureMainMenuTitle_MapEditor);
			SDL_DestroyTexture(pTextureMainMenuTip);
			SDL_DestroyTexture(pTextureMainMenuAuthor);
			for (auto &texture : aryPTextureMainMenuItems)
				SDL_DestroyTexture(texture);

			SDL_FreeSurface(pSurfaceBackground);
			SDL_FreeSurface(pSurfaceMainMenuTitle_Another);
			SDL_FreeSurface(pSurfaceMainMenuTitle_World);
			SDL_FreeSurface(pSurfaceMainMenuTitle_MapEditor);
			SDL_FreeSurface(pSurfaceMainMenuTip);
			SDL_FreeSurface(pSurfaceMainMenuAuthor);
			for (auto &surface : aryPSurfaceMainMenuItems)
				SDL_FreeSurface(surface);

			TTF_CloseFont(pFont_XINWEI_125);
			TTF_CloseFont(pFont_XINWEI_70);
			TTF_CloseFont(pFont_XINWEI_40);
			TTF_CloseFont(pFont_XINWEI_25);
		}
		else if (CurrentScene == SCENE_NEWMAP)
		{

		}
		else if (CurrentScene == SCENE_OPENMAP)
		{
			SDL_DestroyTexture(pTextureOpenMapScene_Title);
			for (auto& pTexture : aryPTextureOpenMapTips)
				SDL_DestroyTexture(pTexture);

			SDL_FreeSurface(pSurfaceOpenMapScene_Title);
			for (auto& pSurface : aryPSurfaceOpenMapTips)
				SDL_FreeSurface(pSurface);

			TTF_CloseFont(pFont_XINWEI_40);
			TTF_CloseFont(pFont_XINWEI_30);
			TTF_CloseFont(pFont_XINWEI_25);

			iMapListIndex = 0, iMapShowingListFirstIndex = 0;

			vMapList.clear();
			vShowingMapList.clear();
		}
		else if (CurrentScene == SCENE_ABOUT)
		{

		}

		if (action == ACTION_QUIT)
			bIsRunning = false;
	}
	
	if (action == ACTION_JUMP)
		/* 切换当前场景未指定场景 */
		CurrentScene = scene;

	if (action == ACTION_JUMP || action == ACTION_INIT)
	{
		/* 处理当前场景初始化 */
		if (CurrentScene == SCENE_EDITOR)
		{

		}
		else if (CurrentScene == SCENE_MAINMENU)
		{
			pSurfaceBackground = IMG_Load("resource/image/background.jpg");
			pTextureBackground = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceBackground);

			pFont_XINWEI_125 = TTF_OpenFont("resource/font/STXINWEI.TTF", 125);
			pFont_XINWEI_70 = TTF_OpenFont("resource/font/STXINWEI.TTF", 70);
			pFont_XINWEI_40 = TTF_OpenFont("resource/font/STXINWEI.TTF", 40);
			pFont_XINWEI_25 = TTF_OpenFont("resource/font/STXINWEI.TTF", 25);

			SDL_Color clrText_Another = { 83, 131, 195, 255 }, clrText_World = { 157, 91, 139, 255 }, clrText_MapEditor = { 191, 120, 58, 255 }, clrText_Tip = { 226, 4, 27, 255 }, clrText_Author = { 173, 173, 173, 255 };

			pSurfaceMainMenuTitle_Another = TTF_RenderUTF8_Blended(pFont_XINWEI_125, "Another", clrText_Another);
			pSurfaceMainMenuTitle_World = TTF_RenderUTF8_Blended(pFont_XINWEI_125, "World", clrText_World);
			pSurfaceMainMenuTitle_MapEditor = TTF_RenderUTF8_Blended(pFont_XINWEI_70, "地图编辑器", clrText_MapEditor);
			pSurfaceMainMenuTip = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "按下 ↑ ↓ ← → 移动光标，按下 Enter 确认", clrText_Tip);
			pSurfaceMainMenuAuthor = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "Powered By Voidmatrix", clrText_Author);
			aryPSurfaceMainMenuItems[0] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, aryStrMainMenuItem[0].c_str(), clrText_MenuItemIdle);
			aryPSurfaceMainMenuItems[1] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, aryStrMainMenuItem[1].c_str(), clrText_MenuItemIdle);
			aryPSurfaceMainMenuItems[2] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, aryStrMainMenuItem[2].c_str(), clrText_MenuItemIdle);
			aryPSurfaceMainMenuItems[3] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, aryStrMainMenuItem[3].c_str(), clrText_MenuItemIdle);

			pTextureMainMenuTitle_Another = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceMainMenuTitle_Another);
			pTextureMainMenuTitle_World = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceMainMenuTitle_World);
			pTextureMainMenuTitle_MapEditor = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceMainMenuTitle_MapEditor);
			pTextureMainMenuTip = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceMainMenuTip);
			pTextureMainMenuAuthor = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceMainMenuAuthor);
			aryPTextureMainMenuItems[0] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceMainMenuItems[0]);
			aryPTextureMainMenuItems[1] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceMainMenuItems[1]);
			aryPTextureMainMenuItems[2] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceMainMenuItems[2]);
			aryPTextureMainMenuItems[3] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceMainMenuItems[3]);
		}
		else if (CurrentScene == SCENE_NEWMAP)
		{

		}
		else if (CurrentScene == SCENE_OPENMAP)
		{
			pFont_XINWEI_40 = TTF_OpenFont("resource/font/STXINWEI.TTF", 40);
			pFont_XINWEI_30 = TTF_OpenFont("resource/font/STXINWEI.TTF", 30);
			pFont_XINWEI_25 = TTF_OpenFont("resource/font/STXINWEI.TTF", 25);

			SDL_Color clrText_Title_Front = { 248, 244, 230, 255 }, clrText_Title_Back = { 75, 75, 75, 255 }, clrText_Tip = { 226, 4, 27, 255 };

			pSurfaceOpenMapScene_Title = TTF_RenderUTF8_Shaded(pFont_XINWEI_40, " 打开地图 ", clrText_Title_Front, clrText_Title_Back);
			aryPSurfaceOpenMapTips[0] = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "按下 ↑ ↓ ← → 移动光标，按下 Enter 打开，按下 Esc 退回到主菜单", clrText_Tip);
			aryPSurfaceOpenMapTips[1] = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "将地图文件放置于“maps”文件夹下，并确保文件名中无中文及其他特殊符号", clrText_Tip);

			pTextureOpenMapScene_Title = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceOpenMapScene_Title);
			aryPTextureOpenMapTips[0] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceOpenMapTips[0]);
			aryPTextureOpenMapTips[1] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceOpenMapTips[1]);

			intptr_t hFile = 0;
			_finddata_t fileinfo;

			if ((hFile = _findfirst("maps/*.map", &fileinfo)) != -1)
			{
				int _index = 0;
				do
				{
					if (!(fileinfo.attrib & _A_SUBDIR))
					{
						vMapList.push_back(fileinfo.name);
						if (_index < 8)
							vShowingMapList.push_back(fileinfo.name);
					}
					_index++;
				} while (!_findnext(hFile, &fileinfo));
				_findclose(hFile);
			}
		}
		else if (CurrentScene == SCENE_ABOUT)
		{

		}
	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	pWindow = SDL_CreateWindow("Another World 地图编辑器 v1.0.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	pWRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

	ManageScene(ACTION_INIT, 0);

	while (bIsRunning)
	{
		unsigned int _start = SDL_GetTicks();

		SDL_SetRenderDrawColor(pWRenderer, 0, 0, 0, 255);
		SDL_RenderClear(pWRenderer);

		SDL_Event _event;
		if (SDL_PollEvent(&_event))
		{
			if (_event.type == SDL_QUIT)
				bIsRunning = false;

			/* 处理不同场景下的事件 */
			if (CurrentScene == SCENE_EDITOR)
			{

			}
			else if (CurrentScene == SCENE_MAINMENU)
			{
				if (_event.type == SDL_KEYDOWN)
				{
					switch (_event.key.keysym.sym)
					{
					case SDLK_DOWN:
					case SDLK_RIGHT:
						iPreMenuItemIndex = iMainMenuItemIndex;
						if (iMainMenuItemIndex < MENUITEMLENGTH - 1)
							iMainMenuItemIndex++;
						else
							iMainMenuItemIndex = 0;
						break;
					case SDLK_UP:
					case SDLK_LEFT:
						iPreMenuItemIndex = iMainMenuItemIndex;
						if (iMainMenuItemIndex > 0)
							iMainMenuItemIndex--;
						else
							iMainMenuItemIndex = MENUITEMLENGTH - 1;
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						switch (iMainMenuItemIndex)
						{
						case 0:
							ManageScene(ACTION_JUMP, SCENE_NEWMAP);
							break;
						case 1:
							ManageScene(ACTION_JUMP, SCENE_OPENMAP);
							break;
						case 2:
							ManageScene(ACTION_JUMP, SCENE_ABOUT);
							break;
						case 3:
							ManageScene(ACTION_QUIT, 0);
							break;
						default:
							break;
						}
						break;
					default:
						break;
					}
				}

			}
			else if (CurrentScene == SCENE_NEWMAP)
			{

			}
			else if (CurrentScene == SCENE_OPENMAP)
			{
				if (_event.type == SDL_KEYDOWN)
				{
					switch (_event.key.keysym.sym)
					{
					case SDLK_DOWN:
					case SDLK_RIGHT:
						if (iMapListIndex < vMapList.size() - 1)
							iMapListIndex++;
						if (iMapListIndex > iMapShowingListFirstIndex + 7)
							iMapShowingListFirstIndex++;
						break;
					case SDLK_UP:
					case SDLK_LEFT:
						if (iMapListIndex > 0)
							iMapListIndex--;
						if (iMapListIndex < iMapShowingListFirstIndex)
							iMapShowingListFirstIndex--;
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						// 处理打开地图操作：运行对应的lua文件，失败则弹窗报错，成功则跳转场景
						/*switch (iMainMenuItemIndex)
						{
						case 0:
							ManageScene(ACTION_JUMP, SCENE_NEWMAP);
							break;
						case 1:
							ManageScene(ACTION_JUMP, SCENE_OPENMAP);
							break;
						case 2:
							ManageScene(ACTION_JUMP, SCENE_ABOUT);
							break;
						case 3:
							ManageScene(ACTION_QUIT, 0);
							break;
						default:
							break;
						}*/
						break;
					case SDLK_ESCAPE:
						ManageScene(ACTION_JUMP, SCENE_MAINMENU);
						break;
					default:
						break;
					}
				}
			}
			else if (CurrentScene == SCENE_ABOUT)
			{

			}
		}

		if (bIsRunning)
		{
			/* 处理不同场景下的渲染 */
			if (CurrentScene == SCENE_EDITOR)
			{

			}
			else if (CurrentScene == SCENE_MAINMENU)
			{
				SDL_Rect rcText_Another = { 250, 100, pSurfaceMainMenuTitle_Another->w, pSurfaceMainMenuTitle_Another->h },
					rcText_World = { rcText_Another.x + 500, rcText_Another.y + 75, pSurfaceMainMenuTitle_World->w, pSurfaceMainMenuTitle_World->h }, 
					rcText_MapEditor = { 325, rcText_World.y + 75, pSurfaceMainMenuTitle_MapEditor->w, pSurfaceMainMenuTitle_MapEditor->h }, 
					rcText_Tip = { 10, WINDOW_HEIGHT - pSurfaceMainMenuTip->h - 10, pSurfaceMainMenuTip->w, pSurfaceMainMenuTip->h }, 
					rcText_Author = { WINDOW_WIDTH - pSurfaceMainMenuAuthor->w - 10, WINDOW_HEIGHT - pSurfaceMainMenuAuthor->h - 10, pSurfaceMainMenuAuthor->w, pSurfaceMainMenuAuthor->h },
					aryRCTextMenuItems[MENUITEMLENGTH] = {
						{WINDOW_WIDTH / 2 - aryPSurfaceMainMenuItems[0]->w / 2, 380 + (aryPSurfaceMainMenuItems[0]->h + 10) * 0,	aryPSurfaceMainMenuItems[0]->w, aryPSurfaceMainMenuItems[0]->h },
						{WINDOW_WIDTH / 2 - aryPSurfaceMainMenuItems[1]->w / 2, 380 + (aryPSurfaceMainMenuItems[0]->h + 15) * 1,	aryPSurfaceMainMenuItems[1]->w, aryPSurfaceMainMenuItems[1]->h },
						{WINDOW_WIDTH / 2 - aryPSurfaceMainMenuItems[2]->w / 2, 380 + (aryPSurfaceMainMenuItems[0]->h + 15) * 2,	aryPSurfaceMainMenuItems[2]->w, aryPSurfaceMainMenuItems[2]->h },
						{WINDOW_WIDTH / 2 - aryPSurfaceMainMenuItems[3]->w / 2, 380 + (aryPSurfaceMainMenuItems[0]->h + 15) * 3,	aryPSurfaceMainMenuItems[3]->w, aryPSurfaceMainMenuItems[3]->h },
				};

				/* 控制动态背景图绘制 */
				SDL_RenderCopy(pWRenderer, pTextureBackground, &rcMainMenuBgImgClip, nullptr);
				if (bIsMainMenuBgImgMoveRight)
					if (rcMainMenuBgImgClip.x < (pSurfaceBackground->w - WINDOW_WIDTH))
						rcMainMenuBgImgClip.x++;
					else
						bIsMainMenuBgImgMoveRight = false;
				else
					if (rcMainMenuBgImgClip.x > 0)
						rcMainMenuBgImgClip.x--;
					else
						bIsMainMenuBgImgMoveRight = true;
				if (bIsMainMenuBgImgMoveDown)
					if (rcMainMenuBgImgClip.y < (pSurfaceBackground->h - WINDOW_HEIGHT))
						rcMainMenuBgImgClip.y++;
					else
						bIsMainMenuBgImgMoveDown = false;
				else
					if (rcMainMenuBgImgClip.y > 0)
						rcMainMenuBgImgClip.y--;
					else
						bIsMainMenuBgImgMoveDown = true;

				SDL_SetRenderDrawBlendMode(pWRenderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(pWRenderer, 65, 65, 65, 175);
				SDL_RenderFillRect(pWRenderer, nullptr);

				/* 将当前选项的前一选项恢复默认颜色 */
				SDL_FreeSurface(aryPSurfaceMainMenuItems[iPreMenuItemIndex]);
				SDL_DestroyTexture(aryPTextureMainMenuItems[iPreMenuItemIndex]);
				aryPSurfaceMainMenuItems[iPreMenuItemIndex] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, aryStrMainMenuItem[iPreMenuItemIndex].c_str(), clrText_MenuItemIdle);
				aryPTextureMainMenuItems[iPreMenuItemIndex] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceMainMenuItems[iPreMenuItemIndex]);

				/* 将当前选项改为选中颜色 */
				SDL_FreeSurface(aryPSurfaceMainMenuItems[iMainMenuItemIndex]);
				SDL_DestroyTexture(aryPTextureMainMenuItems[iMainMenuItemIndex]);
				aryPSurfaceMainMenuItems[iMainMenuItemIndex] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, aryStrMainMenuItem[iMainMenuItemIndex].c_str(), clrText_MenuItemHover);
				aryPTextureMainMenuItems[iMainMenuItemIndex] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceMainMenuItems[iMainMenuItemIndex]);

				SDL_RenderCopy(pWRenderer, pTextureMainMenuTitle_Another, nullptr, &rcText_Another);
				SDL_RenderCopy(pWRenderer, pTextureMainMenuTitle_World, nullptr, &rcText_World);
				SDL_RenderCopy(pWRenderer, pTextureMainMenuTitle_MapEditor, nullptr, &rcText_MapEditor);
				SDL_RenderCopy(pWRenderer, pTextureMainMenuTip, nullptr, &rcText_Tip);
				SDL_RenderCopy(pWRenderer, pTextureMainMenuAuthor, nullptr, &rcText_Author);
				for (int i = 0; i < 4; i++)
					SDL_RenderCopy(pWRenderer, aryPTextureMainMenuItems[i], nullptr, &aryRCTextMenuItems[i]);

				/* 绘制光标 */
				filledTrigonRGBA(pWRenderer, aryRCTextMenuItems[iMainMenuItemIndex].x - 65, aryRCTextMenuItems[iMainMenuItemIndex].y + 5, aryRCTextMenuItems[iMainMenuItemIndex].x - 55, aryRCTextMenuItems[iMainMenuItemIndex].y + aryRCTextMenuItems[iMainMenuItemIndex].h / 2, aryRCTextMenuItems[iMainMenuItemIndex].x - 25, aryRCTextMenuItems[iMainMenuItemIndex].y + aryRCTextMenuItems[iMainMenuItemIndex].h / 2, 136, 203, 127, 255);
				filledTrigonRGBA(pWRenderer, aryRCTextMenuItems[iMainMenuItemIndex].x - 65, aryRCTextMenuItems[iMainMenuItemIndex].y + aryRCTextMenuItems[iMainMenuItemIndex].h - 5, aryRCTextMenuItems[iMainMenuItemIndex].x - 55, aryRCTextMenuItems[iMainMenuItemIndex].y + aryRCTextMenuItems[iMainMenuItemIndex].h / 2, aryRCTextMenuItems[iMainMenuItemIndex].x - 25, aryRCTextMenuItems[iMainMenuItemIndex].y + aryRCTextMenuItems[iMainMenuItemIndex].h / 2, 123, 141, 66, 255);
			}
			else if (CurrentScene == SCENE_NEWMAP)
			{

			}
			else if (CurrentScene == SCENE_OPENMAP)
			{
				SDL_SetRenderDrawColor(pWRenderer, 75, 75, 75, 255);
				SDL_RenderFillRect(pWRenderer, nullptr);

				/* 绘制列表区域外边框 */
				roundedBoxRGBA(pWRenderer, 175, 115, WINDOW_WIDTH - 175, WINDOW_HEIGHT - 115, 4, 123, 124, 125, 255);
				roundedBoxRGBA(pWRenderer, 180, 120, WINDOW_WIDTH - 180, WINDOW_HEIGHT - 120, 4, 75, 75, 75, 255);

				SDL_Rect rcText_Title = { 250, 90, pSurfaceOpenMapScene_Title->w, pSurfaceOpenMapScene_Title->h },
					aryRCMaps[] = {
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 0, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 1, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 2, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 3, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 4, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 5, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 6, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
						{ 180, 120 + (WINDOW_HEIGHT - 240) / 8 * 7, WINDOW_WIDTH - 360, (WINDOW_HEIGHT - 240) / 8 },
					},
					aryRCTips[] = {
						{ 10, WINDOW_HEIGHT - aryPSurfaceOpenMapTips[0]->h - aryPSurfaceOpenMapTips[1]->h - 20, aryPSurfaceOpenMapTips[0]->w, aryPSurfaceOpenMapTips[0]->h },
						{ 10, WINDOW_HEIGHT - aryPSurfaceOpenMapTips[1]->h - 10, aryPSurfaceOpenMapTips[1]->w, aryPSurfaceOpenMapTips[1]->h },
					};

				for (int i = 0; i < 2; i++)
					SDL_RenderCopy(pWRenderer, aryPTextureOpenMapTips[i], nullptr, &aryRCTips[i]);

				/* 绘制列表项外边框 */
				for (auto rect : aryRCMaps)
				{
					roundedBoxRGBA(pWRenderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, 4, 113, 92, 31, 255);
					roundedBoxRGBA(pWRenderer, rect.x + 2, rect.y + 2, rect.x + rect.w - 2, rect.y + rect.h - 2, 4, 169, 158, 147, 255);
				}

				/* 绘制选中高亮项 */
				roundedBoxRGBA(pWRenderer, aryRCMaps[iMapListIndex - iMapShowingListFirstIndex].x + 2, aryRCMaps[iMapListIndex - iMapShowingListFirstIndex].y + 2, aryRCMaps[iMapListIndex - iMapShowingListFirstIndex].x + aryRCMaps[iMapListIndex - iMapShowingListFirstIndex].w - 2, aryRCMaps[iMapListIndex - iMapShowingListFirstIndex].y + aryRCMaps[iMapListIndex - iMapShowingListFirstIndex].h - 2, 4, 253, 222, 165, 255);

				SDL_Color clrText_MapName = { 28, 48, 92, 255 };

				/* 更新当前显示的地图文件名列表 */
				for (int i = 0; i < 8; i++)
					vShowingMapList[i] = vMapList[i + iMapShowingListFirstIndex];

				/* 绘制地图文件名称列表 */
				for (int i = 0; i < vShowingMapList.size(); i++)
				{
					SDL_Surface* _pSurfaceText =  TTF_RenderUTF8_Blended(pFont_XINWEI_30, vShowingMapList[i].c_str(), clrText_MapName);
					
					SDL_Texture* _pTextureText = SDL_CreateTextureFromSurface(pWRenderer, _pSurfaceText);
					SDL_Rect _rcText = { 205, 135 + (WINDOW_HEIGHT - 240) / 8 * i ,_pSurfaceText->w, _pSurfaceText->h };
					_rcText.w = _rcText.w > 870 ? 870 : _rcText.w;
					SDL_RenderCopy(pWRenderer, _pTextureText, nullptr, &_rcText);
					SDL_FreeSurface(_pSurfaceText);
					SDL_DestroyTexture(_pTextureText);
				}

				/* 绘制标题文字 */
				SDL_RenderCopy(pWRenderer, pTextureOpenMapScene_Title, nullptr, &rcText_Title);
			}
			else if (CurrentScene == SCENE_ABOUT)
			{

			}
		}

		SDL_RenderPresent(pWRenderer);

		unsigned int _end = SDL_GetTicks();

		if (_end - _start < 1000 / _FPS_)
			SDL_Delay(1000 / _FPS_ - (_end - _start));
	}

	SDL_DestroyRenderer(pWRenderer);
	SDL_DestroyWindow(pWindow);

	TTF_Quit();
	SDL_Quit();

	return 0;
}