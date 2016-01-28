#ifndef STEPS_BY_SEJVIK
#define STEPS_BY_SEJVIK

int pow(int num, int n)
{
    int res = 1;
    while(n)
    {
        if (n & 1)
        {
            res *= num;
        }
        num *= num;
        n >>= 1;
    }
    return res;
}

int getSejvikStep(int index)
{
    if (index < 0)
        return 0;

    if (index % 2)
        return (8 * pow(2, index)) - (6 * pow(2, (index + 1) / 2)) + 1;

    else
        return (9 * pow(2, index)) - (9 * pow(2, index / 2)) + 1;
}

#endif // STEPS_BY_SEJVIK

