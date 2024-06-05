#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <Windows.h>
#include "Games.h"

typedef std::string(*LPGETPLUGINGAMENAME)();
typedef std::shared_ptr<Games>(*LPGETPLUGINGAME)();

std::wstring DllsPath() {
    WCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return (std::wstring(buffer).substr(0, pos)).append(L"\\*.dll");
}

std::vector<std::pair<int, HMODULE>> EnumeratePlugin()
{
    std::vector<std::pair<int, HMODULE>> result;
    int index = 1;
    WIN32_FIND_DATA wfd;
    HANDLE hSerchHandle;

    std::wstring searchTemplate = DllsPath();
    hSerchHandle = FindFirstFile(searchTemplate.c_str(), &wfd);
    if (hSerchHandle == INVALID_HANDLE_VALUE)
        return result;

    do
    {
        HMODULE dll = LoadLibrary(wfd.cFileName);
        if (dll == NULL)
            continue;

        if (GetProcAddress(dll, "GetPluginGamesName") != NULL)
        {
            result.push_back(std::make_pair(index, dll));
            index++;
        }
        else
        {
            FreeLibrary(dll);
        }
    } while (FindNextFile(hSerchHandle, &wfd) != 0);

    FindClose(hSerchHandle);

    return result;
}

int main()
{
    std::vector<std::shared_ptr<Games>> games;
    int num = 0;
    bool exit = true;

    std::cout << "\033[30;43m                   \033[0m" << std::endl;
    std::cout << "\033[31;43m      Game\033[0m" << "\033[34;43mBOY      \033[0m" << std::endl;
    std::cout << "\033[30;46m       \033[0m" << "\033[31;46mM\033[0m" << "\033[35;46me\033[0m" << "\033[33;46mn\033[0m" << "\033[34;46mu\033[0m" << "\033[30;46m        \033[0m" << std::endl;
    std::cout << "\033[30;46m   Choose a game   \033[0m" << std::endl;

    std::vector<std::pair<int, HMODULE>> dlls = EnumeratePlugin();
    while (exit) {
        for (auto pluginInfo : dlls) {
            LPGETPLUGINGAMENAME GetPluginGameName = (LPGETPLUGINGAMENAME)GetProcAddress(pluginInfo.second, "GetPluginGamesName");
            if (GetPluginGameName == NULL)
                continue;
            std::cout << pluginInfo.first << ". " << GetPluginGameName() << std::endl;
        }

        std::cin >> num;

        HMODULE activeModule = NULL;

        for (auto pluginInfo : dlls)
        {
            if (pluginInfo.first == num) {
                activeModule = pluginInfo.second;
                break;
            }
        }

        if (activeModule == NULL)
        {
            std::cout << "error, try again!";
            continue;
        }

        LPGETPLUGINGAME GetPlugginGame = (LPGETPLUGINGAME)GetProcAddress(activeModule, "GetPluginGames");
        if (GetPlugginGame == NULL)
        {
            std::cout << "bad plug, try again!";
            continue;
        }

        games.push_back(GetPlugginGame());
        if (num != 0)
        {
            exit = false;
            continue;
        }

    }

    for (std::shared_ptr<Games> g : games) {
        std::cout << g->GetName() << " - " << g->GetStatus() << std::endl;
    }

    games.clear();

    for (auto dll : dlls)
    {
        FreeLibrary(dll.second);
    }
    system("pause");
}
