#pragma once
#include "dllmain.hpp"

#define JMPSIZE 14

using namespace injector;
using namespace std;
using namespace reframework;

// if (!pattern.count_hint(1).empty())
// {
//     char *orig_func = new char[30];
//     auto func_addr = pattern.get_first(0);
//     API::get()->log_info("分配写入权限");
//     ProtectMemory(orig_func, 30, PAGE_EXECUTE_READWRITE);
//     // HMODULE h = GetModuleHandleA(NULL);
//     memcpy(orig_func, func_addr, JMPSIZE + 1);
//     API::get()->log_info("备份原函数");
//     loadFile_step2 = (void *(*)(void *, void *, int))orig_func;
//     API::get()->log_info("赋值原始函数指针");
//     MakeAbsJMP(&orig_func[15], func_addr + 15, true);
//     API::get()->log_info("跳回原函数");
//     MakeAbsJMP(func_addr, loadFile_step2_fun, true);
//     API::get()->log_info("构造jmp在函数头部");
// }
namespace XMHook
{
    class Hook
    {
    public:
        LPVOID original;
        Hook() {}
        ~Hook() {}

        injector::memory_pointer_raw MakeAbsJMP(injector::memory_pointer_tr at, injector::memory_pointer_raw dest, bool vp = true)
        {
            injector::WriteMemory<uint16_t>(at, 0x25FF, vp);
            injector::WriteMemory<uint32_t>(at + sizeof(uint16_t), 0, vp);
            injector::WriteMemory<uint64_t>(at + sizeof(uint16_t) + sizeof(uint32_t), dest.as_int(), vp);
            return at.as_int() + JMPSIZE;
        }

        void HookByPattern(
            std::string_view hax_text,
            LPVOID loadFile_step2_fun,
            size_t buff_size = 128,
            size_t back_size = JMPSIZE)
        {
            hook::pattern pattern = hook::pattern(hax_text);
            if (!pattern.count_hint(1).empty())
            {
                try
                {
                    char *orig_func = new char[buff_size];
                    auto func_addr = pattern.get_first(0);

                    ProtectMemory(orig_func, buff_size, PAGE_EXECUTE_READWRITE);    // 分配写入权限
                    memcpy(orig_func, func_addr, back_size);                        // 备份原函数
                    original = (LPVOID)orig_func;                                   // 赋值原始函数指针
                    MakeAbsJMP(&orig_func[back_size], func_addr + back_size, true); // 跳回原函数
                    MakeAbsJMP(func_addr, loadFile_step2_fun, true);                // 构造jmp在函数头部
                    Logger::info("ModLoader", " %s XMHook 成功", hax_text.data());
                }
                catch (const std::exception &e)
                {
                    Logger::info("ModLoader", " %s XMHook 失败:: %s", hax_text.data(), e.what());
                }
            }
            else
            {
                Logger::info("ModLoader", " %s XMHook 失败", hax_text.data());
            }
        }
    };
}