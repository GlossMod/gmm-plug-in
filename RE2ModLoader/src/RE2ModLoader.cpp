#include "RE2ModLoader.h"

using namespace reframework;
using namespace injector;
using namespace std;

/**
 * hook 函数 loadFile_step2
.text:0000000142071FD0                               ; __int64 __fastcall loadFile_step2(__int64, __int64, int)
.text:0000000142071FD0                               loadFile_step2 proc near                ; CODE XREF: loadFile+9↑p
.text:0000000142071FD0                                                                       ; sub_142118FD0+D1↓p
.text:0000000142071FD0                                                                       ; sub_14284EF20+1B4↓p
.text:0000000142071FD0                                                                       ; sub_14288B220+6C4↓p
.text:0000000142071FD0                                                                       ; sub_14584E3D0+80↓p
.text:0000000142071FD0                                                                       ; DATA XREF: .pdata:000000014985AACC↓o
.text:0000000142071FD0
.text:0000000142071FD0                               arg_0= qword ptr  8
.text:0000000142071FD0                               arg_8= qword ptr  10h
.text:0000000142071FD0                               arg_10= qword ptr  18h
.text:0000000142071FD0
.text:0000000142071FD0 48 89 5C 24 08                mov     [rsp+arg_0], rbx
.text:0000000142071FD5 48 89 6C 24 10                mov     [rsp+arg_8], rbp
.text:0000000142071FDA 48 89 74 24 18                mov     [rsp+arg_10], rsi
.text:0000000142071FDF 57                            push    rdi
.text:0000000142071FE0 48 83 EC 20                   sub     rsp, 20h
.text:0000000142071FE4 48 8B F1                      mov     rsi, rcx
.text:0000000142071FE7 44 89 41 08                   mov     [rcx+8], r8d
.text:0000000142071FEB 48 8D 05 9E B5 87 03          lea     rax, off_1458ED590
.text:0000000142071FF2 48 8B EA                      mov     rbp, rdx
.text:0000000142071FF5 48 89 01                      mov     [rcx], rax
.text:0000000142071FF8 41 8B F8                      mov     edi, r8d
.text:0000000142071FFB 48 8D 05 DE BE 84 03          lea     rax, off_1458BDEE0
.text:0000000142072002 48 89 41 10                   mov     [rcx+10h], rax
.text:0000000142072006 48 8D 56 10                   lea     rdx, [rsi+10h]
.text:000000014207200A 48 83 C1 18                   add     rcx, 18h
.text:000000014207200E E8 8D FB FA FF                call    sub_142021BA0
*/

//   原始函数指针
void *(*loadFile_step2)(void *, void *, int) = nullptr;
UINT64(*PathToHash)
(wchar_t *) = nullptr;
// int (*CheckFileInPak)(void *, UINT64) = nullptr;
// void (*_CloseHandle)(HANDLE *) = nullptr;

map<UINT64, string> hashs;
// map<int, string> Natives;

#pragma region 代理函数
void *loadFile_step2_fun(void *a, wchar_t *b, int c)
{
    std::string path = wideCharToMultiByte(b);
    UINT64 hash = PathToHash(b);
    Logger::info("loadFile", "hash: %i ,  path: %s", hash, path.c_str());

    return loadFile_step2(a, b, c); // 返回原内容

    // for (auto &item : Natives)
    // {
    //     auto pos = item.second.find_last_of("\\");
    //     auto fileName = item.second.substr(pos + 1);
    //     fileName = fileName.substr(0, fileName.find_last_of("."));
    //     if (path.find(fileName) != string::npos)
    //     {
    //         Logger::info("ModLoader", "已找到 %s", item.second.c_str());
    //         multiByteToWideChar(item.second, b);
    //     }
    // }
    // if (filesystem::exists(path.c_str()))
    // {
    //     hashs[hash] = path;
    // }
    // Logger::info("ModLoader", "hash: %i ,  path: %s", hash, path.c_str());
    // return loadFile_step2(a, b, c); // 返回原内容
}

UINT64 PathToHash_fun(wchar_t *a)
{
    return PathToHash(a);
}

int CheckFileInPak_fun(void *a1, UINT64 a2)
{
    int ret = CheckFileInPak(a1, a2);
    Logger::info("ModLoader", "CheckFileInPak: %i , as: %i", ret, a2);
    if (ret == -1)
    {
        return ret;
    }
    if (hashs.find(a2) != hashs.end())
    {
        if (filesystem::exists(hashs[a2]))
        {
            ret = -1;
        }
        else
        {
            hashs.erase(a2);
        }
    }
    return ret;
}

void _CloseHandle_fun(HANDLE *handle)
{

    return _CloseHandle(handle);
}

#pragma endregion

void Load()
{

    // hook loadFile_step2
    auto loadFile_step2_hook = new XMHook::Hook();
    loadFile_step2_hook->HookByPattern(
        "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F1 44 89 41 08",
        (LPVOID)loadFile_step2_fun, 30, 15);
    loadFile_step2 = (void *(*)(void *, void *, int))loadFile_step2_hook->original;

    // hook PathToHash
    auto PathToHash_hook = new XMHook::Hook();
    PathToHash_hook->HookByPattern(
        "40 55 53 41 56 48 8D AC 24 C0 F0 FF FF",
        (LPVOID)PathToHash_fun, 32, 18);
    PathToHash = (UINT64(*)(wchar_t *))PathToHash_hook->original;

    // // hook CheckFileInPak
    // auto CheckFileInPak_hook = new XMHook::Hook();
    // CheckFileInPak_hook->HookByPattern(
    //     "41 56 41 57 48 83 EC 28 48 83 B9 A8 00 00 00 00",
    //     (LPVOID)CheckFileInPak_fun, 30, 16);
    // CheckFileInPak = (int (*)(void *, UINT64))CheckFileInPak_hook->original;
}

#pragma region REFrameworkPlugin

extern "C" __declspec(dllexport) void reframework_plugin_required_version(REFrameworkPluginVersion *version)
{
    version->major = REFRAMEWORK_PLUGIN_VERSION_MAJOR;
    version->minor = REFRAMEWORK_PLUGIN_VERSION_MINOR;
    version->patch = REFRAMEWORK_PLUGIN_VERSION_PATCH;
}

extern "C" __declspec(dllexport) bool reframework_plugin_initialize(const REFrameworkPluginInitializeParam *param)
{
    reframework::API::initialize(param);
    return true;
}

#pragma endregion

std::string wideCharToMultiByte(wchar_t *pWCStrKey)
{
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char *pCStrKey = new char[pSize + 1];
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    std::string str = std::string(pCStrKey);
    delete[] pCStrKey;
    return str;
}
void multiByteToWideChar(const string &pKey, wchar_t *pWCStrKey)
{
    const char *pCStrKey = pKey.c_str();
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
}

// // 获取 natives 目录下所有文件 包含子目录
// map<int, std::string> getNativesFileList()
// {
//     std::map<int, std::string> fileMap;
//     int counter = 0;
//     for (const auto &entry : std::filesystem::recursive_directory_iterator("natives"))
//     {
//         Logger::info("ModLoader", "文件: %s", entry.path().string().c_str());

//         // 判断是否为文件，若为目录则不处理
//         if (std::filesystem::is_regular_file(entry.status()))
//         {
//             // 插入数据到映射中
//             fileMap.insert(std::pair<int, std::string>(counter, entry.path().string()));
//             counter++;
//         }
//     }
//     return fileMap;
// }

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Natives = getNativesFileList();
        Load();
    }
    return TRUE;
}
