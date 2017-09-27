#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

static void strreplace(char *str, char a, char b)
{
    while (*str) {
        if (*str == a) *str = b;
        str++;
    }
}

static int readmac(char *file, uint64_t *start, uint64_t *end, uint64_t *cur)
{
    char line[256];
    uint32_t a, b, c, d, e, f;
    uint64_t mac = 0;
    FILE    *fp  = NULL;

    fp = fopen(file, "rb");
    if (!fp) return -1;

    // start
    fgets(line, 256, fp);
    strreplace(line, ':', ' ');
    sscanf(line, "%02x %02x %02x %02x %02x %02x", &a, &b, &c, &d, &e, &f);
    *start = ((uint64_t)a << 40) | ((uint64_t)b << 32) | ((uint64_t)c << 24) | ((uint64_t)d << 16) | ((uint64_t)e << 8) | ((uint64_t)f << 0);

    // end
    fgets(line, 256, fp);
    strreplace(line, ':', ' ');
    sscanf(line, "%02x %02x %02x %02x %02x %02x", &a, &b, &c, &d, &e, &f);
    *end = ((uint64_t)a << 40) | ((uint64_t)b << 32) | ((uint64_t)c << 24) | ((uint64_t)d << 16) | ((uint64_t)e << 8) | ((uint64_t)f << 0);

    // end
    fgets(line, 256, fp);
    strreplace(line, ':', ' ');
    sscanf(line, "%02x %02x %02x %02x %02x %02x", &a, &b, &c, &d, &e, &f);
    *cur = ((uint64_t)a << 40) | ((uint64_t)b << 32) | ((uint64_t)c << 24) | ((uint64_t)d << 16) | ((uint64_t)e << 8) | ((uint64_t)f << 0);

    fclose(fp);
    return 0;
}

static int writemac(char *file, uint64_t start, uint64_t end, uint64_t cur)
{
    FILE *fp = NULL;
    fp = fopen(file, "wb");
    if (!fp) return -1;
    fprintf(fp, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
        (uint32_t)((start >> 40) & 0xff),
        (uint32_t)((start >> 32) & 0xff),
        (uint32_t)((start >> 24) & 0xff),
        (uint32_t)((start >> 16) & 0xff),
        (uint32_t)((start >> 8 ) & 0xff),
        (uint32_t)((start >> 0 ) & 0xff));
    fprintf(fp, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
        (uint32_t)((end   >> 40) & 0xff),
        (uint32_t)((end   >> 32) & 0xff),
        (uint32_t)((end   >> 24) & 0xff),
        (uint32_t)((end   >> 16) & 0xff),
        (uint32_t)((end   >> 8 ) & 0xff),
        (uint32_t)((end   >> 0 ) & 0xff));
    fprintf(fp, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
        (uint32_t)((cur   >> 40) & 0xff),
        (uint32_t)((cur   >> 32) & 0xff),
        (uint32_t)((cur   >> 24) & 0xff),
        (uint32_t)((cur   >> 16) & 0xff),
        (uint32_t)((cur   >> 8 ) & 0xff),
        (uint32_t)((cur   >> 0 ) & 0xff));
    fclose(fp);
    return 0;
}

static int appendmac(char *file, uint64_t mac)
{
    FILE *fp = NULL;
    fp = fopen(file, "ab");
    if (!fp) return -1;
    fprintf(fp, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
        (uint32_t)((mac >> 40) & 0xff),
        (uint32_t)((mac >> 32) & 0xff),
        (uint32_t)((mac >> 24) & 0xff),
        (uint32_t)((mac >> 16) & 0xff),
        (uint32_t)((mac >> 8 ) & 0xff),
        (uint32_t)((mac >> 0 ) & 0xff));
    fclose(fp);
    return 0;
}

int main(void)
{
    uint64_t wifistart = 0;
    uint64_t wifiend   = 0;
    uint64_t wificur   = 0;
    uint64_t btstart   = 0;
    uint64_t btend     = 0;
    uint64_t btcur     = 0;
    FILE *fp = NULL;
    char  cmd [256];
    char  line[256];
    int   failed = 0;
    int   ret1, ret2;

    ret1 = readmac("wifimac.txt", &wifistart, &wifiend, &wificur);
    ret2 = readmac("btmac.txt"  , &btstart  , &btend  , &btcur  );
    if (ret1 == -1 || ret2 == -1) {
        printf("无效的 mac 地址配置！\n");
        return -1;
    }

    if (wificur > wifiend || wificur < wifistart) {
        printf("wifi mac 地址不在配置范围内，或者已经用完！\n");
        return -1;
    }
    if (btcur   > btend   || btcur   < btstart) {
        printf("bt mac 地址不在配置范围内，或者已经用完！\n");
        return -1;
    }

    printf("设置 wifi mac：%02x:%02x:%02x:%02x:%02x:%02x\n",
        (uint32_t)((wificur >> 40) & 0xff),
        (uint32_t)((wificur >> 32) & 0xff),
        (uint32_t)((wificur >> 24) & 0xff),
        (uint32_t)((wificur >> 16) & 0xff),
        (uint32_t)((wificur >> 8 ) & 0xff),
        (uint32_t)((wificur >> 0 ) & 0xff));
    printf("设置 bt   mac：%02x:%02x:%02x:%02x:%02x:%02x\n",
        (uint32_t)((btcur   >> 40) & 0xff),
        (uint32_t)((btcur   >> 32) & 0xff),
        (uint32_t)((btcur   >> 24) & 0xff),
        (uint32_t)((btcur   >> 16) & 0xff),
        (uint32_t)((btcur   >> 8 ) & 0xff),
        (uint32_t)((btcur   >> 0 ) & 0xff));
    sprintf(cmd, "setmac.bat %02x%02x%02x%02x%02x%02x %02x%02x%02x%02x%02x%02x",
        (uint32_t)((wificur >> 40) & 0xff),
        (uint32_t)((wificur >> 32) & 0xff),
        (uint32_t)((wificur >> 24) & 0xff),
        (uint32_t)((wificur >> 16) & 0xff),
        (uint32_t)((wificur >> 8 ) & 0xff),
        (uint32_t)((wificur >> 0 ) & 0xff),
        (uint32_t)((btcur   >> 0 ) & 0xff),
        (uint32_t)((btcur   >> 8 ) & 0xff),
        (uint32_t)((btcur   >> 16) & 0xff),
        (uint32_t)((btcur   >> 24) & 0xff),
        (uint32_t)((btcur   >> 32) & 0xff),
        (uint32_t)((btcur   >> 40) & 0xff));
    ret1 = system(cmd);
    if (ret1 == -1) failed = 1;

    fp = fopen("result.log", "rb");
    if (fp) {
        fread(line, 1, 256, fp);
        if (  strstr(line, "Interface doesn't accept private ioctl")
           || strstr(line, "BT Status not Active Write FAIL")) {
            failed = 1;
        }
        fclose(fp);
    } else failed = 1;
    unlink("result.log");

    if (failed) {
        printf("设置 mac 地址失败 ！\n");
        getch();
        return -1;
    }

    ret1 = appendmac("wifiused.log", wificur++);
    ret2 = appendmac("btused.log"  , btcur  ++);
    if (ret1 == -1 || ret2 == -1) {
        printf("记录已使用 mac 地址失败 ！\n");
        return -1;
    }

    ret1 = writemac("wifimac.txt", wifistart, wifiend, wificur);
    ret2 = writemac("btmac.txt"  , btstart  , btend  , btcur  );
    if (ret1 == -1 || ret2 == -1) {
        printf("更新 mac 地址配置失败 ！\n");
        return -1;
    }

    printf("设置 mac 地址成功 ！\n");
    getch();
}


