// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_MessageQueue_hpp
#define WinCore_MessageQueue_hpp

#include <deque>
#include <WinCore/Windows.h>

class MessageQueue
{
public:
    MessageQueue();

    void Push(const MSG& msg);
    bool Peek(MSG& msg, bool bRemove);
    bool PeekFiltered(MSG& msg, bool bRemove, HWND hWnd, UINT wMin, UINT wMax);
    bool Pop(MSG& msg);
    void Quit();
    bool IsRunning() const;
    bool Empty() const;
    size_t Size() const;
    void PostQuit(WPARAM exitCode);
private:
    std::deque<MSG> _queue;
    bool            _quit;
};

#endif