
extern int funcA(char* str);

int cmpstr(char *s1, char *s2)
{
    int s1Len =  funcA(s1);
    int s2Len = funcA(s2);

    int sharedSize = s1Len > s2Len ? s2Len : s1Len;

    int i = 0;
    while(i<sharedSize)
    {
        if (s1[i] != s2[i])
        {
            return s1[i]-s2[i];
        }
        i++;
    }
    if (s1Len > s2Len)
    {
        return s1[i];
    }
    if (s2Len > s1Len)
    {
        return -1 * s2[i];
    }
    return 0;
}