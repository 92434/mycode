

/*================================================================
 *   Copyright (C) 2017年07月12日 肖飞 All rights reserved
 *   
 *   文件名称：ft_lib.h
 *   创 建 者：肖飞
 *   创建日期：2017年07月12日 星期三 11时57分49秒
 *   修改日期：2017年07月13日 星期四 18时15分44秒
 *   描    述：
 *
 *================================================================*/
#ifndef _FT_LIB_H
#define _FT_LIB_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*ft_printf_t)(char *fmt, ...);
typedef int (*save_bmp_t)(char *label, char *buffer, int len);

int ft_lib_init();
int ft_lib_uninit();
int ft_lib_set_log(ft_printf_t ft_printf);
int ft_lib_set_save_bmp(save_bmp_t save_bmp);
int ft_get_image(char *buffer, int len);
int ft_set_image(const char *buffer, int len);

#ifdef __cplusplus
}
#endif
#endif //FT_LIB_H
