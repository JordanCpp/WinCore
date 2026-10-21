// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/MessageQueue.hpp>

MessageQueue::MessageQueue()
{
}

void MessageQueue::Push(const MSG& msg)
{
    if (msg.message == WM_QUIT)
    {
        PostQuit(msg.wParam);
        return;
    }

    _queue.push_back(msg);
}

bool MessageQueue::Peek(MSG& msg, bool bRemove)
{
    if (_queue.empty())
    {
        return false;
    }

    msg = _queue.front();

    if (bRemove)
    {
        _queue.pop_front();
    }

    return true;
}

bool MessageQueue::Pop(MSG& msg)
{
    return Peek(msg, true);
}

bool MessageQueue::PeekFiltered(MSG& msg, bool bRemove, HWND hWnd, UINT wMin, UINT wMax)
{
    for (std::deque<MSG>::iterator it = _queue.begin(); it != _queue.end(); ++it)
    {
        // WM_QUIT — thread-level сообщение, возвращается всегда
        // независимо от фильтра (Win32-семантика).
        if (it->message == WM_QUIT)
        {
            msg = *it;

            if (bRemove)
            {
                _queue.erase(it);
            }

            return true;
        }

        if (hWnd != NULL && it->hwnd != hWnd)
        {
            continue;
        }

        if (it->message < wMin || it->message > wMax)
        {
            continue;
        }

        msg = *it;

        if (bRemove)
        {
            _queue.erase(it);
        }

        return true;
    }

    return false;
}

void MessageQueue::PostQuit(WPARAM exitCode)
{
    if (HasQuit())
    {
        return; // WM_QUIT уже в очереди — идемпотентно
    }

    MSG quitMsg;
    quitMsg.hwnd = NULL;
    quitMsg.message = WM_QUIT;
    quitMsg.wParam = exitCode;
    quitMsg.lParam = 0;
    quitMsg.time = 0;
    quitMsg.pt.x = 0;
    quitMsg.pt.y = 0;

    _queue.push_back(quitMsg);
}

bool MessageQueue::IsRunning() const
{
    return !HasQuit();
}

bool MessageQueue::Empty() const
{
    return _queue.empty();
}

size_t MessageQueue::Size() const
{
    return _queue.size();
}

bool MessageQueue::HasQuit() const
{
    for (std::deque<MSG>::const_iterator it = _queue.begin(); it != _queue.end(); ++it)
    {
        if (it->message == WM_QUIT)
        {
            return true;
        }
    }

    return false;
}
