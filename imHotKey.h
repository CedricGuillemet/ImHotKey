// ImHotKey v1.0
// https://github.com/CedricGuillemet/ImHotKey
//
// The MIT License(MIT)
// 
// Copyright(c) 2019 Cedric Guillemet
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#pragma once
/*
How to use it?

// Get some hotkeys composed of:
// - hotkey name
// - hotkey comment/lib
// - hotkey scancodes. Computed by the editor. Store that value in your app.

static std::vector<ImHotKey::HotKey> hotkeys = { { "Layout", "Reorder nodes in a simpler layout", 0xFFFF261D}
        ,{"Save", "Save the current graph", 0xFFFF1F1D}
        ,{"Load", "Load an existing graph file", 0xFFFF181D}
        ,{"Play/Stop", "Play or stop the animation from the current graph", 0xFFFFFF3F}
        ,{"SetKey", "Make a new animation key with the current parameters values at the current time", 0xFFFFFF1F}
        };

// The editor is a modal window. bring it with something like that
 if (ImGui::Button("Edit Hotkeys"))
{
    ImGui::OpenPopup("HotKeys Editor");
}
ImHotKey::Edit(hotkeys.data(), hotkeys.size(), "HotKeys Editor");

// ImHotKey also provides a way to retrieve HotKey
int hotkey = ImHotKey::GetHotKey(hotkeys.data(), hotkeys.size());
if (hotkey != -1)
{
    // handle the hotkey index!
}

Awesome, you are done!
To help you integrate in your app, you can get a text (like "Ctrl + O") to integrate in your menu
static void GetHotKeyLib(unsigned int functionKeys, char* buffer, size_t bufferSize);

*/

#include "imgui.h"
#include "imgui_internal.h"

namespace ImHotKey
{
    struct HotKey
    {
        const char *functionName;
        const char *functionLib;
        unsigned int functionKeys;
    };

    struct Key
    {
        const char* lib = nullptr;
        unsigned int order;
        unsigned int scanCode = 0;
        float offset = 0;
        float width = 40;
    };

    static const Key Keys[6][18] = {
        { {"Esc", 4, 0x1, 18}, {"F1", 5, 0x3B, 18}, {"F2", 6, 0x3C}, {"F3", 7, 0x3D}, {"F4", 8, 0x3E}, {"F5", 9, 0x3F, 24}, {"F6", 10, 0x40}, {"F7", 11, 0x41}, {"F8", 12, 0x42}, {"F9", 13, 0x43, 24}, {"F10", 14, 0x44}, {"F11", 15, 0x57}, {"F12", 16, 0x58}, {"PrSn", 17, 0x37, 24}, {"ScLk", 18, 0x46}, {"Brk", 19, 126} },
        { {"~", 20, 0x29}, {"1", 21, 0x2}, {"2", 22, 0x3}, {"3", 23, 0x4}, {"4", 24, 0x5}, {"5", 25, 0x6}, {"6", 26, 0x7}, {"7", 27, 0x8}, {"8", 28, 0x9}, {"9", 29, 0xA}, {"0", 30, 0xB}, {"-", 31, 0xC}, {"+", 32, 0xD},{"Backspace", 33, 0xE, 0, 80}, {"Ins", 34, 0x52, 24}, {"Hom", 35, 0x47}, {"PgU", 36, 0x49} },
        { {"Tab", 3, 0xF, 0, 60}, {"Q", 37, 0x10}, {"W", 38, 0x11}, {"E", 39, 0x12}, {"R", 40, 0x13}, {"T", 41, 0x14}, {"Y", 42, 0x15}, {"U", 43, 0x16}, {"I", 44, 0x17}, {"O", 45, 0x18}, {"P", 46, 0x19}, {"[", 47, 0x1A}, {"]", 48, 0x1B}, {"|", 49, 0x2B, 0, 60}, {"Del", 50, 0x53, 24}, {"End", 51, 0x4F}, {"PgD", 52, 0x51} },
        { {"Caps Lock", 53, 0x3A, 0, 80}, {"A", 54, 0x1E}, {"S", 55, 0x1F}, {"D", 56, 0x20}, {"F", 57, 0x21}, {"G", 58, 0x22}, {"H", 59, 0x23}, {"J", 60, 0x24}, {"K", 61, 0x25}, {"L", 62, 0x26}, {";", 63, 0x27}, {"'", 64, 0x28}, {"Ret", 65, 0x1C, 0, 84} },
        { {"Shift", 2, 0x2A, 0, 104}, {"Z", 66, 0x2C}, {"X", 67, 0x2D}, {"C", 68, 0x2E}, {"V", 69, 0x2F}, {"B", 70, 0x30}, {"N", 71, 0x31}, {"M", 72, 0x32}, {",", 73, 0x33}, {".", 74, 0x34}, {"/", 75, 0x35}, {"Shift", 2, 0x2A, 0, 104}, {"Up", 76, 0x48, 68} },
        { {"Ctrl", 0, 0x1D, 0, 60}, {"Alt", 1, 0x38, 68, 60}, {"Space", 77, 0x39, 0, 260}, {"Alt", 1, 0x38, 0, 60}, {"Ctrl", 0, 0x1D, 68, 60}, {"Left", 78, 0x4B, 24}, {"Down", 79, 0x50}, {"Right", 80, 0x4D} }
    };

    static const Key& GetKeyForScanCode(unsigned int scancode)
    {
        for (unsigned int y = 0; y < 6; y++)
        {
            int x = 0;
            while (Keys[y][x].lib)
            {
                if (Keys[y][x].scanCode == scancode)
                    return Keys[y][x];
                x++;
            }
        }
        return Keys[0][0];
    }

    static unsigned int GetOrderedScanCodes(unsigned char scanCodes[4], unsigned char order[4])
    {
        for (int pass = 0; pass < 2; pass++)
        {
            for (int o = 0; o < 3; o++)
            {
                if (order[o] > order[o + 1])
                {
                    ImSwap(order[o], order[o + 1]); ImSwap(scanCodes[o], scanCodes[o + 1]);
                }
            }
        }
        return (scanCodes[3] << 24) + (scanCodes[2] << 16) + (scanCodes[1] << 8) + scanCodes[0];
    }

    static void HotKeySPrintf(char* buffer, size_t bufferSize, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        ImFormatStringV(buffer, bufferSize, fmt, args);
        va_end(args);
    }

    static void GetHotKeyLib(unsigned int functionKeys, char* buffer, size_t bufferSize)
    {
        static const char* str[4] = { "%s", "%s + %s", "%s + %s +%s", "%s + %s + %s + %s" };
        static const char* lib[4];
        int scanCodeCount = 0;
        for (int i = 0; i < 4; i++)
        {
            unsigned char scanCode = (unsigned char)(functionKeys >> i * 8);
            if (scanCode == 0xFF)
            {
                continue;
            }
            lib[scanCodeCount] = GetKeyForScanCode(scanCode).lib;
            scanCodeCount++;
        }
        if (!scanCodeCount)
        {
            buffer[0] = 0;
            return;
        }
        const char* fmt = str[scanCodeCount-1];
        HotKeySPrintf(buffer, bufferSize, fmt, lib[0], lib[1], lib[2], lib[3]);
    }

    static void Edit(HotKey *hotkey, size_t hotkeyCount, const char *popupModal)
    {
        static int editingHotkey = -1;
        if (!hotkeyCount)
            return;
        static bool keyDown[512] = {};

        ImGui::SetNextWindowSize(ImVec2(1060, 400));
        if (!ImGui::BeginPopupModal(popupModal, NULL, ImGuiWindowFlags_NoResize))
            return;

        ImGui::BeginChildFrame(127, ImVec2(220, -1));
        for(size_t i = 0;i< hotkeyCount;i++)
        {
            char hotKeyLib[128], selectableText[128];
            GetHotKeyLib(hotkey[i].functionKeys, hotKeyLib, sizeof(hotKeyLib));
            HotKeySPrintf(selectableText, sizeof(selectableText), "%s (%s)", hotkey[i].functionName, hotKeyLib);
            if (ImGui::Selectable(selectableText, editingHotkey == int(i)) || editingHotkey == -1)
            {
                editingHotkey = i;
                memset(keyDown, 0, sizeof(keyDown));
                for (int j = 0; j < 4; j++)
                {
                    int scan = (hotkey[editingHotkey].functionKeys >> (8 * j)) & 0xFF;
                    if (scan != 0xFF)
                    {
                        keyDown[scan] = true;
                    }
                }
            }
        }
        ImGui::EndChildFrame();
        ImGui::SameLine();
        ImGui::BeginGroup();

        for (int i = 0; i < 512; i++)
        {
            if (ImGui::IsKeyPressed(i, false))
            {
                int imKey;
#ifdef WIN32
                imKey = MapVirtualKeyA(i, MAPVK_VK_TO_VSC);
#else
                imKey = i;
#endif
                keyDown[imKey] = !keyDown[imKey];
            }
        }
        for (unsigned int y = 0; y < 6; y++)
        {
            int x = 0;
            ImGui::BeginGroup();
            while (Keys[y][x].lib)
            {
                const Key& key = Keys[y][x];
                const float ofs = key.offset + (x?4.f:0.f);

                const float width = key.width;
                if (x)
                {
                    ImGui::SameLine(0.f, ofs);
                }
                else
                {
                    if (ofs >= 1.f)
                    {
                        ImGui::Indent(ofs);
                    }
                }
                bool& butSwtch = keyDown[key.scanCode];
                ImGui::PushStyleColor(ImGuiCol_Button, butSwtch ? 0xFF1040FF : 0x80000000);
                if (ImGui::Button(Keys[y][x].lib, ImVec2(width, 40)))
                {
                    butSwtch = !butSwtch;
                }
                ImGui::PopStyleColor();
                x++;
            }
            ImGui::EndGroup();
        }
        ImGui::InvisibleButton("space", ImVec2(10, 55));
        ImGui::BeginChildFrame(18, ImVec2(540, 40));
        ImGui::Text("%s :", hotkey[editingHotkey].functionName);
        ImGui::SameLine();
        ImGui::TextWrapped("%s", hotkey[editingHotkey].functionLib);
        ImGui::EndChildFrame();
        ImGui::SameLine();
        int keyDownCount = 0;
        for (auto d : keyDown)
        {
            keyDownCount += d ? 1 : 0;
        }
        if (ImGui::Button("Clear", ImVec2(80, 40)))
        {
            memset(keyDown, 0, sizeof(keyDown));
        }
        ImGui::SameLine();
        if (keyDownCount && keyDownCount < 5)
        {
            if (ImGui::Button("Set", ImVec2(80, 40)))
            {
                unsigned char scanCodes[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
                unsigned char order[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
                int scanCodeCount = 0;
                hotkey[editingHotkey].functionKeys = 0;
                for (int i = 1; i < sizeof(keyDown); i++)
                {
                    if (keyDown[i])
                    {
                        scanCodes[scanCodeCount] = (unsigned char)i;
                        order[scanCodeCount] = (unsigned char)GetKeyForScanCode(i).order;
                        scanCodeCount++;
                    }
                }

                hotkey[editingHotkey].functionKeys = GetOrderedScanCodes(scanCodes, order);
            }
        }
        ImGui::SameLine(0.f, 20.f);
        if (ImGui::Button("Done", ImVec2(80, 40))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndGroup();
        ImGui::EndPopup();
    }

    static int GetHotKey(HotKey *hotkey, size_t hotkeyCount)
    {
        static unsigned int lastHotKey = 0xFFFFFFFF;
        unsigned char scanCodes[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
        unsigned char order[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
        int scanCodeCount = 0;
        for (int i = 0; i < 512; i++)
        {
            if (ImGui::IsKeyDown(i))
            {
                int imKey;
#ifdef WIN32
                imKey = MapVirtualKeyA(i, MAPVK_VK_TO_VSC);
#else
                imKey = i;
#endif
                scanCodes[scanCodeCount] = (unsigned char)imKey;
                order[scanCodeCount] = (unsigned char)GetKeyForScanCode(imKey).order;
                scanCodeCount++;
                if (scanCodeCount == 4)
                    break;
            }
        }

        unsigned int newHotKey = GetOrderedScanCodes(scanCodes, order);

        if (scanCodeCount && newHotKey != lastHotKey)
        {
            for (size_t i = 0; i < hotkeyCount; i++)
            {
                if (hotkey[i].functionKeys == newHotKey)
                {
                    lastHotKey = newHotKey;
                    return i;
                }
            }
        }
        lastHotKey = 0xFFFFFFFF;
        return -1;
    }
};
