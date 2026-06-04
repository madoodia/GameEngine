// (C) 2026 madoodia.com
// ---------------------

#include <Core/logger.h>
#include <Core/asserts.h>

int main()
{
    GEFATAL("This is a FATALITY message: %f", 2026.1);
    GEERROR("This is a FATALITY message: %f", 2026.1);
    GEWARNING("This is a FATALITY message: %f", 2026.1);
    GEINFO("This is a FATALITY message: %f", 2026.1);
    GEDEBUG("This is a FATALITY message: %f", 2026.1);
    GETRACE("This is a FATALITY message: %f", 2026.1);

    GEASSERT(1 == 2);
    GEASSERT_MSG(1 == 2, "Should be same");
    GEASSERT_DEBUG('A' == 'C');

    return 0;
}
