#pragma once

namespace atomic::script
{
    void RunInitScripts();
    void RunShutdownScripts();
    bool StartLoudmice(char* outError, int outErrorSize);
}
