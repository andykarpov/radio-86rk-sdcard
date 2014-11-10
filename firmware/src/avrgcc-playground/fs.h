/*
It is an open source software to implement FAT file system to
small embedded systems. This is a free software and is opened for education,
research and commercial developments under license policy of following trems.

(C) 2013-2014 vinxru
(C) 2010, ChaN, all right reserved.

It is a free software and there is NO WARRANTY.
No restriction on use. You can use, modify and redistribute it for
personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
Redistributions of source code must retain the above copyright notice.

Version 1.0 10-05-2014
*/

#ifndef _FS_H_
#define _FS_H_

#include "common.h"

/* Disable checking and kill the file system */
/* С помощью этого макроса можно отключить проеерки, что бы сэкономить ПЗУ и убить файловую систему.*/
/* #define FS_DISABLE_CHECK */

/* Two opened files */
/* Два открытых файла */
/* #define FS_DISABLE_SWAP */

/* Calculation of free disk space */
/* Определение свободного места на диске */
/* #define FS_DISABLE_GETFREESPACE */

// Что бы компиляторпоставил эту переменную первой в памяти
extern uint8_t rom[128];

/* Work buffer for filesystem */
/* Буфер для работы файловой системы */
extern uint8_t buf[512];

/* Error */
/* Ошибки файловой системы */

#define ERR_OK              0  // Нет ошибки
#define ERR_NO_FILESYSTEM   1  // Файловая система не обнаружена
#define ERR_DISK_ERR        2  // Ошибка чтения/записи
#define	ERR_NOT_OPENED      3  // Файл/папка не открыта
#define	ERR_NO_PATH         4  // Файл/папка не найдена
#define ERR_DIR_FULL        5  // Папка содержит максимальное кол-во файлов
#define ERR_NO_FREE_SPACE   6  // Нет свободного места
#define ERR_DIR_NOT_EMPTY   7  // Нельзя удалить папку, она не пуста
#define ERR_FILE_EXISTS     8  // Файл/папка с таким именем уже существует
#define ERR_NO_DATA         9  // fs_file_wtotal=0 при вызове функции fs_write_begin
    
#define ERR_MAX_FILES       10 // Не используется файловой системой, резерв
#define ERR_RECV_STRING     11 // Не используется файловой системой, резерв
#define ERR_INVALID_COMMAND 12 // Не используется файловой системой, резерв

#define ERR_ALREADY_OPENED  13 // Файл уже открыт (fs_swap)

/* Filesystem variables. Can change */
/* Переменные файловой системы. Можно изменять */

extern unsigned long fs_tmp;
extern uint8_t lastError;       /* Последняя ошибка файловой системы или диска */
extern WORD fs_wtotal;       /* Используется функциями fs_write_start, fs_write_end*/

/* Maximal length of file name */
/* Максимальная длина имени */

#define FS_MAXFILE  469

/* Result of fs_readdir */
/* Описатель прочитанный функцией fs_readdir */

#define FS_DIRENTRY  (buf + 480)

/* Functions. Returns not 0 if error occured. Destroy buf variable*/
/* Функцяии. Возращают не 0, если ошибка. Все функции портят buf */

uint8_t fs_init(void);                                  /* Инициалиазция файловой системы / Init filesystem */
uint8_t fs_check(void);                                 /* Проверка наличия диска, и если нужно, то его инициализация / Checking the disk and, if necessary, it will be initialized */
uint8_t fs_readdir(void);                               /* Прочитать содежимое папки в DIRENTRY / Read folder contents in DIRENTRY */
uint8_t fs_delete(void);                                /* Удалить файл или папку, имя в buf / Delete file or folder, name in buf */
uint8_t fs_open0(uint8_t what);                        /* Открыть/создать файл или папку, имя в buf. Open/create file or foder, name in buf */
uint8_t fs_move0(void);                                 /* Переместить файл/папку. Move file or folder, source file must been opened, destination name in buf */
uint8_t fs_move(const char* from, const char* to);  /* Переместить файл/папку. Move file or folder. */
uint8_t fs_lseek(unsigned long ptr, uint8_t mode);             /* Установить указатель чтения/записи файла, увеличивает размер файла. / Set file pointer, enlarge file size. */
uint8_t fs_tell(void);                                  /* Получить указатель чтения/записи файла в fs_tmp / Get file pointer in fs_tmp. */
uint8_t fs_getfilesize(void);                           /* Получить размер файла / Get file size. */
uint8_t fs_read0(uint8_t* ptr, WORD len);              /* Прочитать из файла. НЕЛЬЗЯ ВЫХОДИТЬ ЗА ПРЕДЕЛЫ ФАЙЛА! / Read from the file, DO NOT COME OUT FILE */
uint8_t fs_read(uint8_t* ptr, WORD len, WORD* readed); /* Прочитать из файла. / Read from the file. */
uint8_t fs_write(CONST uint8_t* ptr, WORD len);        /* Записать в файл, увеличивает размер файла. / Записать в файл, enlarge file size. */
uint8_t fs_write_eof(void);                             /* Установить конец файла, уменьшает размер файла. / Set end of file, reduces file size */
uint8_t fs_gettotal(void);                              /* Общее место на диске в fs_tmp в мегабайтах / Total disk space in fs_tmp (megabytes) */

#ifndef FS_DISABLE_SWAP
void fs_swap(void);                                  /* Переключится на второй файл / Switch to the second file */
#endif

#ifndef FS_DISABLE_GETFREESPACE
uint8_t fs_getfree(void);                               /* Свободное место на диске в fs_tmp  в мегабайтах / Free disk space in fs_tmp (megabytes) */
#endif


/* Функции ниже позволяют использовать для записи буфер (buf).
*  The functions below can be used to write the buffer (buf).
*
*  wtotal = размер данных для записи;
*  while(wtotal) {
*    fs_write_start(&wtotal);
*    Копируем fs_file_wlen байт в fs_file_wbuf
*    Ни одна функция ФС не должеа выпонятсья между ними
*    fs_write_end(&wtotal);
*  }
*
* See the source of fs_write
* Или смотри исходники fs_write
*/

#define fs_file_wlen   (*(WORD*)&fs_tmp)
#define fs_file_woff   ((WORD*)&fs_tmp)[1]
#define fs_file_wbuf   (buf + fs_file_woff)

uint8_t fs_write_start(void); 
uint8_t fs_write_end(void);

/* Derived from the function fs_open0 */
/* Производные от функции fs_open0 */

uint8_t fs_open(void);                                  /* Открыть файл */
uint8_t fs_opendir(void);                               /* Открыть папку */
#define fs_openany()    fs_open0(OPENED_NONE)    /* Открыть файл или папку */
#define fs_create()     fs_open0(OPENED_FILE)    /* Создать файл */
#define fs_createdir()  fs_open0(OPENED_DIR)     /* Создать папку */


/* Values ??fs_opened */
/* Значения fs_opened */

#define OPENED_NONE    0
#define OPENED_FILE    1
#define OPENED_DIR     2

/* File attributes */
/* Атрибуты файлов */

#define	AM_RDO         0x01  /* Read only */
#define	AM_HID         0x02  /* Hidden */
#define	AM_SYS         0x04  /* System */
#define	AM_VOL         0x08  /* Volume label */
#define AM_DIR         0x10  /* Directory */
#define AM_ARC         0x20  /* Archive */

/* Fodler descriptor (FS_DIRENTRY) */
/* Описатель */

#define	DIR_Name       0
#define	DIR_Attr       11
#define	DIR_NTres      12
#define	DIR_CrtTime    14
#define	DIR_CrtDate    16
#define	DIR_FstClusHI  20
#define	DIR_WrtTime    22
#define	DIR_WrtDate    24
#define	DIR_FstClusLO  26
#define	DIR_FileSize   28

#endif