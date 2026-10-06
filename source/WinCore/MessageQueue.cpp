// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/MessageQueue.hpp>

MessageQueue::MessageQueue() :
    _quit(false)
{
}

void MessageQueue::Push(const MSG& msg)
{
    _queue.push_back(msg);

    if (msg.message == WM_QUIT)
    {
        _quit = true;
    }
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

bool MessageQueue::PeekFiltered(MSG& msg, bool bRemove, HWND hWnd, UINT wMin, UINT wMax)
{
    for (std::deque<MSG>::iterator it = _queue.begin(); it != _queue.end(); ++it)
    {
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

bool MessageQueue::Pop(MSG& msg)
{
    return Peek(msg, true);
}

void MessageQueue::Quit()
{
    _quit = true;
}

bool MessageQueue::IsRunning() const
{
    return !_quit;
}

bool MessageQueue::Empty() const
{
    return _queue.empty();
}

size_t MessageQueue::Size() const
{
    return _queue.size();
}

void MessageQueue::PostQuit(WPARAM exitCode)
{
    if (_quit)
    {
        return;
    }

    MSG quitMsg = { 0 };
    quitMsg.message = WM_QUIT;
    quitMsg.wParam = exitCode;

    _queue.push_back(quitMsg);
    _quit = true;
}
