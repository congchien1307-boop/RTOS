#include "STM32_reg.h"
#define GPIO_MODER_0_POS  0
#define GPIO_MODER_3_POS  6
#define GPIO_MODER_4_POS  8
#define GPIO_MODER_5_POS  10
#define GPIO_MODER_6_POS  12
#define GPIO_MODER_7_POS  14
#define GPIO_OTYPER_6  (1 << 6)
#define GPIO_OTYPER_7  (1 << 7)
#define GPIO_OTYPER_3     (1 << 3)
#define GPIO_OTYPER_4     (1 << 4)
/* OSPEEDR */
#define GPIO_OSPEEDR_5_POS  10
#define GPIO_OSPEEDR_6_POS  12
#define GPIO_OSPEEDR_7_POS  14
/* BSRR */
#define GPIO_BSRR_BS4    (1 << 4)     /* set chân 4 lên cao */
#define GPIO_BSRR_BR4    (1 << (16+4)) /* kéo chân 4 xuống thấp */
#define GPIO_BSRR_BS3    (1 << 3)
#define GPIO_BSRR_BR3    (1 << (16+3))

