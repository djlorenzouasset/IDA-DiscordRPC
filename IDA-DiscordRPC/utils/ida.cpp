#include "ida.h"

ea_t Ida::GetCursorAddr()
{
    ea_t cursorAddr = get_screen_ea();
    if (!is_code(get_flags(cursorAddr))) return NULL;
    return cursorAddr;
}

ea_t Ida::GetFunctionAddr()
{
    return get_func_start(GetCursorAddr());
}

const char* Ida::GetDBFilename()
{
    char filename[MAXSTR];
    ssize_t readSize = get_root_filename(filename, sizeof(filename));
    return readSize > 0 ? filename : nullptr;
}

const char* Ida::GetFunctionName()
{
    ea_t functionAddr = GetFunctionAddr();
    if (!functionAddr) return nullptr;

    qstring functionName;
    ssize_t sizeRead = get_func_name(&functionName, functionAddr);
    return sizeRead > 0 ? functionName.c_str() : nullptr;
}
