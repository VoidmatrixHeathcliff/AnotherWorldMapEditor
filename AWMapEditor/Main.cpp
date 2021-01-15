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
#include <ctime>

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

#define STARNUM 150

struct Star
{
	SDL_Point position;
	SDL_Color color;
	int radius;
	int speed;
};

int CurrentScene = SCENE_MAINMENU;

bool bIsRunning = true, bIsMainMenuBgImgMoveDown = true, bIsMainMenuBgImgMoveRight = true;

SDL_Window* pWindow = nullptr;
SDL_Renderer* pWRenderer = nullptr;

lua_State* pL;

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
SDL_Surface* pSurfaceAboutScene_Title = nullptr;
SDL_Surface* aryPSurfaceAboutScene_Content[2] = { nullptr, nullptr };
SDL_Surface* pSurfaceAboutScene_Contact = nullptr;
SDL_Surface* pSurfaceNewMapScene_Title = nullptr;
SDL_Surface* aryPSurfaceNewMapSceneTips[2] = { nullptr, nullptr };
SDL_Surface* pSurfaceNewMapScene_Suffix = nullptr;

SDL_Texture* pTextureBackground = nullptr;
SDL_Texture* pTextureMainMenuTitle_Another = nullptr;
SDL_Texture* pTextureMainMenuTitle_World = nullptr;
SDL_Texture* pTextureMainMenuTitle_MapEditor = nullptr;
SDL_Texture* pTextureMainMenuTip = nullptr;
SDL_Texture* pTextureMainMenuAuthor = nullptr;
SDL_Texture* aryPTextureMainMenuItems[MENUITEMLENGTH] = { nullptr, nullptr, nullptr, nullptr };
SDL_Texture* pTextureOpenMapScene_Title = nullptr;
SDL_Texture* aryPTextureOpenMapTips[2] = { nullptr, nullptr };
SDL_Texture* pTextureAboutScene_Title = nullptr;
SDL_Texture* aryPTextureAboutScene_Content[2] = { nullptr, nullptr };
SDL_Texture* pTextureAboutScene_Contact = nullptr;
SDL_Texture* pTextureNewMapScene_Title = nullptr;
SDL_Texture* aryPTextureNewMapTips[2] = { nullptr, nullptr };
SDL_Texture* pTextureNewMapScene_Suffix = nullptr;

int iMainMenuItemIndex = 0, iPreMenuItemIndex = MENUITEMLENGTH - 1, iMapListIndex = 0, iMapShowingListFirstIndex = 0;

string aryStrMainMenuItem[MENUITEMLENGTH] = { "新 建 地 图", "打 开 地 图", "关 于", "退 出" }, strNewMapName = "";

char validCharaList[] = {' ', 'A', 'a', 'B' , 'b' , 'C' , 'c' , 'D' , 'd' , 'E' , 'e' , 'F' , 'f' , 'G' , 'g' , 'H' , 'h' , 'I' , 'i' , 'J' , 'j' , 'K' , 'k' , 'L' , 'l' , 'M' , 'm' , 'N' , 'n' , 'O' , 'o' , 'P' , 'p' , 'Q' , 'q' , 'R' , 'r' , 'S' , 's' , 'T' , 't' , 'U' , 'u' , 'V' , 'v' , 'W' , 'w' , 'X' , 'x' , 'Y' , 'y' , 'Z' , 'z' , '0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '!' , '！' , '@' , '#' , '$' , '￥' , '%' , '^' , '…' , '&' , '*' , '（' , '（' , ')', '）', '-', '_', '—', '+', '=', '{', '}', '[', ']', '【', '】', '、', '：', ';', '；', '“', '”', '‘', '’', '《', '》', ',', '，', '.', '。', '？', '。' , '~' , '`', '·' };

SDL_Color clrText_MenuItemIdle = { 211, 203, 198, 255 }, clrText_MenuItemHover = { 254, 242, 99, 255 };

SDL_Rect rcMainMenuBgImgClip = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

vector<string> vMapList, vShowingMapList;
vector<Star> vStarList;

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
			SDL_StopTextInput();
		}
		else if (CurrentScene == SCENE_OPENMAP)
		{
			lua_close(pL);

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
			SDL_DestroyTexture(pTextureAboutScene_Title);
			for (auto& pTexture : aryPTextureAboutScene_Content)
				SDL_DestroyTexture(pTexture);
			SDL_DestroyTexture(pTextureAboutScene_Contact);

			SDL_FreeSurface(pSurfaceAboutScene_Title);
			for (auto& pSurface : aryPSurfaceAboutScene_Content)
				SDL_FreeSurface(pSurface);
			SDL_FreeSurface(pSurfaceAboutScene_Contact);

			TTF_CloseFont(pFont_XINWEI_70);
			TTF_CloseFont(pFont_XINWEI_40);

			vStarList.clear();
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
			SDL_StartTextInput();

			pFont_XINWEI_70 = TTF_OpenFont("resource/font/STXINWEI.TTF", 70);
			pFont_XINWEI_40 = TTF_OpenFont("resource/font/STXINWEI.TTF", 40);
			pFont_XINWEI_25 = TTF_OpenFont("resource/font/STXINWEI.TTF", 25);

			SDL_Color clrText_Title_Front = { 248, 244, 230, 255 }, clrText_Tip = { 226, 4, 27, 255 }, clrText_Suffix = { 171, 206, 216, 255 };

			pSurfaceNewMapScene_Title = TTF_RenderUTF8_Blended(pFont_XINWEI_70, "§ 新建地图 §", clrText_Title_Front);
			aryPSurfaceNewMapSceneTips[0] = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "请输入新地图的文件名，并按下 Enter 确认", clrText_Tip);
			aryPSurfaceNewMapSceneTips[1] = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "请确保文件名中无中文及其他特殊字符", clrText_Tip);
			pSurfaceNewMapScene_Suffix = TTF_RenderUTF8_Blended(pFont_XINWEI_40, ".map", clrText_Suffix);

			pTextureNewMapScene_Title = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceNewMapScene_Title);
			aryPTextureNewMapTips[0] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceNewMapSceneTips[0]);
			aryPTextureNewMapTips[1] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceNewMapSceneTips[1]);
			pTextureNewMapScene_Suffix = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceNewMapScene_Suffix);
		}
		else if (CurrentScene == SCENE_OPENMAP)
		{
			pL = luaL_newstate();

			pFont_XINWEI_40 = TTF_OpenFont("resource/font/STXINWEI.TTF", 40);
			pFont_XINWEI_30 = TTF_OpenFont("resource/font/STXINWEI.TTF", 30);
			pFont_XINWEI_25 = TTF_OpenFont("resource/font/STXINWEI.TTF", 25);

			SDL_Color clrText_Title_Front = { 248, 244, 230, 255 }, clrText_Title_Back = { 75, 75, 75, 255 }, clrText_Tip = { 226, 4, 27, 255 };

			pSurfaceOpenMapScene_Title = TTF_RenderUTF8_Shaded(pFont_XINWEI_40, "§ 打开地图 §", clrText_Title_Front, clrText_Title_Back);
			aryPSurfaceOpenMapTips[0] = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "按下 ↑ ↓ ← → 移动光标，按下 Enter 打开，按下 Esc 退回到主菜单", clrText_Tip);
			aryPSurfaceOpenMapTips[1] = TTF_RenderUTF8_Blended(pFont_XINWEI_25, "将地图文件放置于“maps”文件夹下，并确保文件名中无中文及其他特殊字符", clrText_Tip);

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
						_index++;
					}
				} while (!_findnext(hFile, &fileinfo));
				_findclose(hFile);
			}
		}
		else if (CurrentScene == SCENE_ABOUT)
		{
			srand(time(0));

			for (int i = 0; i < STARNUM; i++)
			{
				Star _star;
				_star.position.x = rand() % WINDOW_WIDTH, _star.position.y = rand() % WINDOW_HEIGHT;
				_star.color.r = rand() % 255, _star.color.g = rand() % 255, _star.color.b = rand() % 255, _star.color.a = rand() % 255;
				_star.radius = rand() % 5;
				_star.speed = rand() % 2 + 1;
				vStarList.push_back(_star);
			}

			pFont_XINWEI_70 = TTF_OpenFont("resource/font/STXINWEI.TTF", 70);
			pFont_XINWEI_40 = TTF_OpenFont("resource/font/STXINWEI.TTF", 40);

			SDL_Color clrText_Title = { 193, 228, 233, 210 }, clrText_Content = { 255, 241, 207, 210 }, clrText_Contact = { 233, 82, 149, 210 };

			pSurfaceAboutScene_Title = TTF_RenderUTF8_Blended(pFont_XINWEI_70, "Another World Map Editor ★", clrText_Title);
			aryPSurfaceAboutScene_Content[0] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, "为 Another World MUD 的地图设计辅助工具，", clrText_Content);
			aryPSurfaceAboutScene_Content[1] = TTF_RenderUTF8_Blended(pFont_XINWEI_40, "源代码根据MIT协议开源，允许一切非商业目的修改和使用", clrText_Content);

			TTF_SetFontStyle(pFont_XINWEI_40, TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);
			pSurfaceAboutScene_Contact = TTF_RenderUTF8_Blended(pFont_XINWEI_40, "联系开发者：Voidmatrix@outlook.com", clrText_Contact);
			TTF_SetFontStyle(pFont_XINWEI_40, TTF_STYLE_NORMAL);

			pTextureAboutScene_Title = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceAboutScene_Title);
			aryPTextureAboutScene_Content[0] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceAboutScene_Content[0]);
			aryPTextureAboutScene_Content[1] = SDL_CreateTextureFromSurface(pWRenderer, aryPSurfaceAboutScene_Content[1]);
			pTextureAboutScene_Contact = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceAboutScene_Contact);
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
			if (_event.type == SDL_QUIT || (_event.type == SDL_KEYDOWN && _event.key.keysym.sym == SDLK_ESCAPE && CurrentScene == SCENE_MAINMENU))
				ManageScene(ACTION_QUIT, 0);

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
				switch (_event.type)
				{
				case SDL_KEYDOWN:
					switch (_event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						ManageScene(ACTION_JUMP, SCENE_MAINMENU);
						break;
					case SDLK_RETURN:
					case SDLK_KP_ENTER:
						strNewMapName.erase(0, strNewMapName.find_first_not_of(" "));
						if (!strNewMapName.empty())
						{
							const SDL_MessageBoxButtonData buttons[] = {
								{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "确定" },
								{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "取消" },
							};
							const SDL_MessageBoxColorScheme colorScheme = {
								{
									{ 255, 0, 0 },
									{ 0, 255, 0 },
									{ 255, 255, 0 },
									{ 0, 0, 255 },
									{ 255, 0, 255 }
								}
							};
							const SDL_MessageBoxData messageboxdata = {
								SDL_MESSAGEBOX_INFORMATION, pWindow, "提示", string("确定要创建地图：\"").append(strNewMapName).append(".map\" 吗？\n\n创建的地图文件将位于 “maps” 文件夹下").c_str(), SDL_arraysize(buttons), buttons, &colorScheme };

							int buttonid = 0;
							SDL_ShowMessageBox(&messageboxdata, &buttonid);
							if (buttonid == 1) 
							{
								/* 处理生成指定名称的地图文件 */
							}
						}
						else
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "地图创建失败", "地图文件名不能为空！", pWindow);
						break;
					case SDLK_BACKSPACE:
					case SDLK_KP_BACKSPACE:
						strNewMapName = strNewMapName.substr(0, strNewMapName.length() - 1);
						break;
					default:
						break;
					}
					break;
				case SDL_TEXTINPUT:
					for (auto c : validCharaList)
						if (c == _event.text.text[0])
						{
							strNewMapName += c;
							break;
						}
					break;
				default:
					break;
				}
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
						if (luaL_dofile(pL, ("maps/" + vMapList[iMapListIndex]).c_str()))
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "地图打开失败", "地图文件损坏，请勿从外部修改地图数据\n如需恢复，请联系开发者：Voidmatrix@outlook.com", pWindow);
						else
						{
							// 此处补全代码：读取地图数据存储到变量中

							ManageScene(ACTION_JUMP, SCENE_EDITOR);
						}
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
				if (_event.type == SDL_KEYDOWN && (_event.key.keysym.sym == SDLK_RETURN || _event.key.keysym.sym == SDLK_ESCAPE || _event.key.keysym.sym == SDLK_SPACE))
					ManageScene(ACTION_JUMP, SCENE_MAINMENU);
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
				SDL_SetRenderDrawColor(pWRenderer, 75, 75, 75, 255);
				SDL_RenderFillRect(pWRenderer, nullptr);

				SDL_Rect rcText_Title = { WINDOW_WIDTH / 2 - pSurfaceNewMapScene_Title->w / 2, 125, pSurfaceNewMapScene_Title->w, pSurfaceNewMapScene_Title->h },
					aryRCTips[] = {
						{ 10, WINDOW_HEIGHT - aryPSurfaceNewMapSceneTips[0]->h - aryPSurfaceNewMapSceneTips[1]->h - 20, aryPSurfaceNewMapSceneTips[0]->w, aryPSurfaceNewMapSceneTips[0]->h },
						{ 10, WINDOW_HEIGHT - aryPSurfaceNewMapSceneTips[1]->h - 10, aryPSurfaceNewMapSceneTips[1]->w, aryPSurfaceNewMapSceneTips[1]->h }
				},
					rcText_Suffix = { WINDOW_WIDTH - pSurfaceNewMapScene_Suffix->w - 125, WINDOW_HEIGHT / 2 - pSurfaceNewMapScene_Suffix->h / 2, pSurfaceNewMapScene_Suffix->w, pSurfaceNewMapScene_Suffix->h },
					rcInputBox = { 125, rcText_Suffix.y - 10, rcText_Suffix.x - 145, rcText_Suffix.h + 20 };

				/* 绘制输入框 */
				SDL_SetRenderDrawColor(pWRenderer, 215, 237, 239, 255);
				SDL_RenderFillRect(pWRenderer, &rcInputBox);
				thickLineRGBA(pWRenderer, rcInputBox.x + rcInputBox.w, rcInputBox.y, rcInputBox.x + rcInputBox.w, rcInputBox.y + rcInputBox.h, 4, 235, 246, 247, 255);
				thickLineRGBA(pWRenderer, rcInputBox.x, rcInputBox.y + rcInputBox.h, rcInputBox.x + rcInputBox.w, rcInputBox.y + rcInputBox.h, 4, 235, 246, 247, 255);
				thickLineRGBA(pWRenderer, rcInputBox.x, rcInputBox.y, rcInputBox.x + rcInputBox.w, rcInputBox.y, 4, 60, 142, 150, 255);
				thickLineRGBA(pWRenderer, rcInputBox.x, rcInputBox.y, rcInputBox.x, rcInputBox.y + rcInputBox.h, 4, 60, 142, 150, 255);

				/* 绘制输入文字 */
				if (!strNewMapName.empty())
				{
					SDL_Color clrText_NewMapName = { 222, 176, 104, 255 };
					SDL_Surface* _pSurfaceText = TTF_RenderUTF8_Blended(pFont_XINWEI_40, strNewMapName.c_str(), clrText_NewMapName);
					SDL_Texture* _pTextureText = SDL_CreateTextureFromSurface(pWRenderer, _pSurfaceText);
					SDL_Rect _rcCutNewMapNameText = { 0, 0, rcInputBox.w - 40, _pSurfaceText->h },
						rcNewMapNameTextArea = { rcInputBox.x + 20, rcInputBox.y + 10, _rcCutNewMapNameText.w < _pSurfaceText->w ? _rcCutNewMapNameText.w : _pSurfaceText->w, _pSurfaceText->h };
					SDL_RenderCopy(pWRenderer, _pTextureText, &_rcCutNewMapNameText, &rcNewMapNameTextArea);
					SDL_FreeSurface(_pSurfaceText);
					SDL_DestroyTexture(_pTextureText);
				}
				
				SDL_RenderCopy(pWRenderer, pTextureNewMapScene_Title, nullptr, &rcText_Title);
				for (int i = 0; i < 2; i++)
					SDL_RenderCopy(pWRenderer, aryPTextureNewMapTips[i], nullptr, &aryRCTips[i]);
				SDL_RenderCopy(pWRenderer, pTextureNewMapScene_Suffix, nullptr, &rcText_Suffix);
			}
			else if (CurrentScene == SCENE_OPENMAP)
			{
				SDL_SetRenderDrawColor(pWRenderer, 75, 75, 75, 255);
				SDL_RenderFillRect(pWRenderer, nullptr);

				/* 绘制列表区域外边框 */
				roundedBoxRGBA(pWRenderer, 175, 115, WINDOW_WIDTH - 175, WINDOW_HEIGHT - 115, 4, 123, 124, 125, 255);
				roundedBoxRGBA(pWRenderer, 180, 120, WINDOW_WIDTH - 180, WINDOW_HEIGHT - 120, 4, 75, 75, 75, 255);

				SDL_Rect rcText_Title = { WINDOW_WIDTH / 2 - pSurfaceOpenMapScene_Title->w / 2, 90, pSurfaceOpenMapScene_Title->w, pSurfaceOpenMapScene_Title->h },
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
				for (int i = 0; i < (vMapList.size() < 8 ? vMapList.size() : 8); i++)
					vShowingMapList[i] = vMapList[i + iMapShowingListFirstIndex];

				/* 绘制地图文件名称列表 */
				for (int i = 0; i < vShowingMapList.size(); i++)
				{
					SDL_Surface* _pSurfaceText = TTF_RenderUTF8_Blended(pFont_XINWEI_30, vShowingMapList[i].c_str(), clrText_MapName);
					
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
				SDL_SetRenderDrawColor(pWRenderer, 15, 15, 15, 255);
				SDL_RenderFillRect(pWRenderer, nullptr);

				for (auto& star : vStarList)
				{
					if ((star.position.x > WINDOW_WIDTH / 2 - 5 && star.position.x < WINDOW_WIDTH / 2 + 5) || (star.position.y > WINDOW_HEIGHT / 2 - 5 && star.position.y < WINDOW_HEIGHT / 2 + 5))
					{
						star.position.x = rand() % WINDOW_WIDTH, star.position.y = rand() % WINDOW_HEIGHT;
						star.color.r = rand() % 255, star.color.g = rand() % 255, star.color.b = rand() % 255, star.color.a = rand() % 255;
						star.radius = rand() % 5;
					}
					else
					{
						star.position.x = star.position.x < WINDOW_WIDTH / 2 ? star.position.x + star.speed : star.position.x - star.speed;
						star.position.y = star.position.y < WINDOW_HEIGHT / 2 ? star.position.y + star.speed : star.position.y - star.speed;
						filledCircleRGBA(pWRenderer, star.position.x, star.position.y, star.radius, star.color.r, star.color.g, star.color.b, star.color.a);
					}
				}

				SDL_Rect rcText_Title = { 115, 175, pSurfaceAboutScene_Title->w, pSurfaceAboutScene_Title->h },
					aryRCContentText[] = {
						{ WINDOW_WIDTH / 2 - aryPSurfaceAboutScene_Content[0]->w / 2 + 125, 325, aryPSurfaceAboutScene_Content[0]->w, aryPSurfaceAboutScene_Content[0]->h },
						{ WINDOW_WIDTH / 2 - aryPSurfaceAboutScene_Content[1]->w / 2, 415, aryPSurfaceAboutScene_Content[1]->w, aryPSurfaceAboutScene_Content[1]->h }
					},
					rcText_Contact = { WINDOW_WIDTH / 2 - pSurfaceAboutScene_Contact->w / 2, aryRCContentText[1].y + 175, pSurfaceAboutScene_Contact->w, pSurfaceAboutScene_Contact->h };

				SDL_RenderCopy(pWRenderer, pTextureAboutScene_Title, nullptr, &rcText_Title);
				for (int i = 0; i < 2; i++)
					SDL_RenderCopy(pWRenderer, aryPTextureAboutScene_Content[i], nullptr, &aryRCContentText[i]);
				SDL_RenderCopy(pWRenderer, pTextureAboutScene_Contact, nullptr, &rcText_Contact);
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