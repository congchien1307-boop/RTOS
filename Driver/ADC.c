#include "STM32_reg.h"
#include "ADC.h"
void ADC1_Init_Reg(void)
{
    /* 1. Bật clock ADC1 (APB2) + clock GPIOA (nếu chưa bật) */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1_AHB1ENR_GPIOAEN;

    /* 2. Cấu hình PA1 làm Analog mode (11 trong MODER) - GHI ĐÈ, không phải AF */
    GPIOA->MODER |= (3U << (1 * 2));   /* chân 1: set cả 2 bit = 1 -> Analog mode */

    /* 3. Cấu hình thời gian sample cho kênh 1 - chọn giá trị lớn (chậm nhưng
     * chính xác hơn) vì đo pin không cần tốc độ cao, ưu tiên độ ổn định */
    ADC1->SMPR2 |= (7U << ADC_SMPR2_SMP1_Pos);   /* 111 = 480 chu kỳ ADC clock, tối đa, chính xác nhất */

    /* 4. Chuỗi chuyển đổi: chỉ 1 kênh (IN1), đặt làm SQ1 duy nhất */
    ADC1->SQR3 = (1U << ADC_SQR3_SQ1_Pos);   /* SQ1 = kênh 1 */
    ADC1->SQR1 = 0;                           /* L[3:0]=0000 -> chuỗi chỉ có 1 kênh */

    /* 5. Bật ADC, chờ ổn định (tADC_STAB theo datasheet, thường vài µs) */
    ADC1->CR2 |= ADC_CR2_ADON;
    for (volatile uint32_t d = 0; d < 1000; d++) { }
}
uint16_t ADC1_ReadBatteryVoltage_mV(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;             /* bắt đầu 1 lần chuyển đổi */
    while (!(ADC1->SR & ADC_SR_EOC)) { }      /* chờ chuyển đổi xong */

    uint32_t raw = ADC1->DR;                   /* đọc kết quả (đồng thời tự xoá cờ EOC) */

    /* Quy đổi: raw (0-4095, 12-bit) -> điện áp thực tại chân PA1 (mV) -> nhân 2
     * vì qua chia áp R1=R2 (đổi hệ số "2" này nếu bạn dùng tỉ lệ trở khác).
     * VREF giả định = 3300mV (điện áp cấp VDDA thực tế của board bạn - nếu
     * board dùng LDO khác 3.3V đúng, cần đo lại VDDA thật để chính xác). */
    return (uint16_t)((raw * 3300UL * 2UL) / 4095UL);
}